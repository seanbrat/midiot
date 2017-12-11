//
//  MidiStudio.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/7/17.
//
//

#ifndef MidiStudio_hpp
#define MidiStudio_hpp

#include "../JuceLibraryCode/JuceHeader.h"

#include "MidiInstrument.hpp"
#include "MidiInterface.hpp"
#include "MidiDefines.hpp"

#include <stdio.h>

class MidiInstrumentControllerComponent;

class MidiStudio
{
public:
    MidiStudio();
    ~MidiStudio();
    
    int addMidiInstrument(MidiInstrument* midi_instrument);
    
    MidiInterface* getMidiInterface() { return midi_interface_; };
    
private:
    OwnedArray<MidiInstrument> midi_instruments_;
    ScopedPointer<MidiInterface> midi_interface_;
};

MidiStudio* create_test_studio(MidiInstrumentControllerComponent* controller);

#endif /* MidiStudio_hpp */
