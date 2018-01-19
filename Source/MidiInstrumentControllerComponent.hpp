//
//  MidiInstrumentControllerComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#ifndef MidiInstrumentControllerComponent_hpp
#define MidiInstrumentControllerComponent_hpp

#include <stdio.h>
#include <queue>
#include <vector>

#include "GraphicsComponentBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "NoteGridProperties.hpp"
#include "MidiClockUtilities.hpp"
#include "MidiInstrumentControllerProperties.hpp"

#define MIDI_CONTROLS_PER_TAB       18

#define USE_MIDI_COMPONENT  1


using std::vector;

class NoteGridComponent;
class NoteGridViewport;
class NoteGridRulerComponent;
class MidiStudio;
class MidiControl;
class MidiInstrument;

class MidiControlSlider : public Slider
{
public:
    MidiControlSlider();
    ~MidiControlSlider() {};
    
    void setName(const String& newName) override;
    
private:
    Label control_label_;


};

class MidiControlTab : public GraphicsComponentBase
{
public:
    MidiControlTab()
    : GraphicsComponentBase("MidiInstrumentControllerComponent")
    {
        
    }
    
    ~MidiControlTab()
    {
        
    }
    
    void resized() override
    {
        int slider_x = 0;
        int slider_y = 0;
        int slider_width = 90;
        int slider_height = 140;
        int slider_x_offset = 10;
        
        int x_pos = slider_x;
        int y_pos = slider_y;
        
        int component_width = getWidth();
        
        for (int i=0; i<getNumChildComponents(); i++)
        {
            Component* child_component = getChildComponent(i);
            
            if (x_pos + slider_width >= component_width)
            {
                x_pos = 0;
                y_pos += slider_height + slider_x_offset;
            }
            
            child_component->setBounds(x_pos, y_pos, slider_width, slider_height);
            x_pos += slider_width;
        }
    }
    void drawComponent (Graphics& g) override;

};

class MidiControlTabbedComponent : public TabbedComponent
{
public:
    MidiControlTabbedComponent()
    : TabbedComponent(TabbedButtonBar::TabsAtTop)
    {}
    
    ~MidiControlTabbedComponent()
    {}
    
    void addMidiControlTab(const String &tabName,
                           Colour tabBackgroundColour,
                           Component *contentComponent,
                           bool deleteComponentWhenNotNeeded,
                           int insertIndex=-1)
    {
        TabbedComponent::addTab(tabName,
                                tabBackgroundColour,
                                contentComponent,
                                deleteComponentWhenNotNeeded,
                                insertIndex);
    }
 
    void addAndMakeVisible(Component *child, int zOrder = -1)
    {
        int num_controls = midi_control_components_.size();
        
        int control_tab_index = num_controls / MIDI_CONTROLS_PER_TAB;
    
        MidiControlTab* midi_control_tab;
        
        if (control_tab_index*MIDI_CONTROLS_PER_TAB + 1 > num_controls
            || getNumTabs() == 0)
        {
            const Colour c;
            String tab_name = String("Page ") + String(control_tab_index+1);
            midi_control_tab = new MidiControlTab();
            addTab(tab_name, c, midi_control_tab, true);
        }
        else
        {
            midi_control_tab = (MidiControlTab*)getTabContentComponent(control_tab_index);
        }
        
        midi_control_components_.add(child);
        midi_control_tab->addAndMakeVisible(child);
    }

private:
    Array<Component*> midi_control_components_;
    //OwnedArray<MidiControlTab> midi_control_tabs_;

};

//==============================================================================
class MidiInstrumentControllerComponent : public GraphicsComponentBase,
private MidiKeyboardStateListener,
private Button::Listener,
private Label::Listener
{
public:
    enum GridResolution {
        Off = 0,
        ThirtySecondNote,
        SixteenthNote,
        EighthNote,
        QuarterNote,
        HalfNote,
        OneBar,
        TwoBar,
        FourBar,
        EightBar
    };
    
    
    MidiInstrumentControllerComponent();
    ~MidiInstrumentControllerComponent();
    
    void resized() override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    bool mouseGridStepPosition(int &x, int &y);
    void drawComponent (Graphics& g) override;
    
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    MidiControlSlider* addMidiControlSlider(MidiControl* midi_control);
    void addMidiKeyboardStateListener(MidiKeyboardStateListener* const listener);
    void addMidiInstrument(MidiInstrument* midi_instrument);

    void processNextKeyboardMidiEvent(const MidiMessage& message);
    
    void buttonClicked (Button* button) override;
    void labelTextChanged (Label *labelThatHasChanged) override;

    void savePatch();
    
private:

    MidiKeyboardState keyboard_state_;
    MidiKeyboardComponent keyboard_component_;

    MidiControlTabbedComponent control_slider_tabs_;
    
    OwnedArray<MidiControlSlider> control_sliders_;

    Label patch_name_label_;
    TextButton patch_request_button_;
    TextButton patch_save_button_;
    
    MidiInstrument* midi_instrument_;
    MidiInstrumentControllerProperties midi_instrument_properties_;
};





#endif /* MidiInstrumentControllerComponent_hpp */
