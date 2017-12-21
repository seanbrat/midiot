//
//  MidiInstrumentTabComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 12/4/17.
//
//

#include "MidiInstrumentTabComponent.hpp"


MidiInstrumentTabComponent::MidiInstrumentTabComponent()
: lastInputIndex (0),
isAddingFromMidiInput (false),
keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
startTime (Time::getMillisecondCounterHiRes() * 0.001)
{
    setOpaque (true);
    
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);
    
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    const StringArray midiInputs (MidiInput::getDevices());
    midiInputList.addItemList (midiInputs, 1);
    midiInputList.addListener (this);
    
    // find the first enabled device and use that by default
    for (int i = 0; i < midiInputs.size(); ++i)
    {
        if (deviceManager.isMidiInputEnabled (midiInputs[i]))
        {
            setMidiInput (i);
            break;
        }
    }
    
    // if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);
    
    addAndMakeVisible (midiOutputListLabel);
    midiOutputListLabel.setText ("MIDI Output:", dontSendNotification);
    midiOutputListLabel.attachToComponent (&midiOutputList, true);
    
    addAndMakeVisible (midiOutputList);
    midiOutputList.setTextWhenNoChoicesAvailable ("No MIDI Outputs Enabled");
    const StringArray midiOutputs (MidiOutput::getDevices());
    midiOutputList.addItemList(midiOutputs, 1);
    midiOutputList.addListener(this);
    
    if (midiOutputList.getSelectedId() == 0)
        setMidiOutput (0);
    
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);
    
    /*
     addAndMakeVisible(sysexDataDumpButton);
     sysexDataDumpButton.setButtonText("Dump Systemmm Data");
     
     addAndMakeVisible(sysexDataPatchDumpButton);
     sysexDataPatchDumpButton.setButtonText("Dump Patch Data");
     
     addAndMakeVisible(sysexDataIdButton);
     sysexDataIdButton.setButtonText("Data ID Request");
     */
    addAndMakeVisible(readMidiFileButton);
    readMidiFileButton.setButtonText("Read MIDI File");
    readMidiFileButton.addListener(this);
    
#if USE_STEP_GRID
    addAndMakeVisible(step_grid_component);
#else
    addAndMakeVisible(midi_instrument_controller);
#endif
    
    addAndMakeVisible (midiMessagesBox);
    midiMessagesBox.setMultiLine (true);
    midiMessagesBox.setReturnKeyStartsNewLine (true);
    midiMessagesBox.setReadOnly (true);
    midiMessagesBox.setScrollbarsShown (true);
    midiMessagesBox.setCaretVisible (false);
    midiMessagesBox.setPopupMenuEnabled (true);
    midiMessagesBox.setColour (TextEditor::backgroundColourId, Colour (0x32ffffff));
    midiMessagesBox.setColour (TextEditor::outlineColourId, Colour (0x1c000000));
    midiMessagesBox.setColour (TextEditor::shadowColourId, Colour (0x16000000));
    
    create_test_studio(&midi_instrument_controller);
    
    setSize (1200, 1000);
}

MidiInstrumentTabComponent::~MidiInstrumentTabComponent()
{
    keyboardState.removeListener (this);
    deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
    midiInputList.removeListener (this);
    
    midiOutputList.removeListener(this);
}

void MidiInstrumentTabComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void MidiInstrumentTabComponent::resized()
{
    Rectangle<int> area (getLocalBounds());
    //midiInputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
    midiInputList.setBounds (100, 20, 400, 40);
    //midiOutputList.setBounds(area.removeFromTop(36).removeFromRight (getWidth() - 150).reduced (8));
    midiOutputList.setBounds (100, 70, 400, 40);
    
    readMidiFileButton.setBounds(100, 120, 400, 40);
    
    keyboardComponent.setBounds (20, 170, 1000, 120);
    
#if USE_STEP_GRID
    step_grid_component.setBounds(20, 310, 1000, 440);
#else
    midi_instrument_controller.setBounds(20, 310, 1000, 440);
#endif
    
    midiMessagesBox.setBounds(20, 770, 1000, 150);
    
}

String MidiInstrumentTabComponent::getMidiMessageDescription (const MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on "  + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change " + String (m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel " + String (m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";
    
    if (m.isController())
    {
        String name (MidiMessage::getControllerName (m.getControllerNumber()));
        
        if (name.isEmpty())
            name = "[" + String (m.getControllerNumber()) + "]";
        
        return "Controller " + name + ": " + String (m.getControllerValue());
    }
    
    return String::toHexString (m.getRawData(), m.getRawDataSize());
}

void MidiInstrumentTabComponent::logMessage (const String& m)
{
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret (m + newLine);
}

/** Starts listening to a MIDI input device, enabling it if necessary. */
void MidiInstrumentTabComponent::setMidiInput (int index)
{
    const StringArray list (MidiInput::getDevices());
    
    deviceManager.removeMidiInputCallback (list[lastInputIndex], this);
    
    const String newInput (list[index]);
    
    if (! deviceManager.isMidiInputEnabled (newInput))
        deviceManager.setMidiInputEnabled (newInput, true);
    
    deviceManager.addMidiInputCallback (newInput, this);
    midiInputList.setSelectedId (index + 1, dontSendNotification);
    
    lastInputIndex = index;
}

/** Starts listening to a MIDI output device, enabling it if necessary. */
void MidiInstrumentTabComponent::setMidiOutput (int index)
{
    const StringArray list (MidiOutput::getDevices());
    const String newOutput (list[index]);
    
    midiOutputList.setSelectedId (index + 1, dontSendNotification);
    lastOutputIndex = index;
    
    midiOutputDevice = MidiOutput::openDevice(index);
}

void MidiInstrumentTabComponent::buttonClicked (Button* button)
{
    if (button == &readMidiFileButton)
    {
        //postMessageToList()
        logMessage(String("Reading MIDI File..."));
        File midiFile = File::createFileWithoutCheckingPath (String("/Users/seanb/Development/JUCE/Midiot/Resources/basic808.mid"));
        FileInputStream midiStream (midiFile);
        inputMidiFile.readFrom (midiStream);
        
        String log_message_string;
        
        int num_tracks = inputMidiFile.getNumTracks();
        log_message_string = String::formatted ("num_tracks: %d", num_tracks);
        logMessage(log_message_string);
        logMessage(String(""));
        
        const MidiMessageSequence* trackMidiMessageSeq(inputMidiFile.getTrack(0));
        int num_events = trackMidiMessageSeq->getNumEvents();
        log_message_string = String::formatted ("num_events: %d", num_events);
        logMessage(log_message_string);
        logMessage(String(""));
        
        for (int i=0; i<num_events; i++)
        {
            MidiMessageSequence::MidiEventHolder* trackMidiEventPtr(trackMidiMessageSeq->getEventPointer(i));
            MidiMessage midi_message(trackMidiEventPtr->message);
            int event_time(trackMidiMessageSeq->getEventTime(i));
            String source_string("[" + String(event_time) + "] Input MIDI File");
            postMessageToList(midi_message, source_string);
        }
        
    }
}

void MidiInstrumentTabComponent::comboBoxChanged (ComboBox* box)
{
    if (box == &midiInputList)
        setMidiInput (midiInputList.getSelectedItemIndex());
    
    if (box == &midiOutputList)
        setMidiOutput (midiOutputList.getSelectedItemIndex());
}

// These methods handle callbacks from the midi device + on-screen keyboard..
void MidiInstrumentTabComponent::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message)
{
    const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent (message);
    postMessageToList (message, source->getName());
}

void MidiInstrumentTabComponent::handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    if (! isAddingFromMidiInput)
    {
        MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
        m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList (m, "On-Screen Keyboard");
        midiOutputDevice->sendMessageNow(m);
        
        const uint8 sysexDataIdRequest[6] = { 0xF0, 0x7E, 0x00, 0x06, 0x01, 0xF7 };
        MidiMessage idRequest (MidiMessage::createSysExMessage ((const void*)sysexDataIdRequest, 6));
        //midiOutputDevice->sendMessageNow(idRequest);
        
        const uint8 sysexDataDumpRequest[10] = { 0xF0, 0x43, 0x20, 0x7F, 0x1C, 0x03, 0x00, 0x00, 0x00, 0xF7 };
        MidiMessage dumpRequest (MidiMessage::createSysExMessage ((const void*)sysexDataDumpRequest, 10));
        //midiOutputDevice->sendMessageNow(dumpRequest);
        
        //const uint8 sysexDataPatchDump[10] = { 0xF0, 0x43, 0x00, 0x7F, 0x1C, 0x00, 0x1A, 0x03, 0x30, 0x00, 0x00, 0xF7 };
        const uint8 sysexDataPatchDump[10] = { 0xF0, 0x43, 0x20, 0x7F, 0x1C, 0x03, 0x0E, 0x0F, 0x00, 0xF7 };
        MidiMessage PatchDumpRequest (MidiMessage::createSysExMessage ((const void*)sysexDataPatchDump, 10));
        //midiOutputDevice->sendMessageNow(PatchDumpRequest);
    }
    /*
     else
     {
     MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
     m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
     postMessageToList (m, "External Keyboard");
     
     }
     */
}


void MidiInstrumentTabComponent::handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (! isAddingFromMidiInput)
    {
        MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber));
        m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList (m, "On-Screen Keyboard");
        
        midiOutputDevice->sendMessageNow(m);
    }
}

void MidiInstrumentTabComponent::postMessageToList (const MidiMessage& message, const String& source)
{
    (new MidiInstrumentTabComponent::IncomingMessageCallback (this, message, source))->post();
}

void MidiInstrumentTabComponent::addMessageToList (const MidiMessage& message, const String& source)
{
    const double time = message.getTimeStamp() - startTime;
    
    const int hours = ((int) (time / 3600.0)) % 24;
    const int minutes = ((int) (time / 60.0)) % 60;
    const int seconds = ((int) time) % 60;
    const int millis = ((int) (time * 1000.0)) % 1000;
    
    const String timecode (String::formatted ("%02d:%02d:%02d.%03d",
                                              hours,
                                              minutes,
                                              seconds,
                                              millis));
    
    const String description (getMidiMessageDescription (message));
    
    if (description != "f8")
    {
        //        const String midiMessageString (timecode + "  -  " + description + " (" + source + ")"); // [7]
        const String midiByteString(String::toHexString (message.getRawData(), message.getRawDataSize()));
        const String midiMessageString (description + " " + source + ":   " + midiByteString); // [7]
        logMessage (midiMessageString);
    }
}


