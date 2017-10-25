//
//  NoteComponentBoundsConstrainer.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/24/17.
//
//

#include "NoteComponentBoundsConstrainer.hpp"


NoteComponentBoundsConstrainer::NoteComponentBoundsConstrainer(MIDINote& midi_note) :
midi_note_(midi_note),
mouse_drag_x_(0),
mouse_drag_y_(0),
mouse_drag_mode_(NormalMouseMode),
resize_amount_(0)
{
}

void NoteComponentBoundsConstrainer::checkBounds(Rectangle< int > & 	bounds,
                 const Rectangle< int > & 	previousBounds,
                 const Rectangle< int > & 	limits,
                 bool 	isStretchingTop,
                 bool 	isStretchingLeft,
                 bool 	isStretchingBottom,
                 bool 	isStretchingRight)
{
    int previous_x = previousBounds.getX();
    int previous_y = previousBounds.getY();
    int previous_width = previousBounds.getWidth();
    int previous_height = previousBounds.getHeight();
    
    //printf("previous_x: %d\t\t\tprevious_y: %d\n", previous_x, previous_y);
    
    int new_x;
    int new_y;
    int new_width;
    
    if (mouse_drag_y_ < 0)
    {
        new_y = previous_y - previous_height;
        bounds.setY(new_y);
    }
    else if (mouse_drag_y_ > previous_height)
    {
        new_y = previous_y + previous_height;
        bounds.setY(new_y);
    }
    else
    {
        bounds.setY(previous_y);
    }
}

void NoteComponentBoundsConstrainer::set_mouse_drag_pos(int x, int y)
{
    set_mouse_drag_x(x);
    set_mouse_drag_y(y);
}

void NoteComponentBoundsConstrainer::reset()
{
    set_mouse_drag_pos(0, 0);
}

void NoteComponentBoundsConstrainer::set_mouse_mode(int mouse_mode)
{
    mouse_drag_mode_ = mouse_mode;
}
