//
//  MidiStudio.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/7/17.
//
//

#include "MidiStudio.hpp"
#include "MidiInstrument.hpp"
#include "MidiInstrumentControllerComponent.hpp"
#include "MidiInstrumentModelImpl.hpp"

#define USE_REFACE_CS   0

MidiStudio::MidiStudio()
{
    midi_interface_ = new MidiInterface();
}

MidiStudio::~MidiStudio()
{}

MidiStudio* create_test_studio(MidiInstrumentControllerComponent* controller)
{
    MidiStudio* midi_studio = new MidiStudio();
    MidiInterface* midi_interface = midi_studio->getMidiInterface();
#if USE_REFACE_CS
    YamahaRefaceCSModel* yamaha_cs_model = new YamahaRefaceCSModel();
    MidiInstrument* yamaha_cs_inst = new MidiInstrument(
                                        yamaha_cs_model,
                                        controller,
                                        midi_interface->getMidiInputPort("reface CS"),
                                        midi_interface->getMidiOutputPort("reface CS"));
#else
    YamahaRefaceDXModel* yamaha_dx_model = new YamahaRefaceDXModel();
    MidiInstrument* yamaha_dx_inst = new MidiInstrument(
                                                        yamaha_dx_model,
                                                        controller,
                                                        midi_interface->getMidiInputPort("reface DX"),
                                                        midi_interface->getMidiOutputPort("reface DX"),
                                                        15,
                                                        15);

#endif
    //yamaha_cs_inst->sendSysexPatchDumpMessage();
    
    return midi_studio;
}

int MidiStudio::addMidiInstrument(MidiInstrument* midi_instrument)
{
    int instrument_id = midi_instruments_.size();
    
    midi_instrument->set_instrument_id(instrument_id);
    
    midi_instruments_.add(midi_instrument);
    
    return instrument_id;
    
}
