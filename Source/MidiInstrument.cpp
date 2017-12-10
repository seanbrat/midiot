//
//  MidiInstrumentModel.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#include "MidiInstrument.hpp"


MidiInstrumentModel::ContinuousControl* createContinuousControl(short number,
                                                                short range_min,
                                                                short range_max)
{
    return new MidiInstrumentModel::ContinuousControl(number,
                                                      range_min,
                                                      range_max);
}

MidiInstrumentModel::SysexControl* createSysexControl(short param_table,
                                                      short address_high,
                                                      short address_mid,
                                                      short address_low,
                                                      short size_bytes,
                                                      int range_min,
                                                      int range_max)
{
    return new MidiInstrumentModel::SysexControl(param_table,
                                                 address_high,
                                                 address_mid,
                                                 address_low,
                                                 size_bytes,
                                                 range_min,
                                                 range_max);
}

MidiInstrument::MidiInstrument(MidiInstrumentModel* inst_model)
: inst_model_(inst_model),
channel_(0)
{}

MidiInstrument::~MidiInstrument()
{}

void MidiInstrument::set_instrument_id(int instrument_id)
{
    instrument_id_ = instrument_id;
}
