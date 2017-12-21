//
//  MidiControl.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/20/17.
//
//

#ifndef MidiControl_hpp
#define MidiControl_hpp

#include <stdio.h>

#include "../JuceLibraryCode/JuceHeader.h"


class MidiInputPort;
class MidiOutputPort;
class MidiInstrumentControllerComponent;
class MidiControlSlider;
class MidiInstrument;

class MidiControl
: public SliderListener
{
public:
    class ContinuousControl
    {
    public:
        ContinuousControl(short number,
                          short range_min = 0,
                          short range_max = 127)
        : number_(number),
        range_min_(range_min),
        range_max_(range_max)
        {}
        
        ~ContinuousControl()
        {}
        
        const short number() { return number_; }
        const short range_min() { return range_min_; }
        const short range_max() { return range_max_; }
        
    private:
        const short number_;
        const short range_min_;
        const short range_max_;
    };
    
    class SysexControl
    {
    public:
        enum ValueDisplayType {
            SignedIntRange = 0,
            UnsignedIntRange,
            SignedFloatRange
        };
        
        SysexControl(short param_table = 0,
                     short address_high = 0,
                     short address_mid = 0,
                     short address_low = 0,
                     short size_bytes = 0,
                     int range_min = 0,
                     int range_max = 0)
        : param_table_(param_table),
        address_high_(address_high),
        address_mid_(address_mid),
        address_low_(address_low),
        size_bytes_(size_bytes),
        range_min_(range_min),
        range_max_(range_max)
        {};
        
        ~SysexControl() {};
        
        const short address_high() { return address_high_; }
        const short address_mid() { return address_mid_; }
        const short address_low() { return address_low_; }
        
        const short size_bytes() { return size_bytes_; }
        const int range_min() { return range_min_; }
        const int range_max() { return range_max_; }
        
    private:
        const short param_table_;
        const short address_high_;
        const short address_mid_;
        const short address_low_;
        const short size_bytes_;
        const int range_min_;
        const int range_max_;
    };
    
public:
    MidiControl(String name,
                int control_id,
                int initial_value,
                ContinuousControl* cc_control = NULL,
                SysexControl* sysex_control = NULL)
    : name_(name),
    control_id_(control_id),
    value_(initial_value),
    midi_control_slider_(NULL)
    {
        if (cc_control)
        {
            cc_control_ = cc_control;
        }
        
        if (sysex_control)
        {
            sysex_control_ = sysex_control;
        }
    }
    
    ~MidiControl() {};
    
    int getRangeMinimum()
    {
        int range_min = 0;
        
        if (cc_control_)
        {
            range_min = cc_control_->range_min();
        }
        
        if (sysex_control_)
        {
            range_min = sysex_control_->range_min();
        }
        
        return range_min;
    }
    
    int getRangeMaximum()
    {
        int range_max = 0;
        
        if (cc_control_)
        {
            range_max = cc_control_->range_max();
        }
        
        if (sysex_control_)
        {
            range_max = sysex_control_->range_max();
        }
        
        return range_max;
    }
    
    void set_value(const int value) { value_ = value; }
    const int value() { return value_; }
    
    void sliderValueChanged (Slider *slider) override;
    void handleMidiControlEvent(const MidiMessage& message);
    
    String name() { return name_; }
    
    void setMidiInstrument(MidiInstrument* midi_instrument);
    void setMidiControlSlider(MidiControlSlider* control_slider);
    
private:
    ScopedPointer<ContinuousControl> cc_control_;
    ScopedPointer<SysexControl> sysex_control_;
    
    String name_;
    int control_id_;
    int value_;
    
    MidiInstrument* midi_instrument_;
    
    MidiControlSlider* midi_control_slider_;
};


#endif /* MidiControl_hpp */
