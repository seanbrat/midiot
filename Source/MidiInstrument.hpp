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
#include "MidiInstrumentModel.hpp"

class MidiInputPort;
class MidiOutputPort;
class MidiInstrumentControllerComponent;

class MidiInstrument : public MidiKeyboardStateListener
{
public:
    MidiInstrument(MidiInstrumentModel* inst_model,
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
    
    bool updateMidiControl(String control_name, int control_value, bool sendMidiOnUpdate = false);
    
    void sendMidiControlPatchData();
    
    // 1. Adds UI slider for MidiControl to MidiInstrumentControllerComponent
    // 2. Adds MidiInstrument to MidiInstrumentControllerComponent
    // 3. Adds MidiInstrument pointer to MidiControl
    // 4. Adds MidiControlSlider to MidiControl
    void setupMidiControlInterface(MidiControl* midi_control);
    
    void handleIncomingMidiMessage(const MidiMessage& message);

    void sendSysexPatchDumpMessage();
    
    var getInstrumentParametersVar();
    var getPatchVar(String patch_name);
    
    String getManufacturerName();
    String getModelName();
    
private:
    ScopedPointer<MidiInstrumentModel> inst_model_;
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
