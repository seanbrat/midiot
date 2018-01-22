//
//  MidiInstrumentModelImpl.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/31/17.
//
//

#ifndef MidiInstrumentModelImpl_hpp
#define MidiInstrumentModelImpl_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiDefines.hpp"
#include "MidiControl.hpp"
#include "MidiInstrumentModel.hpp"

class YamahaRefaceCSModel : public MidiInstrumentModel
{
public:
    YamahaRefaceCSModel();
    ~YamahaRefaceCSModel() {};
    
    virtual bool handleMidiSysexEvent(const MidiMessage& message);
    
private:
    uint8 sysex_group_number_high_;
    uint8 sysex_group_number_low_;
    
};

class YamahaRefaceDXModel : public MidiInstrumentModel
{
public:
    YamahaRefaceDXModel();
    ~YamahaRefaceDXModel() {};
    
    virtual bool handleMidiSysexEvent(const MidiMessage& message);
    
private:
    uint8 sysex_group_number_high_;
    uint8 sysex_group_number_low_;
    
};



#endif /* MidiInstrumentModelImpl_hpp */
