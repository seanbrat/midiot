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
midi_output_port_(output_port)
{

    MidiControl** ctrl_iter = inst_model->getMidiControlIterator();
    
    for (ctrl_iter;
         ctrl_iter != inst_model->getMidiControlIteratorEnd();
         ctrl_iter++)
    {
        MidiControl* midi_control = *ctrl_iter;
        printf("adding MidiControl: %s\n", midi_control->name().toRawUTF8());
        controller->addMidiControlSlider(midi_control);
    }

}

MidiInstrument::~MidiInstrument()
{}

void MidiInstrument::set_instrument_id(int instrument_id)
{
    instrument_id_ = instrument_id;
}


