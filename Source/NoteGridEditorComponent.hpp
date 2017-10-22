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


#include <queue>
#include <vector>

#include "GraphicsComponentBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "NoteGridComponent.hpp"
#include "NoteGridViewport.hpp"

#include "MidiClockUtilities.hpp"

using std::vector;


//==============================================================================
class NoteGridEditorComponent  : public GraphicsComponentBase
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
    
    class NoteGridRulerComponent : public GraphicsComponentBase
    {
    public:
        NoteGridRulerComponent(NoteGridViewport* viewport)
        : GraphicsComponentBase ("NoteGridRulerComponent"),
        ruler_viewport(viewport),
        num_steps(16),
        num_rows(4),
        grid_thickness(12.0),
        grid_resolution(SixteenthNote),
        division_ppq(24),
        tick_to_pixel_x_factor(2.0),
        tick_to_pixel_y_factor(2.0),
        previous_mouse_pos(Point<int>(0, 0)),
        previous_mouse_queue_size(5),
        init_grid(true)
        {
            for (int i=0; i<previous_mouse_queue_size; i++)
            {
                previous_mouse_positions.push(Point<int>(0,0));
            }
        }
        
        
        void resized() override
        {
        }
        
        
        void mouseDrag(const MouseEvent& e) override
        {
            //printf("mouseDrag called with e.tiltX %f\n", e.tiltX);

            //float dragDirection(atan(e.x / e)
            
            Point<int> mouse_position(e.getPosition());
            Point<int> check_mouse_pos(previous_mouse_positions.front());
            /*
            float x_drag(abs(previous_mouse_pos.x - e.getPosition().x));
            float y_drag(abs(previous_mouse_pos.y - e.getPosition().y));
            */
            float x_drag(abs(check_mouse_pos.x - mouse_position.x));
            float y_drag(abs(check_mouse_pos.y - mouse_position.y));

            float drag_angle = atanf(x_drag / y_drag);
            
            /*
            printf("\n\nx_drag: %f\t\ty_drag: %f\n", x_drag, y_drag);
            printf("e.getMouseDownPosition().x: %d\n", e.getMouseDownPosition().x);
            printf("e.getMouseDownPosition().y: %d\n", e.getMouseDownPosition().y);
            printf("e.getPosition().x: %d\n", e.getPosition().x);
            printf("e.getPosition().y: %d\n", e.getPosition().y);
            printf("drag_angle: %f\n", drag_angle);
            */

            if (0.0 <= drag_angle && drag_angle <= 0.09)
            {
                printf("--> mouseDrag VERTICAL with drag_angle: %f\n", drag_angle);
                
                int delta_y = mouse_position.y - previous_mouse_positions.back().y;
                printf("-----> delta_y: %d\n", delta_y);
                
                tick_to_pixel_x_factor += (0.01 * (float)delta_y);
                
                if (tick_to_pixel_x_factor < 0.25)
                {
                    tick_to_pixel_x_factor = 0.25;
                }
                
                repaint();
            }
            /*
            else if (1.04 < e.orientation && e.orientation < 2.09)
            {
                printf("mouseDrag up\n");
            }
             */
            
            previous_mouse_positions.pop();
            previous_mouse_positions.push(mouse_position);
            
        }
        
        void mouseMove(const MouseEvent& e) override
        {
            repaint();
            //printf("mouseMove called with e.x: %d and e.y: %d\n", e.x, e.y);
        }
        
        void mouseDown(const MouseEvent& e) override
        {
            previous_mouse_pos = e.getMouseDownPosition();
            
            Point<int> mouse_position = e.getMouseDownPosition();
            for (int i=0; i<previous_mouse_queue_size; i++)
            {
                previous_mouse_positions.push(Point<int>(mouse_position.x,
                                                         mouse_position.y));
            }
            
            ruler_viewport->setScrollOnDragEnabled(true);
            repaint();
        }
       
        
        void mouseUp (const MouseEvent& e) override
        {
            ruler_viewport->setScrollOnDragEnabled(false);
            
            int previous_mouse_pos_size = previous_mouse_positions.size();
            
            for (int i=0; i<previous_mouse_pos_size; i++)
            {
                previous_mouse_positions.pop();
            }
            
            repaint();
        }
        
        
        void drawComponent (Graphics& g) override
        {
            int fill_x = getWidth() / 2;
            int fill_y = getHeight() / 2;
            
            g.addTransform (getTransform());
            
            g.setColour (Colours::lightgrey);
            g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
            
            
            int grid_width = getWidth();
            int grid_height = getHeight();
            
            step_width = division_ppq * tick_to_pixel_x_factor;
            step_height = division_ppq * tick_to_pixel_y_factor;
            
            int grid_step_x = -(getWidth() / 2);
            int grid_step_y = -(getHeight() / 2);
            
            int num_grid_steps = 16 * 8;
            int num_notes = 1;
            
            int border_x = -(getWidth() / 2);
            int border_y = -(getHeight() / 2);
            int border_width = 16 * 8 * 24 * tick_to_pixel_x_factor;
            int border_height = step_height * num_notes;
            
            g.setColour (Colours::darkgrey);
            
            g.drawRect((float)border_x,
                       (float)border_y,
                       (float)border_width,
                       (float)border_height,
                       2.0);
            
            g.setColour (Colours::darkgrey);

            
            BarBeatTime bar_beat_time;
            
            grid_step_x = -(getWidth() / 2);
            
            for (int step = 0; step < num_grid_steps; step++)
            {
                g.drawRect((float)grid_step_x,
                           (float)grid_step_y,
                           step_width,
                           step_height,
                           0.5);
                
                if (!(step % 4))
                {
                    GlyphArrangement ga;
                    //g.fillRect (ga.getBoundingBox (0, ga.getNumGlyphs(), true).getSmallestIntegerContainer().expanded (4));
                    
                    ga.addFittedText (displayFont,
                                      bar_beat_time.getDisplayString(BarBeatTime::TimeResolution::Beats),
                                      (float)grid_step_x+2,
                                      (float)grid_step_y+32,
                                      48, 24, Justification::topLeft, 3);
                    
                    ga.draw (g);
                }
                
                bar_beat_time.advanceTime(BarBeatTime::TimeResolution::Sixteenths);
                grid_step_x += step_width;

            }
            
            grid_step_y += step_height;
            
        }
        
    private:
        NoteGridViewport* ruler_viewport;
        
        int num_steps;
        int num_rows;
        float grid_thickness;
        
        float step_width;
        float step_height;
                
        // Editor properties
        short grid_resolution;
        
        float tick_to_pixel_x_factor;
        float tick_to_pixel_y_factor;
        
        // MIDI File properties
        BarBeatTime clip_length;
        int division_ppq;

        Point<int> previous_mouse_pos;
        
        std::queue<Point<int>> previous_mouse_positions;
        short previous_mouse_queue_size;
        
        bool init_grid;

    };
    
    
    NoteGridEditorComponent ()
    : GraphicsComponentBase ("NoteGridEditorComponent"),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0),
    grid_resolution(SixteenthNote),
    division_ppq(24),
    tick_to_pixel_x_factor(2.0),
    tick_to_pixel_y_factor(2.0),
    init_grid(true)
    {
        addAndMakeVisible(grid_viewport);
        note_grid = new NoteGridComponent(&grid_viewport);
        addAndMakeVisible(note_grid);
        grid_viewport.setViewedComponent(note_grid);
        grid_viewport.setScrollBarsShown(false, false, true, true);
        
        addAndMakeVisible(ruler_viewport);
        grid_ruler = new NoteGridRulerComponent(&ruler_viewport);
        addAndMakeVisible(grid_ruler);
        ruler_viewport.setViewedComponent(grid_ruler);
        //ruler_viewport.setScrollOnDragEnabled(true);
        ruler_viewport.setScrollBarsShown(false, false, false, true);
        
        grid_viewport.linkViewport(&ruler_viewport);
        ruler_viewport.linkViewport(&grid_viewport);
        
    }
    
    ~NoteGridEditorComponent()
    {
        delete note_grid;
    }
    
    void resized() override
    {
        grid_ruler->setBounds(0, 0, 10000, 48);
        ruler_viewport.setBounds(0, 0, 900, 48);

        note_grid->setBounds(0, 24, 10000, 6144);
        grid_viewport.setBounds(0, 24, 900, 380);
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
    
    void drawComponent (Graphics& g) override
    {
    }
    
    int num_steps;
    int num_rows;
    float grid_thickness;
    
    float step_width;
    float step_height;
    

    // Editor properties
    short grid_resolution;
    
    float tick_to_pixel_x_factor;
    float tick_to_pixel_y_factor;
    
    // MIDI File properties
    BarBeatTime clip_length;
    int division_ppq;
    
    bool init_grid;
    
    ComponentBoundsConstrainer* component_bounds;
    
    NoteGridComponent* note_grid;
    NoteGridRulerComponent* grid_ruler;
    
    NoteGridViewport grid_viewport;
    NoteGridViewport ruler_viewport;
};





#endif /* NoteGridEditorComponent_hpp */
