//
//  MidiInstrument.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#include "MidiInstrument.hpp"
#include "MidiInterface.hpp"
#include "MidiInstrumentControllerComponent.hpp"
#include "MidiControl.hpp"



void MidiInstrumentBaseModel::handleMidiControlEvent(const MidiMessage& message)
{
    int controller_number = message.getControllerNumber();

    midi_controls_[cc_redirect_table_[controller_number]]->handleMidiControlEvent(message);
}

void MidiInstrumentBaseModel::handleMidiSysexEvent(const MidiMessage& message)
{
    printf("MidiInstrumentBaseModel::handleMidiSysexEvent with sysex data size: %d and data size: %d\n", message.getSysExDataSize(), message.getRawDataSize());
    
    int data_size = message.getSysExDataSize();
    const uint8* sysex_data = message.getSysExData();
    
    for (int i=0; i<data_size; i++)
    {
        printf("sysex_data[%d]: %02x\n", i, sysex_data[i]);
    }
    

    
    
}


MidiInstrument::MidiInstrument(MidiInstrumentBaseModel* inst_model,
                               MidiInstrumentControllerComponent* controller,
                               MidiInputPort* input_port,
                               MidiOutputPort* output_port)
: inst_model_(inst_model),
channel_(0),
midi_input_port_(input_port),
midi_output_port_(output_port),
controller_component_(controller)
{
    // Hook up keyboard component to midi_input_port_
    controller_component_->addMidiKeyboardStateListener(this);
    
    MidiControl** ctrl_iter = inst_model->getMidiControlIterator();
    
    for (ctrl_iter;
         ctrl_iter != inst_model->getMidiControlIteratorEnd();
         ctrl_iter++)
    {
        MidiControl* midi_control = *ctrl_iter;
        printf("adding MidiControl: %s\n", midi_control->name().toRawUTF8());
        setupMidiControlInterface(midi_control);
    }

    midi_input_port_->addInstrumentToPort(this);
}

MidiInstrument::~MidiInstrument()
{
    if (controller_component_)
    {
        controller_component_->addMidiKeyboardStateListener(this);
    }
}

void MidiInstrument::setupMidiControlInterface(MidiControl* midi_control)
{
    MidiControlSlider* control_slider = controller_component_->addMidiControlSlider(midi_control);
    controller_component_->addMidiInstrument(this);
    midi_control->setMidiInstrument(this);
    midi_control->setMidiControlSlider(control_slider);
    midi_input_port_->addInstrumentToPort(this);
    
}

void MidiInstrument::listenToControllerComponentKeyboard()
{
    
}

void MidiInstrument::set_instrument_id(int instrument_id)
{
    instrument_id_ = instrument_id;
}


void MidiInstrument::sendControllerEvent(int midi_channel,
                                         int controller_type,
                                         int value)
{
    MidiMessage m(MidiMessage::controllerEvent(midi_channel, controller_type, value));
    m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.0001);
    midi_output_port_->sendMessageNow(m);
}

void MidiInstrument::sendNoteOn(int midi_channel,
                                int midi_note_number,
                                float velocity)
{
    MidiMessage m (MidiMessage::noteOn (midi_channel, midi_note_number, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    midi_output_port_->sendMessageNow(m);
}

void MidiInstrument::sendNoteOff(int midi_channel,
                                 int midi_note_number,
                                 float velocity)
{
    MidiMessage m (MidiMessage::noteOff (midi_channel, midi_note_number, velocity));
    m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
    midi_output_port_->sendMessageNow(m);
}



void MidiInstrument::handleNoteOn (MidiKeyboardState* keyboard_state,
                   int midi_channel,
                   int midi_note_number,
                   float velocity)
{
    //printf("MidiInstrument::handleNoteOn() with midi_channel: %d note: %d velocity: %f\n", midi_channel, midi_note_number, velocity);
    midi_output_port_->sendNoteOn(midi_channel, midi_note_number, velocity);
}

void MidiInstrument::handleNoteOff (MidiKeyboardState* keyboard_state,
                    int midi_channel,
                    int midi_note_number,
                    float velocity)
{
    //printf("MidiInstrument::handleNoteOff()\n");
    midi_output_port_->sendNoteOff(midi_channel, midi_note_number, velocity);

}

void MidiInstrument::handleIncomingMidiMessage(const MidiMessage& message)
{

    if (message.isNoteOn() || message.isNoteOff())
    {
        //printf("MidiInstrument::handleIncomingMidiMessage() with note\n");
        controller_component_->processNextKeyboardMidiEvent(message);
    }
    else if (message.isController())
    {
        //printf("MidiInstrument::handleIncomingMidiMessage() with controller\n");
        inst_model_->handleMidiControlEvent(message);
    }
    else if (message.isSysEx())
    {
        //printf("MidiInstrument::handleIncomingMidiMessage() with sysex\n");
        inst_model_->handleMidiSysexEvent(message);
}
    else if (!message.isMidiClock())
    {
        //printf("MidiInstrument::handleIncomingMidiMessage()\n");
    }
    
}

void MidiInstrument::sendSysexPatchDumpMessage()
{
    //const uint8 sysex_data_patch_dump[10] = { 0xF0, 0x43, 0x20, 0x7F, 0x1C, 0x03, 0x0E, 0x0F, 0x00, 0xF7 };
    
    const uint8 sysexDataDumpRequest[10] = { 0xF0, 0x43, 0x20, 0x7F, 0x1C, 0x03, 0x00, 0x00, 0x00, 0xF7 };
    MidiMessage dumpRequest(MidiMessage::createSysExMessage ((const void*)sysexDataDumpRequest, 10));
    printf("sending data dump request\n");
    midi_output_port_->sendMessageNow(dumpRequest);

    const uint8 sysexDataPatchDump[10] = { 0xF0, 0x43, 0x20, 0x7F, 0x1C, 0x03, 0x0E, 0x0F, 0x00, 0xF7 };
    MidiMessage PatchDumpRequest (MidiMessage::createSysExMessage ((const void*)sysexDataPatchDump, 10));
    printf("sending patch dump request\n");
    midi_output_port_->sendMessageNow(PatchDumpRequest);

    
    const uint8 sysexDataIdRequest[6] = { 0xF0, 0x7E, 0x00, 0x06, 0x01, 0xF7 };
    MidiMessage idRequest (MidiMessage::createSysExMessage ((const void*)sysexDataIdRequest, 6));
    printf("sending data ID request\n");
    midi_output_port_->sendMessageNow(idRequest);
    
    int midi_channel = 1;
    int midi_note_number = 60;
    float velocity = 0.99;
    MidiMessage m(MidiMessage::noteOn(midi_channel, midi_note_number, velocity));
//    midi_output_port_->sendMessageNow(m);
}
