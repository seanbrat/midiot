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

MidiControl::ContinuousControl* createContinuousControl(short number,
                                                                short range_min,
                                                                short range_max)
{
    return new MidiControl::ContinuousControl(number,
                                                      range_min,
                                                      range_max);
}

MidiControl::SysexControl* createSysexControl(short param_table,
                                                      short address_high,
                                                      short address_mid,
                                                      short address_low,
                                                      short size_bytes,
                                                      int range_min,
                                                      int range_max)
{
    return new MidiControl::SysexControl(param_table,
                                                 address_high,
                                                 address_mid,
                                                 address_low,
                                                 size_bytes,
                                                 range_min,
                                                 range_max);
}

void MidiControl::sliderValueChanged (Slider *slider)
{
    printf("MidiControl::sliderValueChanged()\n");
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
    controller_component_->addMidiControlSlider(midi_control);
    midi_control->setMidiOutputPort(midi_output_port_);
}

void MidiInstrument::listenToControllerComponentKeyboard()
{
    
}

void MidiInstrument::set_instrument_id(int instrument_id)
{
    instrument_id_ = instrument_id;
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
