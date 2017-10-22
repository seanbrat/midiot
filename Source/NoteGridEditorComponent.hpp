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
#include "NoteGridProperties.hpp"

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
        NoteGridRulerComponent(NoteGridProperties* properties, NoteGridViewport* viewport)
        : GraphicsComponentBase ("NoteGridRulerComponent"),
        ruler_viewport(viewport),
        properties_(properties),
        previous_mouse_pos(Point<int>(0, 0)),
        previous_mouse_queue_size(5)
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
            Point<int> mouse_position(e.getPosition());
            Point<int> check_mouse_pos(previous_mouse_positions.front());

            float x_drag(abs(check_mouse_pos.x - mouse_position.x));
            float y_drag(abs(check_mouse_pos.y - mouse_position.y));

            float drag_angle = atanf(x_drag / y_drag);
            
            if (0.0 <= drag_angle && drag_angle <= 0.09)
            {
                
                int delta_y = mouse_position.y - previous_mouse_positions.back().y;
                
                properties_->tick_to_pixel_x_factor_ += (0.01 * (float)delta_y);
                
                if (properties_->tick_to_pixel_x_factor_ < 0.25)
                {
                    properties_->tick_to_pixel_x_factor_ = 0.25;
                }
                
                properties_->updateGridProperties();
            }
            
            previous_mouse_positions.pop();
            previous_mouse_positions.push(mouse_position);
            
        }
        
        void mouseMove(const MouseEvent& e) override
        {
            repaint();
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
            
            float step_width = properties_->step_width_;
            float step_height = properties_->step_height_;
            
            int grid_step_x = -(getWidth() / 2);
            int grid_step_y = -(getHeight() / 2);
            
            int num_grid_steps = 16 * 8;
            int num_notes = 1;
            
            int border_x = -(getWidth() / 2);
            int border_y = -(getHeight() / 2);
            int border_width = 16 * 8 * 24 * properties_->tick_to_pixel_x_factor_;
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
        
        // MIDI File properties
        BarBeatTime clip_length;

        Point<int> previous_mouse_pos;
        
        std::queue<Point<int>> previous_mouse_positions;
        short previous_mouse_queue_size;
        
        NoteGridProperties* properties_;

    };
    
    
    NoteGridEditorComponent ()
    : GraphicsComponentBase ("NoteGridEditorComponent"),
    properties_(NoteGridProperties())
    {
        addAndMakeVisible(grid_viewport);
        note_grid = new NoteGridComponent(&properties_, &grid_viewport);
        addAndMakeVisible(note_grid);
        grid_viewport.setViewedComponent(note_grid);
        grid_viewport.setScrollBarsShown(false, false, true, true);
        
        addAndMakeVisible(ruler_viewport);
        grid_ruler = new NoteGridRulerComponent(&properties_, &ruler_viewport);
        addAndMakeVisible(grid_ruler);
        ruler_viewport.setViewedComponent(grid_ruler);
        //ruler_viewport.setScrollOnDragEnabled(true);
        ruler_viewport.setScrollBarsShown(false, false, false, true);
        
        grid_viewport.linkViewport(&ruler_viewport);
        ruler_viewport.linkViewport(&grid_viewport);
        
        properties_.note_grid_component_ = note_grid;
        properties_.note_grid_ruler_component_ = grid_ruler;
        properties_.note_grid_editor_component_ = this;
        
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

    // MIDI File properties
    BarBeatTime clip_length;
    
    
    ComponentBoundsConstrainer* component_bounds;
    
    NoteGridProperties properties_;
    
    NoteGridComponent* note_grid;
    NoteGridRulerComponent* grid_ruler;
    
    NoteGridViewport grid_viewport;
    NoteGridViewport ruler_viewport;
};





#endif /* NoteGridEditorComponent_hpp */
