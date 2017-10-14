#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoUtilities.h"

#include <vector>


#define USE_STEP_GRID       0

using std::vector;
using std::pair;

//==============================================================================
class GraphicsDemoBase  : public Component
{
public:
    GraphicsDemoBase (const String& name)
    : Component (name)
    {
        displayFont = Font (Font::getDefaultMonospacedFontName(), 12.0f, Font::bold);
    }
    
    AffineTransform getTransform()
    {
        const float hw = 0.5f * getWidth();
        const float hh = 0.5f * getHeight();
        
        AffineTransform t;
        
        //if (controls.animateRotation.getToggleState())
        //    t = t.rotated (rotation.getValue() * float_Pi * 2.0f);
        
        //if (controls.animateSize.getToggleState())
        //    t = t.scaled (0.3f + size.getValue() * 2.0f);
        
        //if (controls.animatePosition.getToggleState())
        //    t = t.translated (hw + hw * (offsetX.getValue() - 0.5f),
        //                      hh + hh * (offsetY.getValue() - 0.5f));
        //else
            t = t.translated (hw, hh);
        
        //if (controls.animateShear.getToggleState())
        //    t = t.sheared (shear.getValue() * 2.0f - 1.0f, 0.0f);
        
        return t;
    }
    
    float getAlpha() const
    {
        //if (controls.animateAlpha.getToggleState())
            return alpha.getValue();
        
        return 1.0f;
    }
    
    
    
    void paint (Graphics& g) override
    {
        double startTime = 0.0;
        
        {
            // A ScopedSaveState will return the Graphics context to the state it was at the time of
            // construction when it goes out of scope. We use it here to avoid clipping the fps text
            const Graphics::ScopedSaveState state (g);

            //if (controls.clipToRectangle.getToggleState())
            //    clipToRectangle (g);
            //if (controls.clipToPath     .getToggleState())
            //    clipToPath (g);
            //if (controls.clipToImage    .getToggleState())
            //    clipToImage (g);
                        
            //g.setImageResamplingQuality (controls.quality.getToggleState() ? Graphics::highResamplingQuality
            //                                         : Graphics::mediumResamplingQuality);
            //g.setImageResamplingQuality (Graphics::highResamplingQuality);
            
            // take a note of the time before the render
            //startTime = Time::getMillisecondCounterHiRes();
                        
            // then let the demo draw itself..
            drawDemo (g);
        }
    }
    
    virtual void drawDemo (Graphics&) = 0;
    
    void clipToRectangle (Graphics& g)
    {
        int w = getWidth() / 2;
        int h = getHeight() / 2;
        
        int x = (int) (w * clipRectX.getValue());
        int y = (int) (h * clipRectY.getValue());
        
        g.reduceClipRegion (x, y, w, h);
    }
    
    void clipToPath (Graphics& g)
    {
        float pathSize = (float) jmin (getWidth(), getHeight());
        
        Path p;
        p.addStar (Point<float> (clipPathX.getValue(),
                                 clipPathY.getValue()) * pathSize,
                   7,
                   pathSize * (0.5f + clipPathDepth.getValue()),
                   pathSize * 0.5f,
                   clipPathAngle.getValue());
        
        g.reduceClipRegion (p, AffineTransform());
    }
    
    void clipToImage (Graphics& g)
    {
        if (! clipImage.isValid())
            createClipImage();
        
        AffineTransform transform (AffineTransform::translation (clipImage.getWidth() / -2.0f,
                                                                 clipImage.getHeight() / -2.0f)
                                   .rotated (clipImageAngle.getValue() * float_Pi * 2.0f)
                                   .scaled (2.0f + clipImageSize.getValue() * 3.0f)
                                   .translated (getWidth() * 0.5f,
                                                getHeight() * 0.5f));
        
        g.reduceClipRegion (clipImage, transform);
    }
    
    void createClipImage()
    {
        clipImage = Image (Image::ARGB, 300, 300, true);
        
        Graphics g (clipImage);
        
        g.setGradientFill (ColourGradient (Colours::transparentBlack, 0, 0,
                                           Colours::black, 0, 300, false));
        
        for (int i = 0; i < 20; ++i)
            g.fillRect (Random::getSystemRandom().nextInt (200),
                        Random::getSystemRandom().nextInt (200),
                        Random::getSystemRandom().nextInt (100),
                        Random::getSystemRandom().nextInt (100));
    }
    
    //==============================================================================
    
    SlowerBouncingNumber offsetX, offsetY, rotation, size, shear, alpha, clipRectX,
    clipRectY, clipPathX, clipPathY, clipPathDepth, clipPathAngle,
    clipImageX, clipImageY, clipImageAngle, clipImageSize;
    
    double lastRenderStartTime, averageTimeMs, averageActualFPS;
    Image clipImage;
    Font displayFont;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicsDemoBase)
};


//==============================================================================
class StepGridComponent  : public GraphicsDemoBase
{
public:
    enum GridResolution {
        
        
    };
    
    StepGridComponent ()
    : GraphicsDemoBase ("Fill Types: Rectangles"),
    colour1 (Colours::red),
    colour2 (Colours::green),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0),
    mouse_xpos(0),
    mouse_ypos(0),
    mouse_click_xpos(0),
    mouse_click_ypos(0)
    {
        for (int row = 0; row < num_rows; row++)
        {
            for (int step = 0; step < num_steps; step++)
            {
                TextButton *grid_button = new TextButton();
                grid_buttons.add(grid_button);
                grid_button->setClickingTogglesState(true);
                grid_button->setColour(TextButton::ColourIds::buttonOnColourId, Colours::firebrick);
                
                if (step % 4)
                {
                    grid_button->setColour(TextButton::ColourIds::buttonColourId, Colours::darkslategrey);
                }
                
                addAndMakeVisible(*grid_button);
            }
        }
    }
    

    void mouseMove(const MouseEvent& e) override
    {
        mouse_xpos = e.x;
        mouse_ypos = e.y;
        
        repaint();
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        mouse_xpos = e.x;
        mouse_ypos = e.y;
        mouse_click_xpos = e.x;
        mouse_click_ypos = e.y;
        
        repaint();
    }
    
    bool mouseGridStepPosition(int &x, int &y)
    {
        return true;
    }
    
    void drawDemo (Graphics& g) override
    {
        int fill_x = getWidth() / 2;
        int fill_y = getHeight() / 2;
        
        g.addTransform (getTransform());
        
        g.setColour (Colours::darkgrey);
        g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
        
        g.setColour (Colours::black);
        
        int grid_width = getWidth();
        int grid_height = getHeight();

        step_width = 48;
        step_height = 48;
        
        int grid_step_x;
        int grid_step_y = -(getHeight() / 2) + grid_thickness;
        int grid_pos_x;
        int grid_pos_y = grid_thickness;

        int grid_button_index = 0;
        
        for (int row = 0; row < num_rows; row++)
        {
            grid_step_x = -(getWidth() / 2) + grid_thickness;
            grid_pos_x = grid_thickness;
            
            for (int step = 0; step < num_steps; step++)
            {
                //g.fillRect((float)grid_step_x, (float)grid_step_y, step_width, step_height);
                
                //TextButton *grid_button = new TextButton();
                //grid_buttons.add(grid_button);
                //addAndMakeVisible(*grid_button);
                grid_buttons[grid_button_index++]->setBounds(grid_pos_x, grid_pos_y, step_width, step_height);
                
                grid_step_x += grid_thickness + step_width;
                grid_pos_x += grid_thickness + step_width;
            }
            
            grid_step_y += grid_thickness + step_height;
            grid_pos_y += grid_thickness + step_height;
        }
        
        /*

        const int rectSize = 100;//jmin (getWidth(), getHeight()) / 2 - 20;
                g.setColour (colour1);//.withAlpha (getAlpha()));
        g.fillRect (-rectSize, -rectSize, rectSize, rectSize);
        
        g.setGradientFill (ColourGradient (colour1, 10.0f, (float) -rectSize,
                                           colour2, 10.0f + rectSize, 0.0f, false));
        //g.setOpacity (getAlpha());
        g.fillRect (10, -rectSize, rectSize, rectSize);
        
        g.setGradientFill (ColourGradient (colour1, rectSize * -0.5f, 10.0f + rectSize * 0.5f,
                                           colour2, 0, 10.0f + rectSize, true));
        //g.setOpacity (getAlpha());
        g.fillRect (-rectSize, 10, rectSize, rectSize);
        
        g.setGradientFill (ColourGradient (colour1, 10.0f, 10.0f,
                                           colour2, 10.0f + rectSize, 10.0f + rectSize, false));
        */
        //g.setOpacity (getAlpha());

        int mouse_grid_x = -1;
        int mouse_grid_y = -1;
        
        String selected_grid_step = "none";
        
        if (mouse_grid_x != -1 && mouse_grid_y != -1)
        {
            selected_grid_step = String(mouse_grid_x) + ", " + String(mouse_grid_y);
        }
        
        g.setColour (Colours::white);
        GlyphArrangement ga;
        //g.fillRect (ga.getBoundingBox (0, ga.getNumGlyphs(), true).getSmallestIntegerContainer().expanded (4));
        
        ga.addFittedText (displayFont,
                          "Mouse Pos X:\t\t\t" + String(mouse_xpos)
                          + "\nMouse Pos Y:\t\t\t" + String(mouse_ypos)
                          + "\nMouse Click X:\t" + String (mouse_click_xpos)
                          + "\nMouse Click Y:\t" + String (mouse_click_ypos)
                          + "\nStep Width: " + String(step_width)
                          + " Step Height: " + String(step_height)
                          + "\nSelected Grid Step: " + selected_grid_step,
                          8-fill_x, fill_y-73, 400.0, 400.0, Justification::topLeft, 3);
        
        ga.draw (g);

    }
    
    int num_steps;
    int num_rows;
    float grid_thickness;
    
    float step_width;
    float step_height;
    
    Colour colour1, colour2;
    int mouse_xpos;
    int mouse_ypos;
    int mouse_click_xpos;
    int mouse_click_ypos;
  
    // row major format
    vector<vector<int>> grid_values;
    
    OwnedArray<TextButton> grid_buttons;

};



//==============================================================================
class NoteGridComponent  : public GraphicsDemoBase
{
public:
    NoteGridComponent ()
    : GraphicsDemoBase ("Fill Types: Rectangles"),
    colour1 (Colours::red),
    colour2 (Colours::green),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0),
    mouse_xpos(0),
    mouse_ypos(0),
    mouse_click_xpos(0),
    mouse_click_ypos(0)
    {

    }
    
    
    void mouseMove(const MouseEvent& e) override
    {
        mouse_xpos = e.x;
        mouse_ypos = e.y;
        
        repaint();
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        mouse_xpos = e.x;
        mouse_ypos = e.y;
        mouse_click_xpos = e.x;
        mouse_click_ypos = e.y;
        
        repaint();
    }
    
    bool mouseGridStepPosition(int &x, int &y)
    {
        return true;
    }
    
    void drawDemo (Graphics& g) override
    {
        int fill_x = getWidth() / 2;
        int fill_y = getHeight() / 2;
        
        g.addTransform (getTransform());
        
        g.setColour (Colours::darkgrey);
        g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
        
        g.setColour (Colours::black);
        
        
        
        int grid_width = getWidth();
        int grid_height = getHeight();
        
        step_width = 48;
        step_height = 48;
        
        int grid_step_x;
        int grid_step_y = -(getHeight() / 2) + grid_thickness;
        int grid_pos_x;
        int grid_pos_y = grid_thickness;
        
        int grid_button_index = 0;
        
        
        
        
        int mouse_grid_x = -1;
        int mouse_grid_y = -1;
        
        String selected_grid_step = "none";
        
        if (mouse_grid_x != -1 && mouse_grid_y != -1)
        {
            selected_grid_step = String(mouse_grid_x) + ", " + String(mouse_grid_y);
        }
        
        g.setColour (Colours::white);
        GlyphArrangement ga;
        //g.fillRect (ga.getBoundingBox (0, ga.getNumGlyphs(), true).getSmallestIntegerContainer().expanded (4));
        
        ga.addFittedText (displayFont,
                          "Mouse Pos X:\t\t\t" + String(mouse_xpos)
                          + "\nMouse Pos Y:\t\t\t" + String(mouse_ypos)
                          + "\nMouse Click X:\t" + String (mouse_click_xpos)
                          + "\nMouse Click Y:\t" + String (mouse_click_ypos)
                          + "\nStep Width: " + String(step_width)
                          + " Step Height: " + String(step_height)
                          + "\nSelected Grid Step: " + selected_grid_step,
                          8-fill_x, fill_y-73, 400.0, 400.0, Justification::topLeft, 3);
        
        ga.draw (g);
        
    }
    
    int num_steps;
    int num_rows;
    float grid_thickness;
    
    float step_width;
    float step_height;
    
    Colour colour1, colour2;
    int mouse_xpos;
    int mouse_ypos;
    int mouse_click_xpos;
    int mouse_click_ypos;
    
    // row major format
    vector<vector<int>> grid_values;
    
    OwnedArray<TextButton> grid_buttons;
    
};




class MainContentComponent  : public Component,
                              private Button::Listener,
                              private ComboBox::Listener,
                              private MidiInputCallback,
                              private MidiKeyboardStateListener
{
public:
    MainContentComponent()
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
        addAndMakeVisible(StepGridComponent);
#else
        addAndMakeVisible(NoteGridComponent);
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
        
        setSize (1200, 1000);
    }

    ~MainContentComponent()
    {
        keyboardState.removeListener (this);
        deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
        midiInputList.removeListener (this);
        
        midiOutputList.removeListener(this);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
    }

    void resized() override
    {
        Rectangle<int> area (getLocalBounds());
        //midiInputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
        midiInputList.setBounds (100, 20, 400, 40);
        //midiOutputList.setBounds(area.removeFromTop(36).removeFromRight (getWidth() - 150).reduced (8));
        midiOutputList.setBounds (100, 70, 400, 40);

/*
        //sysexDataDumpButton.setBounds(area.removeFromTop(36).removeFromRight (getWidth() - 30));
        sysexDataDumpButton.setBounds(100, 120, 400, 40);
        //sysexDataIdButton.setBounds(area.removeFromTop(36).removeFromRight (getWidth() - 30));
        sysexDataIdButton.setBounds(100, 170, 400, 40);
        //sysexDataPatchDumpButton.setBounds(area.removeFromTop(36).removeFromRight (getWidth() - 30));
        sysexDataPatchDumpButton.setBounds(100, 220, 400, 40);
*/
/*
        keyboardComponent.setBounds (20, 320, 1000, 120);
        StepGridComponent.setBounds(20, 460, 1000, 320);
        midiMessagesBox.setBounds(20, 800, 1000, 200);
*/
        
        readMidiFileButton.setBounds(100, 120, 400, 40);
        
        keyboardComponent.setBounds (20, 170, 1000, 120);

#if USE_STEP_GRID
        StepGridComponent.setBounds(20, 310, 1000, 400);
#else
        NoteGridComponent.setBounds(20, 310, 1000, 400);
#endif
        
        midiMessagesBox.setBounds(20, 730, 1000, 200);
        
    }

private:
    static String getMidiMessageDescription (const MidiMessage& m)
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

    void logMessage (const String& m)
    {
        midiMessagesBox.moveCaretToEnd();
        midiMessagesBox.insertTextAtCaret (m + newLine);
    }

    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput (int index)
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
    void setMidiOutput (int index)
    {
        const StringArray list (MidiOutput::getDevices());
        const String newOutput (list[index]);
        
        midiOutputList.setSelectedId (index + 1, dontSendNotification);
        lastOutputIndex = index;
        
        midiOutputDevice = MidiOutput::openDevice(index);
    }
    
    void buttonClicked (Button* button) override
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
            
            ScopedPointer<const MidiMessageSequence> trackMidiMessageSeq(inputMidiFile.getTrack(0));
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
    
    void comboBoxChanged (ComboBox* box) override
    {
        if (box == &midiInputList)
            setMidiInput (midiInputList.getSelectedItemIndex());

        if (box == &midiOutputList)
            setMidiOutput (midiOutputList.getSelectedItemIndex());
    }

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
        keyboardState.processNextMidiEvent (message);
        postMessageToList (message, source->getName());
    }

    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
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

    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override
    {
        if (! isAddingFromMidiInput)
        {
            MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber));
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keyboard");

            midiOutputDevice->sendMessageNow(m);
        }
    }

    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback   : public CallbackMessage
    {
    public:
        IncomingMessageCallback (MainContentComponent* o, const MidiMessage& m, const String& s)
           : owner (o), message (m), source (s)
        {}

        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList (message, source);
        }

        Component::SafePointer<MainContentComponent> owner;
        MidiMessage message;
        String source;
    };

    void postMessageToList (const MidiMessage& message, const String& source)
    {
        (new IncomingMessageCallback (this, message, source))->post();
    }

    void addMessageToList (const MidiMessage& message, const String& source)
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
//            const String midiMessageString (timecode + "  -  " + description + " (" + source + ")"); // [7]
            const String midiByteString(String::toHexString (message.getRawData(), message.getRawDataSize()));
            const String midiMessageString (description + " " + source + ":   " + midiByteString); // [7]
            logMessage (midiMessageString);
        }
    }

    //==============================================================================
    AudioDeviceManager deviceManager;           // [1]
    ComboBox midiInputList;                     // [2]
    Label midiInputListLabel;
    int lastInputIndex;                         // [3]
    bool isAddingFromMidiInput;                 // [4]

    MidiKeyboardState keyboardState;            // [5]
    MidiKeyboardComponent keyboardComponent;    // [6]

    ComboBox midiOutputList;
    Label midiOutputListLabel;
    int lastOutputIndex;
    MidiOutput *midiOutputDevice;
    
    TextEditor midiMessagesBox;
    double startTime;
    
    TextButton sysexDataDumpButton;
    Label sysexDataDumpLabel;
    
    TextButton sysexDataIdButton;
    Label sysexDataIdLabel;
    
    TextButton sysexDataPatchDumpButton;
    Label sysexDataPatchDumpLabel;

    TextButton readMidiFileButton;
    Label testLabel;

    StepGridComponent StepGridComponent;
    NoteGridComponent NoteGridComponent;
    
    OwnedArray<TextButton> gridButtons;
    
    MidiFile inputMidiFile;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent);
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
