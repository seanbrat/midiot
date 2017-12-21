//
//  MidiControl.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/20/17.
//
//

#include "MidiControl.hpp"
#include "MidiInstrument.hpp"


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
    printf("MidiControl::sliderValueChanged() with value: %f\n", slider->getValue());
    
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
