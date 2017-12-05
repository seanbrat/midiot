//
//  MidiInstrumentEditorComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#include "MidiInstrumentEditorComponent.hpp"

#include "NoteGridComponent.hpp"
#include "NoteGridViewport.hpp"
#include "NoteGridRulerComponent.hpp"


MidiInstrumentEditorComponent::MidiInstrumentEditorComponent ()
: GraphicsComponentBase ("MidiInstrumentEditorComponent")
{

    
}

MidiInstrumentEditorComponent::~MidiInstrumentEditorComponent()
{

}

void MidiInstrumentEditorComponent::resized()
{

}


void MidiInstrumentEditorComponent::mouseDrag(const MouseEvent& e)
{
    //grid_viewport.autoScroll(e.x, e.y, 20, 1);
    //printf("mouseDrag called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void MidiInstrumentEditorComponent::mouseMove(const MouseEvent& e)
{
    repaint();
    //printf("mouseMove called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void MidiInstrumentEditorComponent::mouseUp (const MouseEvent& e)
{
    repaint();
}

bool MidiInstrumentEditorComponent::mouseGridStepPosition(int &x, int &y)
{
    return true;
}

void MidiInstrumentEditorComponent::drawComponent (Graphics& g)
{
}
