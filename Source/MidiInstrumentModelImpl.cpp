//
//  MidiInstrumentModelImpl.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/31/17.
//
//

#include "MidiInstrumentModelImpl.hpp"

const uint8 NUM_REFACE_CS_PARAMS = 18;

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
                                                      0x127));       // range_max
    
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
        
        for (int i=0; i<NUM_REFACE_CS_PARAMS; i++)
        {
            if (i == 1)
            {
                param_byte++;
                continue;
            }
            
            midi_controls_[param_index++]->set_value(sysex_data[param_byte++], true);
        }
    }
    
    return true;
}
