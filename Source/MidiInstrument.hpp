//
//  MidiInstrument.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#ifndef MidiInstrument_hpp
#define MidiInstrument_hpp

#include <stdio.h>

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiDefines.hpp"
#include "MidiControl.hpp"

class MidiInputPort;
class MidiOutputPort;
class MidiInstrumentControllerComponent;

class MidiInstrumentBaseModel
{
    
public:
    MidiInstrumentBaseModel(String name)
    :
    name_(name)
    {
        for (int i=0; i<NUM_MIDI_CC; i++)
        {
            cc_redirect_table_[i] = -1;
        }
    }
    
    ~MidiInstrumentBaseModel()
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
    
    void handleMidiControlEvent(const MidiMessage& message);
    void handleMidiSysexEvent(const MidiMessage& message);
    
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


class MidiInstrument : public MidiKeyboardStateListener
{
public:
    MidiInstrument(MidiInstrumentBaseModel* inst_model,
                   MidiInstrumentControllerComponent* controller,
                   MidiInputPort* input_port,
                   MidiOutputPort* output_port);
    ~MidiInstrument();
    
    void handleNoteOn (MidiKeyboardState* keyboard_state,
                       int midi_channel,
                       int midi_note_number,
                       float velocity) override;
    void handleNoteOff (MidiKeyboardState* keyboard_state,
                        int midi_channel,
                        int midi_note_number,
                        float velocity) override;
    
    void listenToControllerComponentKeyboard();
    
    void set_instrument_id(int instrument_id);


    void sendNoteOn(int midi_channel,
                    int midi_note_number,
                    float velocity);
    void sendNoteOff(int midi_channel,
                     int midi_note_number,
                     float velocity);
    void sendControllerEvent(int midi_channel,
                             int controller_type,
                             int value);
    
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
    
    // 1. Adds UI slider for MidiControl to MidiInstrumentControllerComponent
    // 2. Adds MidiInstrument to MidiInstrumentControllerComponent
    // 3. Adds MidiInstrument pointer to MidiControl
    // 4. Adds MidiControlSlider to MidiControl
    void setupMidiControlInterface(MidiControl* midi_control);
    
    void handleIncomingMidiMessage(const MidiMessage& message);
    
    void sendSysexPatchDumpMessage();
    
private:
    ScopedPointer<MidiInstrumentBaseModel> inst_model_;
    int instrument_id_;
    
    // MIDI channel
    short channel_;
    
    // MIDI input port
    MidiInputPort* midi_input_port_;
    
    // MIDI output port
    MidiOutputPort* midi_output_port_;
    
    MidiInstrumentControllerComponent* controller_component_;
};

#endif /* MidiInstrument_hpp */
