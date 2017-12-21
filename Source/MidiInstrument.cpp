//
//  MidiInstrumentModel.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#include "MidiInstrument.hpp"
#include "MidiInterface.hpp"
#include "MidiInstrumentControllerComponent.hpp"
#include "MidiControl.hpp"



void MidiInstrumentModel::handleMidiControlEvent(const MidiMessage& message)
{
    int controller_number = message.getControllerNumber();

    midi_controls_[cc_redirect_table_[controller_number]]->handleMidiControlEvent(message);
}

MidiInstrument::MidiInstrument(MidiInstrumentModel* inst_model,
                               MidiInstrumentControllerComponent* controller,
                               MidiInputPort* input_port,
                               MidiOutputPort* output_port)
: inst_model_(inst_model),
channel_(0),
midi_input_port_(input_port),
midi_output_port_(output_port),
controller_component_(controller)
{
    // Hook up keyboard component to midi_input_port_
    controller_component_->addMidiKeyboardStateListener(this);
    
    MidiControl** ctrl_iter = inst_model->getMidiControlIterator();
    
    for (ctrl_iter;
         ctrl_iter != inst_model->getMidiControlIteratorEnd();
         ctrl_iter++)
    {
        MidiControl* midi_control = *ctrl_iter;
        printf("adding MidiControl: %s\n", midi_control->name().toRawUTF8());
        setupMidiControlInterface(midi_control);
    }

    midi_input_port_->addInstrumentToPort(this);
}

MidiInstrument::~MidiInstrument()
{
    if (controller_component_)
    {
        controller_component_->addMidiKeyboardStateListener(this);
    }
}

void MidiInstrument::setupMidiControlInterface(MidiControl* midi_control)
{
    MidiControlSlider* control_slider = controller_component_->addMidiControlSlider(midi_control);
    midi_control->setMidiInstrument(this);
    midi_control->setMidiControlSlider(control_slider);
    midi_input_port_->addInstrumentToPort(this);
}

void MidiInstrument::listenToControllerComponentKeyboard()
{
    
}

void MidiInstrument::set_instrument_id(int instrument_id)
{
    instrument_id_ = instrument_id;
}


void MidiInstrument::sendControllerEvent(int midi_channel,
                                         int controller_type,
                                         int value)
{
    MidiMessage m(MidiMessage::controllerEvent(midi_channel, controller_type, value));
    m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.0001);
    midi_output_port_->sendMessageNow(m);
}

void MidiInstrument::sendNoteOn(int midi_channel,
                                int midi_note_number,
                                float velocity)
{
    MidiMessage m (MidiMessage::noteOn (midi_channel, midi_note_number, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    midi_output_port_->sendMessageNow(m);
}

void MidiInstrument::sendNoteOff(int midi_channel,
                                 int midi_note_number,
                                 float velocity)
{
    MidiMessage m (MidiMessage::noteOff (midi_channel, midi_note_number, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    midi_output_port_->sendMessageNow(m);
}



void MidiInstrument::handleNoteOn (MidiKeyboardState* keyboard_state,
                   int midi_channel,
                   int midi_note_number,
                   float velocity)
{
    printf("MidiInstrument::handleNoteOn() with midi_channel: %d note: %d velocity: %f\n", midi_channel, midi_note_number, velocity);
    midi_output_port_->sendNoteOn(midi_channel, midi_note_number, velocity);
}

void MidiInstrument::handleNoteOff (MidiKeyboardState* keyboard_state,
                    int midi_channel,
                    int midi_note_number,
                    float velocity)
{
    printf("MidiInstrument::handleNoteOff()\n");
    midi_output_port_->sendNoteOff(midi_channel, midi_note_number, velocity);

}

void MidiInstrument::handleIncomingMidiMessage(const MidiMessage& message)
{
    if (message.isNoteOn() || message.isNoteOff())
    {
        printf("MidiInstrument::handleIncomingMidiMessage() with note\n");
        controller_component_->processNextKeyboardMidiEvent(message);
    }
    else if (message.isController())
    {
        printf("MidiInstrument::handleIncomingMidiMessage() with controller\n");
        inst_model_->handleMidiControlEvent(message);
    }
}



