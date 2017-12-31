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
keyboard_component_(keyboard_state_, MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboard_component_);
    keyboard_state_.addListener(this);
    
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
    
    int slider_x = 0;
    int slider_y = 100;
    int slider_width = 100;
    int slider_height = 140;
    
    int x_pos = slider_x;
    int y_pos = slider_y;
    
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
    addAndMakeVisible(control_slider);
    
    return control_slider;
}

void MidiInstrumentControllerComponent::processNextKeyboardMidiEvent(const MidiMessage& message)
{
    keyboard_state_.processNextMidiEvent (message);
}

