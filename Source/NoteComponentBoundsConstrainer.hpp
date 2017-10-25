//
//  NoteComponentBoundsConstrainer.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/24/17.
//
//

#ifndef NoteComponentBoundsConstrainer_hpp
#define NoteComponentBoundsConstrainer_hpp

#include <stdio.h>
#include "NoteGridProperties.hpp"

#include "../JuceLibraryCode/JuceHeader.h"

class NoteComponent;

class NoteComponentBoundsConstrainer : public ComponentBoundsConstrainer
{
public:
    NoteComponentBoundsConstrainer(MIDINote& midi_note,
                                   NoteComponent* note_component);
    
    void checkBounds(Rectangle< int > & 	bounds,
                     const Rectangle< int > & 	previousBounds,
                     const Rectangle< int > & 	limits,
                     bool 	isStretchingTop,
                     bool 	isStretchingLeft,
                     bool 	isStretchingBottom,
                     bool 	isStretchingRight) override;
    
    
    int get_mouse_drag_x() { return mouse_drag_x_; }
    int get_mouse_drag_y() { return mouse_drag_y_; }
    void set_mouse_drag_x(int mouse_drag_x) { mouse_drag_x_ = mouse_drag_x; }
    void set_mouse_drag_y(int mouse_drag_y) { mouse_drag_y_ = mouse_drag_y; }
    int get_resize_amount() { return resize_amount_; }

    void set_mouse_drag_pos(int x, int y);
    void reset();
    void set_mouse_mode(int mouse_mode);
    
private:
    int mouse_drag_x_;
    int mouse_drag_y_;
    int mouse_drag_mode_;
    int resize_amount_;
    MIDINote& midi_note_;
    NoteComponent* note_component_;
};


#endif /* NoteComponentBoundsConstrainer_hpp */
