//
//  MidiInstrumentModel.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/30/17.
//
//

#ifndef MidiInstrumentModel_hpp
#define MidiInstrumentModel_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiDefines.hpp"
#include "MidiControl.hpp"

const int SYSEX_MODEL_ID_BYTES = 16;

class MidiInstrumentModel
{
public:
    MidiInstrumentModel(String manufacturer, String model_name)
    :
    manufacturer_(manufacturer),
    model_name_(model_name),
    sysex_manufacturer_id_(0),
    sysex_device_id_(0),
    sysex_model_id_bytes_(0)
    {
        for (int i=0; i<NUM_MIDI_CC; i++)
        {
            cc_redirect_table_[i] = -1;
        }
        
        for (int i=0; i<SYSEX_MODEL_ID_BYTES; i++)
        {
            sysex_model_id_[i] = 0;
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
    
    bool handleMidiControlEvent(const MidiMessage& message);
    virtual bool handleMidiSysexEvent(const MidiMessage& message) { return false; }

    virtual bool handleSysexIdRequest(const MidiMessage& message) { return false; }
    virtual bool handleSysexPatchDumpRequest(const MidiMessage& message) { return false; }
    virtual bool handleSysexParameterRequest(const MidiMessage& message) { return false; }
    
    const String manufacturer();
    const String model_name();
    
protected:
    OwnedArray<MidiControl> midi_controls_;
    int cc_redirect_table_[NUM_MIDI_CC];
    Identifier model_name_;
    Identifier manufacturer_;
    
    uint8 sysex_manufacturer_id_;
    uint8 sysex_device_id_;
    uint8 sysex_model_id_[SYSEX_MODEL_ID_BYTES];
    uint8 sysex_model_id_bytes_;
};




#endif /* MidiInstrumentModel_hpp */
