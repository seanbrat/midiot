//
//  NoteComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/24/17.
//
//

#include "NoteComponent.hpp"
#include "NoteComponentSorter.hpp"
#include "NoteGridViewport.hpp"
#include "NoteComponentBoundsConstrainer.hpp"
#include "NoteGridComponent.hpp"

NoteEdgeComponent::NoteEdgeComponent(NoteComponent *componentToResize,
                  ComponentBoundsConstrainer *constrainer,
                  Edge edgeToResize,
                  NoteGridViewport* viewport,
                  NoteGridComponent* note_grid) :
ResizableEdgeComponent(componentToResize, constrainer, edgeToResize),
grid_viewport(viewport),
note_component(componentToResize),
note_grid_(note_grid)
{
    setName(String("NoteEdgeComponent"));
}

void NoteEdgeComponent::mouseDown(const MouseEvent& e)
{
    mouse_drag_x = e.x;
    mouse_drag_y = e.y;
    mouse_down_y_ = e.y;
    note_grid_->setResizeDownWidth(note_component->getWidth());
    ResizableEdgeComponent::mouseDown(e);
    
    printf("NoteEdgeComponent::mouseDown()\n");
    
    if (!note_grid_->isNoteSelected(note_component))
    {
        note_component->setMouseDownBounds(getBounds());
        note_grid_->setSelectedNote(note_component);
    }
    else
    {
        note_grid_->initSelectedNotes();
    }
}

void NoteEdgeComponent::mouseUp(const MouseEvent& e)
{
    ResizableEdgeComponent::mouseUp(e);
    
    printf("NoteEdgeComponent::mouseUp()\n");
    note_grid_->flushNoteRemovePool();
}

void NoteEdgeComponent::mouseDrag(const MouseEvent& e)
{
    ResizableEdgeComponent::mouseDrag(e);
    
    int edge_x = this->getX();
    
    if (note_component == nullptr)
    {
        jassertfalse; // You've deleted the component that this resizer was supposed to be using!
        return;
    }
    
    int component_x = note_component->getX();
    
    //printf("NoteEdgeComponent mouseDrag with e.x: %d\t\tedge_x: %d\t\tcomponent_x: %d\n", e.x, edge_x, component_x);
    
//    note_grid_->updateSelectedNote(note_component->getBoundsInParent(), note_component);
  
    note_grid_->resizeSelectedNotes(e, note_component, mouse_down_y_);

    
    note_grid_->updateSelectedNotes();
    
    grid_viewport->autoScroll(this->getX() + note_component->getX() - grid_viewport->getViewPositionX(),
                              this->getY() + note_component->getY() - grid_viewport->getViewPositionY(),
                              1, 2);
    
}



NoteComponent::NoteComponent(NoteGridViewport* viewport,
              int note_num,
              int velocity,
              int note_on_time,
              int note_off_time,
              MidiMessageSequence::MidiEventHolder* note_on_ptr,
              MidiMessageSequence::MidiEventHolder* note_off_ptr,
              NoteGridComponent* note_grid) :
mouse_cursor_mode(NormalMouseMode),
normal_mouse_cursor(MouseCursor::StandardCursorType::NormalCursor),
left_edge_mouse_cursor(MouseCursor::StandardCursorType::LeftEdgeResizeCursor),
right_edge_mouse_cursor(MouseCursor::StandardCursorType::RightEdgeResizeCursor),
grid_viewport(viewport),
midi_note_(MIDINote(note_num,
                   velocity,
                   note_on_time,
                   note_off_time)),
note_on_ptr_(note_on_ptr),
note_off_ptr_(note_off_ptr),
note_grid_(note_grid),
mouse_down_bounds_(0,0,0,0),
mouse_drag_y(0)
{
    setName(String("NoteComponent"));
    
    note_bounds = new NoteComponentBoundsConstrainer(midi_note_, this);
    
    left_edge = new NoteEdgeComponent(this,
                                      0,
                                      ResizableEdgeComponent::Edge::leftEdge,
                                      grid_viewport,
                                      note_grid_);
    addAndMakeVisible(left_edge);
    right_edge = new NoteEdgeComponent(this,
                                       0,
                                       ResizableEdgeComponent::Edge::rightEdge,
                                       grid_viewport,
                                       note_grid_);
    addAndMakeVisible(right_edge);
}

NoteComponent::~NoteComponent()
{
    printf("~NoteComponent() destructor...\n");
    delete left_edge;
    delete right_edge;
    //delete note_bounds;
}

MIDINote& NoteComponent::getMidiNote()
{
    return midi_note_;
}


void NoteComponent::resized()
{
    left_edge->setBounds(0, 0, 2, getHeight());
    right_edge->setBounds(getWidth()-2, 0, 2, getHeight());
}


void NoteComponent::moved()
{
    left_edge->setBounds(0, 0, 2, getHeight());
    right_edge->setBounds(getWidth()-2, 0, 2, getHeight());
}


void NoteComponent::modifierKeysChanged(const ModifierKeys &modifiers)
{
    if (modifiers.isCtrlDown())
    {
        if (modifiers.isShiftDown())
        {
            note_grid_->setDrawMode(false);
            note_grid_->setEraseMode(true);
        }
        else
        {
            note_grid_->setDrawMode(true);
            note_grid_->setEraseMode(false);
        }
    }
    else
    {
        note_grid_->setDrawMode(false);
        note_grid_->setEraseMode(false);
    }
}

/*
void NoteComponent::focusGained(FocusChangeType cause)
{
    printf("focusGained\n");
    setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::lightblue);
}

void NoteComponent::focusLost(FocusChangeType cause)
{
    printf("focusLost\n");
    setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);
}
*/
MouseCursor NoteComponent::getMouseCursor()
{
    switch (mouse_cursor_mode)
    {
        case LeftEdgeResizeMouseMode:
            return left_edge_mouse_cursor;
        case RightEdgeResizeMouseMode:
            return right_edge_mouse_cursor;
        case NormalMouseMode:
        default:
        {
            if (note_grid_->getDrawMode())
            {
                return note_grid_->getDrawModeCursor();
            }
            else if (note_grid_->getEraseMode())
            {
                return note_grid_->getEraseModeCursor();
            }
            else
            {
                return normal_mouse_cursor;
            }
        }
    };
}

void NoteComponent::mouseMove (const MouseEvent& e)
{
    printf("NoteComponent::mouseMove() with e.x: %d and e.y: %d\n", e.getScreenX(), e.getScreenY());
}

void NoteComponent::mouseDown (const MouseEvent& e)
{
    note_bounds->set_mouse_drag_pos(e.x, e.y);
    
    mouse_drag_x = e.x;
    mouse_drag_y = e.y;
    mouse_down_y_ = e.y;
    
    note_grid_->startDrag();
    
    printf("mouse_down_y_: %d\n", mouse_down_y_);
    
    startDraggingComponent(left_edge, e);
    startDraggingComponent(right_edge, e);
    startDraggingComponent(this, e);
    
    if (!note_grid_->isNoteSelected(this))
    {
        note_grid_->setSelectedNote(this);
    }
    else
    {
        note_grid_->initSelectedNotes();
    }
}

void NoteComponent::mouseDrag (const MouseEvent& e)
{
    printf("NoteComponent::mouseDrag() called with e.x: %d and e.y: %d\n", e.x, e.y);
    note_bounds->set_mouse_drag_pos(e.x, e.y);
    
    Rectangle<int> grid_bounds = getBoundsInParent();
        
    mouse_drag_x = e.x;
    mouse_drag_y = e.y;

    dragComponent(left_edge, e, note_bounds);
    dragComponent(right_edge, e, note_bounds);
    dragComponent(this, e, note_bounds);
    
    left_edge->setBounds(0, 0, 2, getHeight());
    right_edge->setBounds(getWidth()-2, 0, 2, getHeight());
    
    note_grid_->dragSelectedNotes(e, this, mouse_down_y_);
    
    //printf("\nmouseDrag note_num: %d\n", midi_note.note_num_);
    //note_grid_->updateSelectedNote(grid_bounds, this);
    
    note_grid_->updateSelectedNotes();
    
    grid_viewport->autoScroll(e.x + this->getX() - grid_viewport->getViewPositionX(),
                              e.y + this->getY() - grid_viewport->getViewPositionY(),
                              1, 2);
    
    beginDragAutoRepeat(10);
}

void NoteComponent::mouseUp (const MouseEvent& e)
{
    note_bounds->reset();
    
    mouse_drag_x = -1;
    mouse_drag_y = -1;
    
//    note_grid_->updateSelectedNote(getBoundsInParent(), this);
    
    note_grid_->updateSelectedNotes();
    note_grid_->flushNoteRemovePool();
    note_grid_->endDrag();
        
    note_grid_->repaint();
}

