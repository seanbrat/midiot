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

using std::vector;

class NoteGridComponent;
class NoteGridViewport;
class NoteGridRulerComponent;
class MidiStudio;
class MidiControl;


class MidiControlSlider : public Slider
{
public:
    MidiControlSlider();
    ~MidiControlSlider() {};
    
    void setName(const String& newName) override;
    
private:
    Label control_label_;


};

//==============================================================================
class MidiInstrumentControllerComponent  : public GraphicsComponentBase,
private MidiKeyboardStateListener
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

    void processNextKeyboardMidiEvent(const MidiMessage& message);
    
private:

    MidiKeyboardState keyboard_state_;
    MidiKeyboardComponent keyboard_component_;
    
    OwnedArray<MidiControlSlider> control_sliders_;

};





#endif /* MidiInstrumentControllerComponent_hpp */
