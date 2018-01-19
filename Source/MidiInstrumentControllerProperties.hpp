//
//  MidiInstrumentControllerProperties.hpp
//  Midiot
//
//  Created by Sean Bratnober on 1/17/18.
//
//

#ifndef MidiInstrumentControllerProperties_hpp
#define MidiInstrumentControllerProperties_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class MidiInstrumentControllerProperties
{
public:
    MidiInstrumentControllerProperties();
    ~MidiInstrumentControllerProperties();
    
    String patch_name() { return patch_name_; }
    void set_patch_name(String patch_name) { patch_name_ = patch_name; }
    
private:
    String patch_name_;
    
};

#endif /* MidiInstrumentControllerProperties_hpp */
