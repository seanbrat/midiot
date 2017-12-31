//
//  MidiInstrumentModel.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/30/17.
//
//

#include "MidiInstrumentModel.hpp"



bool MidiInstrumentModel::handleMidiControlEvent(const MidiMessage& message)
{
    int controller_number = message.getControllerNumber();
    
    midi_controls_[cc_redirect_table_[controller_number]]->handleMidiControlEvent(message);
    
    return true;
}
