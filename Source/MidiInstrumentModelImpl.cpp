//
//  MidiInstrumentModelImpl.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/31/17.
//
//

#include "MidiInstrumentModelImpl.hpp"

const uint8 NUM_REFACE_CS_PARAMS = 18;

const uint8 REFACE_CS_LFO_ASSIGN = 2;
const uint8 REFACE_CS_PORTAMENTO = 5;
const uint8 REFACE_CS_OSC_TYPE = 6;
const uint8 REFACE_CS_EFFECT_TYPE = 16;


YamahaRefaceCSModel::YamahaRefaceCSModel()
: MidiInstrumentModel("Yamaha", "Reface CS")
{
    sysex_manufacturer_id_ = 0x43;
    sysex_device_id_ = 0x00;
    sysex_model_id_bytes_ = 1;
    sysex_model_id_[0] = 0x03;
    sysex_group_number_high_ = 0x7f;
    sysex_group_number_low_ = 0x1c;
    
    addMidiControl(String("Volume"),        // name
                   0,                               // initial value
                   
                   createContinuousControl(7,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x00,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    

    
    addMidiControl(String("LFO Assign"),        // name
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
                                                      0x7F));       // range_max
    
    addMidiControl(String("LFO Depth"),        // name
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
    
    addMidiControl(String("LFO Speed"),        // name
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
    
    addMidiControl(String("Portamento"),        // name
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
    
    addMidiControl(String("Osc Type"),        // name
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
                                                      0x7F));       // range_max
    
    addMidiControl(String("Osc Texture"),        // name
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
    
    addMidiControl(String("Osc Mod"),        // name
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
    
    addMidiControl(String("Filter Cutoff"),        // name
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
    
    addMidiControl(String("Filter Resonance"),        // name
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
    
    addMidiControl(String("EG Balance"),        // name
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
    
    addMidiControl(String("EG Attack"),        // name
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
    
    addMidiControl(String("EG Decay"),        // name
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
    
    addMidiControl(String("EG Sustain"),        // name
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
    
    addMidiControl(String("EG Release"),        // name
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
    
    addMidiControl(String("Effect Type"),        // name
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
                                                      0x7F));       // range_max
    
    addMidiControl(String("Effect Depth"),        // name
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
    
    addMidiControl(String("Effect Rate"),        // name
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
    

    
}

bool YamahaRefaceCSModel::handleMidiSysexEvent(const MidiMessage& message)
{
    printf("MidiInstrumentModel::handleMidiSysexEvent with sysex data size: %d and data size: %d\n", message.getSysExDataSize(), message.getRawDataSize());
    
    int data_size = message.getSysExDataSize();
    const uint8* sysex_data = message.getSysExData();
    
    for (int i=0; i<data_size; i++)
    {
        printf("sysex_data[%d]: %02x\n", i, sysex_data[i]);
    }
    
    if (sysex_data[0] != sysex_manufacturer_id_ ||
        sysex_data[1] != sysex_device_id_ ||
        sysex_data[2] != sysex_group_number_high_ ||
        sysex_data[3] != sysex_group_number_low_ ||
        sysex_data[6] != sysex_model_id_[0])
    {
        return false;
    }
    
    uint8 byte_count_high = sysex_data[4];
    uint8 byte_count_low = sysex_data[5];
    
    uint16 byte_count = byte_count_high;
    byte_count = (byte_count << 2) + byte_count_low;
    
    printf("handleMidiSysexEvent returning true with byte_count: %d\n", byte_count);
    
    uint8 address_high = sysex_data[7];
    uint8 address_mid = sysex_data[8];
    uint8 address_low = sysex_data[9];
    
    printf("evaluating sysex dump at address %02x : %02x : %02x\n", address_high, address_mid, address_low);
    
    if (address_high == 0x30 &&
        address_mid == 0x00 &&
        address_low == 0x00 &&
        byte_count == 26)
    {
        printf("handle patch dump\n");
        
        int param_index = 0;
        int param_byte = 10;
        
        for (int i=0; i<NUM_REFACE_CS_PARAMS+1; i++)
        {
            if (i == 1)
            {
                param_byte++;
                continue;
            }
            
            int value_to_set = sysex_data[param_byte++];
            
            if (i == REFACE_CS_LFO_ASSIGN ||
                i == REFACE_CS_OSC_TYPE ||
                i == REFACE_CS_EFFECT_TYPE)
            {
                switch (value_to_set)
                {
                    case 0:
                        break;
                    case 1:
                        value_to_set = 32;
                        break;
                    case 2:
                        value_to_set = 64;
                        break;
                    case 3:
                        value_to_set = 95;
                        break;
                    case 4:
                        value_to_set = 127;
                        break;
                };
            }
            
            midi_controls_[param_index++]->set_value(value_to_set, true);
        }
    }
    
    return true;
}



YamahaRefaceDXModel::YamahaRefaceDXModel()
: MidiInstrumentModel("Yamaha", "Reface DX")
{
    sysex_manufacturer_id_ = 0x43;
    sysex_device_id_ = 0x00;
    sysex_model_id_bytes_ = 1;
    sysex_model_id_[0] = 0x05;
    sysex_group_number_high_ = 0x7f;
    sysex_group_number_low_ = 0x1c;
    
    addMidiControl(String("Transpose"),        // name
                   25,                               // initial value
                   NULL,                            // no cc control
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x0C,         // address_low
                                      1,            // size (bytes)
                                      0x28,         // range_min
                                      0x58));       // range_max
    
    addMidiControl(String("Part Mode"),        // name
                   0,                               // initial value
                   NULL,                            // no cc control
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x0D,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x02));       // range_max
    
    addMidiControl(String("Portamento"),        // name
                   0,                               // initial value
                   NULL,                            // no cc control
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x0E,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Pitch Bend Range"),        // name
                   12,                               // initial value
                   NULL,                            // no cc control
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x0F,         // address_low
                                      1,            // size (bytes)
                                      0x28,         // range_min
                                      0x58));       // range_max

    addMidiControl(String("Algorithm"),        // name
                   0,                               // initial value
                   
                   createContinuousControl(80,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x10,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x0B));       // range_max
    
    addMidiControl(String("LFO Wave"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x11,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x06));       // range_max

    addMidiControl(String("LFO Speed"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x12,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max

    addMidiControl(String("LFO Delay"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x13,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max

    addMidiControl(String("LFO Pitch Mod"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x14,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max

    addMidiControl(String("Pitch EG Rate 1"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x15,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Pitch EG Rate 2"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x16,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Pitch EG Rate 3"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x17,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Pitch EG Rate 4"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x18,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Pitch EG Level 1"),        // name
                   49,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x19,         // address_low
                                      1,            // size (bytes)
                                      0x10,         // range_min
                                      0x70));       // range_max
    
    addMidiControl(String("Pitch EG Level 2"),        // name
                   49,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x1A,         // address_low
                                      1,            // size (bytes)
                                      0x10,         // range_min
                                      0x70));       // range_max
    
    addMidiControl(String("Pitch EG Level 3"),        // name
                   49,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x1B,         // address_low
                                      1,            // size (bytes)
                                      0x10,         // range_min
                                      0x70));       // range_max
    
    addMidiControl(String("Pitch EG Level 4"),        // name
                   49,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x1C,         // address_low
                                      1,            // size (bytes)
                                      0x10,         // range_min
                                      0x70));       // range_max

    addMidiControl(String("Effect 1 Type"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x1D,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x07));       // range_max
    
    addMidiControl(String("Effect 1 Parameter 1"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x1E,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max

    addMidiControl(String("Effect 1 Parameter 2"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x1F,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    
    addMidiControl(String("Effect 2 Type"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x20,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x07));       // range_max
    
    addMidiControl(String("Effect 2 Parameter 1"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x21,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Effect 2 Parameter 2"),        // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x30,         // address_high
                                      0x00,         // address_mid
                                      0x22,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x00,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 1 EG Rate 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x01,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Rate 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x02,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Rate 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x03,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Rate 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x04,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Level 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x05,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Level 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x06,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Level 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x07,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 EG Level 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x08,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 KSC-R"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x09,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 KSC-Level Left Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x0A,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 KSC-Level Right Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x0B,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 KSC-Level Left Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x0C,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 1 KSC-Level Right Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x0D,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 1 LFO Amp Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x0E,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max

    addMidiControl(String("Op 1 LFO Pitch Mod On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x0F,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 1 Pitch EG On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x10,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    

    addMidiControl(String("Op 1 Level Velocity Sens"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x11,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 Output Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(85,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x12,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 Feedback Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(86,      // cc number
                                           0,       // range_min
                                           127),    // range_max

                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x13,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 1 Feedback Type"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(87,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x14,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 1 Freq Mode"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(88,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x15,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max

    addMidiControl(String("Op 1 Freq Coarse"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(89,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x16,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x1F));       // range_max
    
    addMidiControl(String("Op 1 Freq Fine"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(90,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x17,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x63));       // range_max
    
    addMidiControl(String("Op 1 Freq Detune"),     // name
                   65,                              // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x00,         // address_mid
                                      0x18,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    



    
    
    
    
    
    
    addMidiControl(String("Op 2 On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x00,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 2 EG Rate 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x01,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Rate 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x02,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Rate 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x03,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Rate 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x04,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Level 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x05,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Level 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x06,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Level 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x07,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 EG Level 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x08,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 KSC-R"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x09,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 KSC-Level Left Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x0A,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 KSC-Level Right Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x0B,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 KSC-Level Left Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x0C,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 2 KSC-Level Right Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x0D,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 2 LFO Amp Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x0E,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 LFO Pitch Mod On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x0F,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 2 Pitch EG On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x10,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    
    addMidiControl(String("Op 2 Level Velocity Sens"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x11,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 Output Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(102,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x12,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 Feedback Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(103,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x13,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 2 Feedback Type"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(104,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x14,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 2 Freq Mode"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(105,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x15,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 2 Freq Coarse"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(106,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x16,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x1F));       // range_max
    
    addMidiControl(String("Op 2 Freq Fine"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(107,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x17,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x63));       // range_max
    
    addMidiControl(String("Op 2 Freq Detune"),     // name
                   65,                              // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x01,         // address_mid
                                      0x18,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    

    
    
    
    
    
    
    
    
    
    addMidiControl(String("Op 3 On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x00,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 3 EG Rate 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x01,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Rate 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x02,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Rate 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x03,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Rate 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x04,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Level 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x05,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Level 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x06,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Level 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x07,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 EG Level 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x08,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 KSC-R"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x09,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 KSC-Level Left Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x0A,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 KSC-Level Right Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x0B,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 KSC-Level Left Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x0C,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 3 KSC-Level Right Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x0D,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 3 LFO Amp Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x0E,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 LFO Pitch Mod On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x0F,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 3 Pitch EG On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x10,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    
    addMidiControl(String("Op 3 Level Velocity Sens"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x11,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 Output Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(108,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x12,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 Feedback Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(109,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x13,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 3 Feedback Type"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(110,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x14,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 3 Freq Mode"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(111,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x15,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 3 Freq Coarse"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(112,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x16,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x1F));       // range_max
    
    addMidiControl(String("Op 3 Freq Fine"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(113,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x17,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x63));       // range_max
    
    addMidiControl(String("Op 3 Freq Detune"),     // name
                   65,                              // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x02,         // address_mid
                                      0x18,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    
    

    
    
    
    
    
    addMidiControl(String("Op 4 On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x00,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 4 EG Rate 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x01,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Rate 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x02,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Rate 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x03,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Rate 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x04,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Level 1"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x05,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Level 2"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x06,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Level 3"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x07,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 EG Level 4"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x08,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 KSC-R"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x09,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 KSC-Level Left Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x0A,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 KSC-Level Right Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x0B,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 KSC-Level Left Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x0C,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 4 KSC-Level Right Curve"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x0D,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x03));       // range_max
    
    addMidiControl(String("Op 4 LFO Amp Depth"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x0E,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 LFO Pitch Mod On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x0F,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 4 Pitch EG On/Off"),     // name
                   1,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x10,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    
    addMidiControl(String("Op 4 Level Velocity Sens"),     // name
                   0,                               // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x11,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 Output Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(114,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x12,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 Feedback Level"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(115,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x13,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
    addMidiControl(String("Op 4 Feedback Type"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(116,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x14,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 4 Freq Mode"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(117,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x15,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x01));       // range_max
    
    addMidiControl(String("Op 4 Freq Coarse"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(118,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x16,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x1F));       // range_max
    
    addMidiControl(String("Op 4 Freq Fine"),     // name
                   0,                               // initial value
                   
                   createContinuousControl(119,      // cc number
                                           0,       // range_min
                                           127),    // range_max
                   
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x17,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x63));       // range_max
    
    addMidiControl(String("Op 4 Freq Detune"),     // name
                   65,                              // initial value
                   NULL,
                   createSysexControl(0,            // param table
                                      0x31,         // address_high
                                      0x03,         // address_mid
                                      0x18,         // address_low
                                      1,            // size (bytes)
                                      0x00,         // range_min
                                      0x7F));       // range_max
    
}




bool YamahaRefaceDXModel::handleMidiSysexEvent(const MidiMessage& message)
{
    return true;
}

