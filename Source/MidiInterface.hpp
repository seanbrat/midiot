//
//  MidiInterface.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#ifndef MidiInterface_hpp
#define MidiInterface_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiDefines.hpp"

#include <stdio.h>

class MidiInstrument;

class MidiInputPort :
public MidiInputCallback,
public MidiKeyboardStateListener
{
public:
    MidiInputPort(const String name);
    ~MidiInputPort();

    void add_instrument_to_port(MidiInstrument* instrument);
    void remove_instrument_from_port(MidiInstrument* instrument);

    const String name() { return name_; }
    
private:
    void handleIncomingMidiMessage (MidiInput* source,
                                    const MidiMessage& message) override;
    void handlePartialSysexMessage(MidiInput* source,
                                   const uint8* message_data,
                                   int num_bytes_so_far,
                                   double timestamp) override;
    
    void handleNoteOn (MidiKeyboardState* keyboard_state,
                       int midi_channel,
                       int midi_note_number,
                       float velocity) override;
    void handleNoteOff (MidiKeyboardState* keyboard_state,
                        int midi_channel,
                        int midi_note_number,
                        float velocity) override;

    MidiKeyboardState keyboard_state;
    
    Array<MidiInstrument*> channel_routing_table_[NUM_MIDI_CHANNELS];
    
    const String name_;

};

class MidiOutputPort
{
public:
    MidiOutputPort(const String name, MidiOutput* midi_output);
    ~MidiOutputPort();
    
    const String name() { return name_; }
    
private:
    const String name_;
    ScopedPointer<MidiOutput> midi_output_;
};

class MidiInterface
{
public:
    MidiInterface();
    ~MidiInterface();
    
    MidiInputPort* getMidiInputPort(int port_index);
    MidiInputPort* getMidiInputPort(String name);
    MidiOutputPort* getMidiOutputPort(int port_index);
    MidiOutputPort* getMidiOutputPort(String name);
    
private:
    ScopedPointer<AudioDeviceManager> device_manager_;
    OwnedArray<MidiInputPort> input_ports_;
    OwnedArray<MidiOutputPort> output_ports_;
    
    String name_;

};

#endif /* MidiInterface_hpp */
