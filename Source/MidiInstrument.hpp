//
//  MidiInstrument.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#ifndef MidiInstrumentModel_hpp
#define MidiInstrumentModel_hpp

#include <stdio.h>

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiDefines.hpp"

class MidiInputPort;
class MidiOutputPort;
class MidiInstrumentControllerComponent;


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
    value_(initial_value)
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
    
    String name() { return name_; }
    
private:
    ScopedPointer<ContinuousControl> cc_control_;
    ScopedPointer<SysexControl> sysex_control_;

    String name_;
    
    int control_id_;
    
    int value_;
};


class MidiInstrumentModel
{
    
public:
    MidiInstrumentModel(String name)
    :
    name_(name)
    {
        for (int i=0; i<NUM_MIDI_CC; i++)
        {
            cc_redirect_table_[i] = -1;
        }
    }
    
    ~MidiInstrumentModel()
    {}
    
    int addMidiControl(String name,
                       int intital_value = 0,
                       MidiControl::ContinuousControl* cc_control = NULL,
                       MidiControl::SysexControl* sysex_control = NULL)
    {
        int control_id = midi_controls_.size();
        
        midi_controls_.add(new MidiControl(name,
                                           control_id,
                                           intital_value,
                                           cc_control,
                                           sysex_control));
        
        if (cc_control)
        {
            cc_redirect_table_[cc_control->number()] = control_id;
        }
        
        return control_id;
    }
    
    MidiControl** getMidiControlIterator() { return midi_controls_.begin(); }
    MidiControl** getMidiControlIteratorEnd() { return midi_controls_.end(); }

    
private:
    OwnedArray<MidiControl> midi_controls_;
    int cc_redirect_table_[NUM_MIDI_CC];
    Identifier name_;
};


MidiControl::ContinuousControl* createContinuousControl(short number,
                                                                short range_min = 0,
                                                                short range_max = 127);

MidiControl::SysexControl* createSysexControl(short param_table = 0,
                                                      short address_high = 0,
                                                      short address_mid = 0,
                                                      short address_low = 0,
                                                      short size_bytes = 0,
                                                      int min_range = 0,
                                                      int max_range = 0);


class MidiInstrument
{
public:
    MidiInstrument(MidiInstrumentModel* inst_model,
                   MidiInstrumentControllerComponent* controller,
                   MidiInputPort* input_port,
                   MidiOutputPort* output_port);
    ~MidiInstrument();
    
    void set_instrument_id(int instrument_id);


    short channel() { return channel_; }
    void set_channel(short channel)
    {
        if (channel >= 0 && channel < NUM_MIDI_CHANNELS)
        {
            channel_ = channel;
        }
    }
    
    void setMidiInputPort(MidiInputPort* midi_input_port)
    {
        midi_input_port_ = midi_input_port;
    }
    
    void setMidiOutputPort(MidiOutputPort* midi_output_port)
    {
        midi_output_port_ = midi_output_port;
    }
    
    MidiControl** getMidiControlIterator()
    {
        return inst_model_->getMidiControlIterator();
    }
    
private:
    ScopedPointer<MidiInstrumentModel> inst_model_;
    int instrument_id_;
    
    // MIDI channel
    short channel_;
    
    // MIDI input port
    MidiInputPort* midi_input_port_;
    
    // MIDI output port
    MidiOutputPort* midi_output_port_;
    
};

#endif /* MidiInstrumentModel_hpp */
