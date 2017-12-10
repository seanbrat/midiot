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

class MidiInstrumentModel
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
        
    private:
        const short param_table_;
        const short address_high_;
        const short address_mid_;
        const short address_low_;
        const short size_bytes_;
        const int range_min_;
        const int range_max_;
    };
    
    class MidiControl
    {
    public:
        MidiControl(Identifier name,
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
        
        void set_value(const int value) { value_ = value; }
        const int value() { return value_; }
        
    private:
        ScopedPointer<ContinuousControl> cc_control_;
        ScopedPointer<SysexControl> sysex_control_;
        
        Identifier name_;
        int control_id_;
        
        int value_;
    };
    
public:
    MidiInstrumentModel(Identifier name)
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
    
    int addMidiControl(Identifier name,
                       int intital_value = 0,
                       ContinuousControl* cc_control = NULL,
                       SysexControl* sysex_control = NULL)
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
    

    
private:
    OwnedArray<MidiControl> midi_controls_;
    int cc_redirect_table_[NUM_MIDI_CC];
    Identifier name_;
};


MidiInstrumentModel::ContinuousControl* createContinuousControl(short number,
                                                                short range_min = 0,
                                                                short range_max = 127);

MidiInstrumentModel::SysexControl* createSysexControl(short param_table = 0,
                                                      short address_high = 0,
                                                      short address_mid = 0,
                                                      short address_low = 0,
                                                      short size_bytes = 0,
                                                      int min_range = 0,
                                                      int max_range = 0);


class MidiInstrument
{
public:
    MidiInstrument(MidiInstrumentModel* inst_model);
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
    
private:
    ScopedPointer<MidiInstrumentModel> inst_model_;
    int instrument_id_;
    
    // MIDI channel
    short channel_;
    
    // MIDI input port
    // MIDI output port
    
};

#endif /* MidiInstrumentModel_hpp */
