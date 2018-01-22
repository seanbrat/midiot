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

const String MidiInstrumentModel::manufacturer()
{
    return manufacturer_.toString();
}

const String MidiInstrumentModel::model_name()
{
    return model_name_.toString();
}

MidiControl* MidiInstrumentModel::getMidiControl(String control_name)
{
    MidiControl* requested_midi_control = NULL;
    
    MidiControl** ctrl_iter;
    for (ctrl_iter = getMidiControlIterator();
         ctrl_iter != getMidiControlIteratorEnd();
         ctrl_iter++)
    {
        MidiControl* midi_control = *ctrl_iter;
        
        if (control_name == midi_control->name())
        {
            requested_midi_control = midi_control;
            break;
        }
    }
    
    return requested_midi_control;
}

void MidiInstrumentModel::sendMidiControlPatchData()
{
    MidiControl** ctrl_iter;
    
    for (ctrl_iter = getMidiControlIterator();
         ctrl_iter != getMidiControlIteratorEnd();
         ctrl_iter++)
    {
        MidiControl* midi_control = *ctrl_iter;
        midi_control->send_value_to_midi();
    }
}


bool MidiInstrumentModel::updateMidiControl(String control_name,
                                            int control_value,
                                            bool sendMidiOnUpdate)
{
    bool success = true;
    
    MidiControl* midi_control_to_update = getMidiControl(control_name);
    
    if (midi_control_to_update)
    {
        midi_control_to_update->set_value(control_value, true);
    }
    else
    {
        success = false;
    }
    
    return success;
}
