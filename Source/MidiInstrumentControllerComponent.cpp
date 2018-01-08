//
//  MidiInstrumentControllerComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#include "MidiInstrumentControllerComponent.hpp"

#include "NoteGridComponent.hpp"
#include "NoteGridViewport.hpp"
#include "NoteGridRulerComponent.hpp"
#include "MidiStudio.hpp"
#include "MidiInstrument.hpp"


void MidiControlTab::drawComponent (Graphics& g)
{
    //resized();
}

MidiControlSlider::MidiControlSlider()
: Slider()
{
    setSliderStyle(LinearVertical);
}

void MidiControlSlider::setName(const String& newName)
{
    Component::setName(newName);
    control_label_.setText(newName, dontSendNotification);
    control_label_.attachToComponent(this, false);
}

MidiInstrumentControllerComponent::MidiInstrumentControllerComponent ()
: GraphicsComponentBase("MidiInstrumentControllerComponent"),
keyboard_component_(keyboard_state_, MidiKeyboardComponent::horizontalKeyboard),
control_slider_tabs_()
{
    addAndMakeVisible(keyboard_component_);
    keyboard_state_.addListener(this);

#if USE_MIDI_COMPONENT
    addAndMakeVisible(control_slider_tabs_);
#endif
    
    addAndMakeVisible(patch_request_button_);
    patch_request_button_.setButtonText("Patch Request");
    patch_request_button_.addListener(this);
}

MidiInstrumentControllerComponent::~MidiInstrumentControllerComponent()
{

}

void MidiInstrumentControllerComponent::addMidiInstrument(MidiInstrument* midi_instrument)
{
    midi_instrument_ = midi_instrument;
}

void MidiInstrumentControllerComponent::addMidiKeyboardStateListener(MidiKeyboardStateListener* const listener)
{
    keyboard_state_.addListener(listener);
}

void MidiInstrumentControllerComponent::resized()
{
    keyboard_component_.setBounds(0, 0, 1000, 80);
    
    int x_pos;
    int y_pos;
    
#if USE_MIDI_COMPONENT
    control_slider_tabs_.setBounds(0, 100, 1100, 500);
    //midi_control_tab_.setBounds(0, 80, 1000, 140);
    x_pos = 0;
    y_pos = 600;
    //control_slider_tabs_.resized();
#else
    int slider_x = 0;
    int slider_y = 100;
    int slider_width = 100;
    int slider_height = 140;
    
    x_pos = slider_x;
    y_pos = slider_y;
    
    for (int i=0; i<control_sliders_.size(); i++)
    {
        if (x_pos + slider_width >= 1000)
        {
            x_pos = 0;
            y_pos += slider_height + 30;
        }
        
        control_sliders_[i]->setBounds(x_pos, y_pos, slider_width, slider_height);
        x_pos += slider_width;
    }
    
    x_pos = 0;
    y_pos += slider_height + 30;
#endif
    
    patch_request_button_.setBounds(x_pos, y_pos, 100, 40);
}


void MidiInstrumentControllerComponent::mouseDrag(const MouseEvent& e)
{
    //grid_viewport.autoScroll(e.x, e.y, 20, 1);
    //printf("mouseDrag called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void MidiInstrumentControllerComponent::mouseMove(const MouseEvent& e)
{
    repaint();
    //printf("mouseMove called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void MidiInstrumentControllerComponent::mouseUp (const MouseEvent& e)
{
    repaint();
}

bool MidiInstrumentControllerComponent::mouseGridStepPosition(int &x, int &y)
{
    return true;
}

void MidiInstrumentControllerComponent::drawComponent (Graphics& g)
{
}

void MidiInstrumentControllerComponent::buttonClicked (Button* button)
{
    if (button == &patch_request_button_)
    {
        midi_instrument_->sendSysexPatchDumpMessage();
    }
}

void MidiInstrumentControllerComponent::handleNoteOn(
                                            MidiKeyboardState* keyboard_state,
                                            int midi_channel,
                                            int midi_note_number,
                                            float velocity)
{
}

void MidiInstrumentControllerComponent::handleNoteOff(
                                            MidiKeyboardState* keyboard_state,
                                            int midi_channel,
                                            int midi_note_number,
                                            float velocity)
{
}

MidiControlSlider* MidiInstrumentControllerComponent::addMidiControlSlider(MidiControl* midi_control)
{
    MidiControlSlider* control_slider = new MidiControlSlider();
    control_slider->setName(midi_control->name());
    control_slider->setRange(midi_control->getRangeMinimum(),
                             midi_control->getRangeMaximum(),
                             1.0);
    control_slider->addListener(midi_control);
    control_sliders_.add(control_slider);
#if USE_MIDI_COMPONENT
    control_slider_tabs_.addAndMakeVisible(control_slider);
//    midi_control_tab_.addMidiControlAndMakeVisible(control_slider);
#else
    addAndMakeVisible(control_slider);
#endif
    
    return control_slider;
}

void MidiInstrumentControllerComponent::processNextKeyboardMidiEvent(const MidiMessage& message)
{
    keyboard_state_.processNextMidiEvent (message);
}

