//
//  MidiInterface.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#include "MidiInterface.hpp"
#include "MidiInstrument.hpp"

MidiInputPort::MidiInputPort(const String name)
: name_(name)
{

}

MidiInputPort::~MidiInputPort()
{
    
}


void MidiInputPort::addInstrumentToPort(MidiInstrument* instrument)
{
    short inst_channel = instrument->channel();
    
    channel_routing_table_[inst_channel].addIfNotAlreadyThere(instrument);
}

void MidiInputPort::removeInstrumentFromPort(MidiInstrument* instrument)
{
    short inst_channel = instrument->channel();
    
    int inst_index = channel_routing_table_[inst_channel].indexOf(instrument);
    
    if (inst_index >= 0)
    {
        channel_routing_table_[inst_channel].remove(inst_index);
    }
}

void MidiInputPort::handleIncomingMidiMessage(MidiInput* source,
                                              const MidiMessage& message)
{
    short message_channel = message.getChannel();

    if (message.isNoteOn())
    {
        printf("MidiInputPort::handleIncomingMidiMessage() with note\n");
        printf("message_channel: %d\n", message_channel);
    }
    else if (message.isController())
    {
        printf("MidiInputPort::handleIncomingMidiMessage() with controller\n");
        printf("message_channel: %d\n", message_channel);
    }
    
    Array<MidiInstrument*> channel_instruments = channel_routing_table_[message_channel-1];
    
    for (int i=0; i<channel_instruments.size(); i++)
    {
        channel_instruments[i]->handleIncomingMidiMessage(message);
    }

}

void MidiInputPort::handlePartialSysexMessage(MidiInput* source,
                                              const uint8* messageData,
                                              int numBytesSoFar,
                                              double timestamp)
{
    
}


void MidiInputPort::handleNoteOn (MidiKeyboardState* keyboard_state,
                   int midi_channel,
                   int midi_note_number,
                   float velocity)
{
    printf("MidiInputPort::handleNoteOn() with note number: %d\n", midi_note_number);
}

void MidiInputPort::handleNoteOff (MidiKeyboardState* keyboard_state,
                    int midi_channel,
                    int midi_note_number,
                    float velocity)
{
    
}

MidiOutputPort::MidiOutputPort(const String name, MidiOutput* midi_output)
: name_(name),
midi_output_(midi_output)
{
}

MidiOutputPort::~MidiOutputPort()
{
}

void MidiOutputPort::sendControllerEvent(int midi_channel,
                         int controller_type,
                         int value)
{
    MidiMessage m(MidiMessage::controllerEvent(midi_channel, controller_type, value));
    m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.0001);
    midi_output_->sendMessageNow(m);
}

void MidiOutputPort::sendNoteOn(int midi_channel,
                                int midi_note_number,
                                float velocity)
{
    MidiMessage m (MidiMessage::noteOn (midi_channel, midi_note_number, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    midi_output_->sendMessageNow(m);
}

void MidiOutputPort::sendNoteOff(int midi_channel,
                                 int midi_note_number,
                                 float velocity)
{
    MidiMessage m (MidiMessage::noteOff (midi_channel, midi_note_number, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    midi_output_->sendMessageNow(m);
}

void MidiOutputPort::sendMessageNow(const MidiMessage& message)
{
    midi_output_->sendMessageNow(message);
}


MidiInterface::MidiInterface()
: device_manager_(new AudioDeviceManager())
{
    const StringArray midi_inputs(MidiInput::getDevices());
    
    // find the first enabled device and use that by default
    for (int i = 0; i < midi_inputs.size(); ++i)
    {
        const String input_name(midi_inputs[i]);
        
        if (!device_manager_->isMidiInputEnabled(input_name))
        {
            device_manager_->setMidiInputEnabled(input_name, true);
        }

        MidiInputPort* midi_input_port = new MidiInputPort(input_name);
        device_manager_->addMidiInputCallback(input_name, midi_input_port);
        
        input_ports_.add(midi_input_port);
        printf("added MidiInputPort %d: %s\n", i, input_name.toRawUTF8());
    }
    
    const StringArray midi_outputs(MidiOutput::getDevices());
    
    // find the first enabled device and use that by default
    for (int i = 0; i < midi_outputs.size(); ++i)
    {
        const String output_name(midi_outputs[i]);
        
        MidiOutput* midi_output = MidiOutput::openDevice(i);
        
        MidiOutputPort* midi_output_port = new MidiOutputPort(output_name, midi_output);

        output_ports_.add(midi_output_port);
        printf("added MidiOutputPort %d: %s\n", i, output_name.toRawUTF8());
    }
}

MidiInterface::~MidiInterface()
{
    
}

MidiInputPort* MidiInterface::getMidiInputPort(int port_index)
{
    if (port_index < 0 && port_index >= input_ports_.size())
    {
        return NULL;
    }
    
    return input_ports_[port_index];
}

MidiInputPort* MidiInterface::getMidiInputPort(String name)
{
    MidiInputPort* midi_port = NULL;
    
    for (int i=0; i<input_ports_.size(); i++)
    {
        if (input_ports_[i]->name() == name)
        {
            midi_port = input_ports_[i];
            break;
        }
    }
    
    return midi_port;
}

MidiOutputPort* MidiInterface::getMidiOutputPort(int port_index)
{
    if (port_index < 0 && port_index >= input_ports_.size())
    {
        return NULL;
    }
    
    return output_ports_[port_index];
}

MidiOutputPort* MidiInterface::getMidiOutputPort(String name)
{
    MidiOutputPort* midi_port = NULL;
    
    for (int i=0; i<output_ports_.size(); i++)
    {
        if (output_ports_[i]->name() == name)
        {
            midi_port = output_ports_[i];
            break;
        }
    }
    
    return midi_port;
}
