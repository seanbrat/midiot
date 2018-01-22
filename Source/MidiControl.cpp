//
//  MidiControl.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/20/17.
//
//

#include "MidiControl.hpp"
#include "MidiInstrument.hpp"
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

void MidiControl::handleMidiControlEvent(const MidiMessage& message)
{
    value_ = message.getControllerValue();

    const MessageManagerLock mm_lock;
    
    if (midi_control_slider_)
    {
        midi_control_slider_->setValue(value_, dontSendNotification);
//        midi_control_slider_-
    }
}

void MidiControl::send_value_to_midi()
{
    if (cc_control_)
    {
        midi_instrument_->sendControllerEvent(midi_instrument_->channel()+1,
                                              cc_control_->number(),
                                              value_);
    }
    else if (sysex_control_)
    {
        
    }
}

void MidiControl::setMidiInstrument(MidiInstrument* midi_instrument)
{
    midi_instrument_ = midi_instrument;
}

void MidiControl::setMidiControlSlider(MidiControlSlider* control_slider)
{
    midi_control_slider_ = control_slider;
}


void MidiControl::sliderValueChanged (Slider *slider)
{
    //printf("MidiControl::sliderValueChanged() with value: %f\n", slider->getValue());
    
    value_ = slider->getValue();
    
    if (cc_control_)
    {
        midi_instrument_->sendControllerEvent(midi_instrument_->channel()+1,
                                              cc_control_->number(),
                                              value_);
    }
    else if (sysex_control_)
    {
        
    }
}

void MidiControl::set_value(const int value, bool update_slider)
{
    printf("MidiControl::set_value(%d) for control %s\n", value, name_.toRawUTF8());
    value_ = value;
    
    if (update_slider)
    {
        const MessageManagerLock mm_lock;
        
        if (midi_control_slider_)
        {
            midi_control_slider_->setValue(value_, dontSendNotification);
            //        midi_control_slider_-
        }
    }
}

