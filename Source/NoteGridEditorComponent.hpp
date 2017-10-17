//
//  NoteGridEditorComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#ifndef NoteGridEditorComponent_hpp
#define NoteGridEditorComponent_hpp

#include <stdio.h>



#include <vector>

#include "GraphicsDemoBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "NoteGridComponent.hpp"

#include "MidiClockUtilities.hpp"

using std::vector;

//==============================================================================
class NoteGridEditorComponent  : public GraphicsDemoBase
{
public:
    enum GridResolution {
        Off = 0,
        ThirtySecondNote,
        SixteenthNote,
        EighthNote,
        QuarterNote,
        HalfNote,
        OneBar,
        TwoBar,
        FourBar,
        EightBar
    };
    
    
    NoteGridEditorComponent ()
    : GraphicsDemoBase ("NoteGridEditorComponent"),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0),
    grid_resolution(SixteenthNote),
    division_ppq(24),
    tick_pos_multiplier(2),
    tick_to_pixel_x_factor(2.0),
    tick_to_pixel_y_factor(2.0),
    init_grid(true)
    {
        addAndMakeVisible(grid_viewport);
        
        note_grid = new NoteGridComponent(&grid_viewport);
        addAndMakeVisible(note_grid);
        
        grid_viewport.setViewedComponent(note_grid);
        //grid_viewport.setScrollOnDragEnabled(true);
        //grid_viewport.autoScroll(0, 0, 20, 2);
    }
    
    ~NoteGridEditorComponent()
    {
        delete note_grid;
    }
    
    void resized() override
    {
        note_grid->setBounds(0, 0, 10000, 6144);
        grid_viewport.setBounds(0, 0, 900, 380);
    }
    
    
    void mouseDrag(const MouseEvent& e) override
    {
        //grid_viewport.autoScroll(e.x, e.y, 20, 1);
        //printf("mouseDrag called with e.x: %d and e.y: %d\n", e.x, e.y);
    }
    
    void mouseMove(const MouseEvent& e) override
    {
        repaint();
        //printf("mouseMove called with e.x: %d and e.y: %d\n", e.x, e.y);
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        repaint();
    }
    
    bool mouseGridStepPosition(int &x, int &y)
    {
        return true;
    }
    
    void drawDemo (Graphics& g) override
    {

        
    }
    
    int num_steps;
    int num_rows;
    float grid_thickness;
    
    float step_width;
    float step_height;
    

    // Editor properties
    short grid_resolution;
    int tick_pos_multiplier;
    
    float tick_to_pixel_x_factor;
    float tick_to_pixel_y_factor;
    
    // MIDI File properties
    BarBeatTime clip_length;
    int clip_length_ticks;
    int division_ppq;
    int time_sig_numerator;
    int time_sig_denominator;
    
    bool init_grid;
    
    ComponentBoundsConstrainer* component_bounds;
    
    NoteGridComponent* note_grid;
    
    Viewport grid_viewport;
};





#endif /* NoteGridEditorComponent_hpp */
