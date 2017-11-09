//
//  NoteGridEditorComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#include "NoteGridEditorComponent.hpp"

#include "NoteGridComponent.hpp"
#include "NoteGridViewport.hpp"
#include "NoteGridRulerComponent.hpp"


NoteGridEditorComponent::NoteGridEditorComponent ()
: GraphicsComponentBase ("NoteGridEditorComponent"),
properties_(NoteGridProperties())
{
    setName(String("NoteGridEditorComponent"));
    grid_viewport = new NoteGridViewport();
    addAndMakeVisible(grid_viewport);
    
    note_grid = new NoteGridComponent(&properties_, grid_viewport);
    grid_viewport->setNoteGrid(note_grid);
    addAndMakeVisible(note_grid);

    grid_viewport->setViewedComponent(note_grid);
    grid_viewport->setScrollBarsShown(false, false, true, true);
    
    ruler_viewport = new NoteGridViewport();
    addAndMakeVisible(ruler_viewport);
    grid_ruler = new NoteGridRulerComponent(&properties_, ruler_viewport);
    addAndMakeVisible(grid_ruler);
    ruler_viewport->setViewedComponent(grid_ruler);
    //ruler_viewport->setScrollOnDragEnabled(true);
    ruler_viewport->setScrollBarsShown(false, false, false, true);
    
    grid_viewport->linkViewport(ruler_viewport);
    ruler_viewport->linkViewport(grid_viewport);
    
    properties_.note_grid_component_ = note_grid;
    properties_.note_grid_ruler_component_ = grid_ruler;
    properties_.note_grid_editor_component_ = this;
    
}

NoteGridEditorComponent::~NoteGridEditorComponent()
{
    delete note_grid;
    
    delete grid_viewport;
    delete ruler_viewport;
}

void NoteGridEditorComponent::resized()
{
    grid_ruler->setBounds(0, 0, 10000, 48);
    ruler_viewport->setBounds(0, 0, 900, 48);
    
    note_grid->setBounds(0, 24, 10000, 6144);
    grid_viewport->setBounds(0, 24, 900, 380);
}


void NoteGridEditorComponent::mouseDrag(const MouseEvent& e)
{
    //grid_viewport.autoScroll(e.x, e.y, 20, 1);
    //printf("mouseDrag called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void NoteGridEditorComponent::mouseMove(const MouseEvent& e)
{
    repaint();
    //printf("mouseMove called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void NoteGridEditorComponent::mouseUp (const MouseEvent& e)
{
    repaint();
}

bool NoteGridEditorComponent::mouseGridStepPosition(int &x, int &y)
{
    return true;
}

void NoteGridEditorComponent::drawComponent (Graphics& g)
{
}
