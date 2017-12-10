//
//  MidiInterface.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#include "MidiInterface.hpp"
#include "MidiInstrument.hpp"

MidiInputPort::MidiInputPort(String name)
: name_(name)
{

}

MidiInputPort::~MidiInputPort()
{
    
}


void MidiInputPort::add_instrument_to_port(MidiInstrument* instrument)
{
    short inst_channel = instrument->channel();
    
    channel_routing_table_[inst_channel].addIfNotAlreadyThere(instrument);
}

void MidiInputPort::remove_instrument_from_port(MidiInstrument* instrument)
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
    
}

void MidiInputPort::handleNoteOff (MidiKeyboardState* keyboard_state,
                    int midi_channel,
                    int midi_note_number,
                    float velocity)
{
    
}

MidiOutputPort::MidiOutputPort(String name, MidiOutput* midi_output)
: name_(name),
midi_output_(midi_output)
{
    
}

MidiOutputPort::~MidiOutputPort()
{
    
}


MidiInterface::MidiInterface()
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
        printf("added MidiInputPort %s\n", input_name.toRawUTF8());
    }
    
    const StringArray midi_outputs(MidiOutput::getDevices());
    
    // find the first enabled device and use that by default
    for (int i = 0; i < midi_outputs.size(); ++i)
    {
        const String output_name(midi_outputs[i]);
        
        MidiOutput* midi_output = MidiOutput::openDevice(i);
        
        MidiOutputPort* midi_output_port = new MidiOutputPort(output_name, midi_output);

        output_ports_.add(midi_output_port);
        printf("added MidiOutputPort %s\n", output_name.toRawUTF8());

    }

}

MidiInterface::~MidiInterface()
{
    
}
