//
//  NoteGridProperties.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/19/17.
//
//

#include "NoteGridProperties.hpp"
#include "MidiClockUtilities.hpp"

class NoteGridProperties {
public:
    
    NoteGridProperties(int num_steps,
                       int num_rows,
                       float grid_thickness,
                       short grid_resolution,
                       int division_ppq)
    {
        
    }
    
private:
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
    
    bool init_grid;
};
