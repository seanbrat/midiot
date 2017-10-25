//
//  NoteGridProperties.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/19/17.
//
//

#ifndef NoteGridProperties_hpp
#define NoteGridProperties_hpp

#include <stdio.h>

#include "../JuceLibraryCode/JuceHeader.h"

#include "MidiClockUtilities.hpp"

class MIDINote
{
public:
    MIDINote(int note_num = 0,
             int velocity = 0,
             int note_on_time = 0,
             int note_off_time = 0,
             int channel = 0)
    : note_num_(note_num),
    velocity_(velocity),
    note_on_time_(note_on_time),
    note_off_time_(note_off_time),
    channel_(channel)
    {
    }
    
    int note_num_;
    int velocity_;
    int note_on_time_;
    int note_off_time_;
    int channel_;
};

enum MouseMode {
    NormalMouseMode = 0,
    LeftEdgeResizeMouseMode,
    RightEdgeResizeMouseMode
};

const int num_midi_notes_ = 128;


class NoteGridProperties {
public:
    
    NoteGridProperties(int num_steps = 16,
                       int num_rows = 4,
                       float grid_thickness = 12.0,
                       short grid_resolution = BarBeatTime::TimeResolution::Sixteenths,
                       int division_ppq = 24,
                       float tick_to_pixel_x_factor = 2.0,
                       float tick_to_pixel_y_factor = 2.0,
                       bool init_grid = true)
    : num_steps_(num_steps),
    num_rows_(num_rows),
    grid_thickness_(grid_thickness),
    grid_resolution_(grid_resolution),
    tick_to_pixel_x_factor_(tick_to_pixel_x_factor),
    tick_to_pixel_y_factor_(tick_to_pixel_y_factor),
    division_ppq_(division_ppq),
    init_grid_(init_grid)
    {
        updateGridProperties();
    }
    
    void updateGridProperties()
    {
        step_width_ = division_ppq_ * tick_to_pixel_x_factor_;
        step_height_ = division_ppq_ * tick_to_pixel_y_factor_;
        init_grid_ = true;
        
        if (note_grid_component_)
        {
            note_grid_component_->repaint();
        }
        
        if (note_grid_ruler_component_)
        {
            note_grid_ruler_component_->repaint();
        }

        if (note_grid_editor_component_)
        {
            note_grid_editor_component_->repaint();
        }

    }
    
    int num_steps_;
    int num_rows_;
    float grid_thickness_;
    float step_width_;
    float step_height_;
    
    short grid_resolution_;
    
    float tick_to_pixel_x_factor_;
    float tick_to_pixel_y_factor_;
    
    BarBeatTime clip_length_;
    int division_ppq_;
    int time_sig_numerator_;
    int time_sig_denominator_;
    
    bool init_grid_;
    
    Component* note_grid_component_;
    Component* note_grid_ruler_component_;
    Component* note_grid_editor_component_;
};

#endif /* NoteGridProperties_hpp */
