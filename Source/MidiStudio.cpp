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

MidiStudio::MidiStudio()
{
    midi_interface_ = new MidiInterface();
}

MidiStudio::~MidiStudio()
{}

MidiInstrumentModel* createYamahaRefaceCS()
{
    MidiInstrumentModel* yamaha_cs_inst = new MidiInstrumentModel(Identifier("Yamaha Reface CS"));
    
    yamaha_cs_inst->addMidiControl(Identifier("LFO Assign"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(78,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x02,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x04));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("LFO Depth"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(77,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x03,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("LFO Speed"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(76,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x04,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Portamento"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(20,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x05,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Osc Type"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(80,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x06,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x04));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Osc Texture"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(81,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x07,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Osc Mod"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(82,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x08,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Filter Cutoff"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(74,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x09,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Filter Resonance"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(71,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x0A,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("EG Balance"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(83,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x0B,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("EG Attack"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(73,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x0C,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("EG Decay"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(75,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x0D,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("EG Sustain"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(79,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x0E,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("EG Release"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(72,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x0F,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Effect Type"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(17,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x10,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x04));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Effect Depth"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(18,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x11,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    yamaha_cs_inst->addMidiControl(Identifier("Effect Rate"),        // name
                                   0,                               // initial value
                                   
                                   createContinuousControl(19,      // cc number
                                                           0,       // range_min
                                                           127),    // range_max
                                   
                                   createSysexControl(0,            // param table
                                                      0x30,         // address_high
                                                      0x00,         // address_mid
                                                      0x12,         // address_low
                                                      1,            // size (bytes)
                                                      0x00,         // range_min
                                                      0x7F));       // range_max
    
    
    return yamaha_cs_inst;
}

MidiStudio* create_test_studio(MidiInstrumentControllerComponent* controller)
{
    MidiStudio* midi_studio = new MidiStudio();
    MidiInterface* midi_interface = midi_studio->getMidiInterface();
    MidiInstrument* yamaha_cs_inst = new MidiInstrument(
                                        createYamahaRefaceCS(),
                                        controller,
                                        midi_interface->getMidiInputPort("reface CS"),
                                        midi_interface->getMidiOutputPort("reface CS"));

    return midi_studio;
}

int MidiStudio::addMidiInstrument(MidiInstrument* midi_instrument)
{
    int instrument_id = midi_instruments_.size();
    
    midi_instrument->set_instrument_id(instrument_id);
    
    midi_instruments_.add(midi_instrument);
    
    return instrument_id;
    
}
