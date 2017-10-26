//
//  NoteComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/24/17.
//
//

#ifndef NoteComponent_hpp
#define NoteComponent_hpp

#include <stdio.h>

#include "GraphicsComponentBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "NoteGridProperties.hpp"

class NoteGridViewport;
class NoteComponentBoundsConstrainer;
class NoteGridComponent;
class NoteComponent;

class NoteEdgeComponent : public ResizableEdgeComponent
{
public:
    NoteEdgeComponent(NoteComponent *componentToResize,
                      ComponentBoundsConstrainer *constrainer,
                      Edge edgeToResize,
                      NoteGridViewport* viewport,
                      NoteGridComponent* note_grid);

    void mouseDrag(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

private:
    NoteGridViewport* grid_viewport;
    NoteGridComponent* note_grid_;
    NoteComponent* note_component;
};


class NoteComponent : public TextButton, ComponentDragger
{
public:
    
    NoteComponent(NoteGridViewport* viewport,
                  int note_num,
                  int velocity,
                  int note_on_time,
                  int note_off_time,
                  MidiMessageSequence::MidiEventHolder* note_on_ptr,
                  MidiMessageSequence::MidiEventHolder* note_off_ptr,
                  NoteGridComponent* note_grid);
    
    ~NoteComponent();
    
    MIDINote& getMidiNote();
    void setMidiNote(MIDINote& midi_note) { midi_note_ = midi_note; };
    
    void resized() override;
    
    MouseCursor getMouseCursor() override;
    void mouseMove (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    
    //virtual void focusGained(FocusChangeType cause);
    //virtual void focusLost(FocusChangeType cause);

private:
    ScopedPointer<NoteComponentBoundsConstrainer> note_bounds;
    
    int mouse_drag_x;
    int mouse_drag_y;
    
    ResizableEdgeComponent *left_edge;
    ResizableEdgeComponent *right_edge;
    
    int mouse_cursor_mode;
    
    MouseCursor normal_mouse_cursor;
    MouseCursor left_edge_mouse_cursor;
    MouseCursor right_edge_mouse_cursor;
    
    NoteGridViewport* grid_viewport;
    NoteGridComponent* note_grid_;
    
    // MIDI note info
    MIDINote midi_note_;
    
    // pointer to MidiEventHolders for note
    MidiMessageSequence::MidiEventHolder* note_on_ptr_;
    MidiMessageSequence::MidiEventHolder* note_off_ptr_;
};



#endif /* NoteComponent_hpp */
