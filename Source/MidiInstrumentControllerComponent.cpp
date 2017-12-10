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


MidiInstrumentControllerComponent::MidiInstrumentControllerComponent ()
: GraphicsComponentBase ("MidiInstrumentControllerComponent")
{

    
}

MidiInstrumentControllerComponent::~MidiInstrumentControllerComponent()
{

}

void MidiInstrumentControllerComponent::resized()
{

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
