//
//  NoteGridRulerComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/24/17.
//
//

#include "NoteGridRulerComponent.hpp"
#include "NoteGridViewport.hpp"
#include "NoteGridProperties.hpp"


NoteGridRulerComponent::NoteGridRulerComponent(NoteGridProperties* properties, NoteGridViewport* viewport)
: GraphicsComponentBase ("NoteGridRulerComponent"),
ruler_viewport(viewport),
properties_(properties),
previous_mouse_pos(Point<int>(0, 0)),
previous_mouse_queue_size(5)
{
    setName(String("NoteGridRulerComponent"));
    
    for (int i=0; i<previous_mouse_queue_size; i++)
    {
        previous_mouse_positions.push(Point<int>(0,0));
    }
}


void NoteGridRulerComponent::resized()
{
}


void NoteGridRulerComponent::mouseDrag(const MouseEvent& e)
{
    Point<int> mouse_position(e.getPosition());
    Point<int> check_mouse_pos(previous_mouse_positions.front());
    
    float x_drag(abs(check_mouse_pos.x - mouse_position.x));
    float y_drag(abs(check_mouse_pos.y - mouse_position.y));
    
    float drag_angle = atanf(x_drag / y_drag);
    
    printf("NoteGridRulerComponent::mouseDrag\n\n");
    
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

void NoteGridRulerComponent::mouseMove(const MouseEvent& e)
{
    repaint();
}

void NoteGridRulerComponent::mouseDown(const MouseEvent& e)
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


void NoteGridRulerComponent::mouseUp (const MouseEvent& e)
{
    ruler_viewport->setScrollOnDragEnabled(false);
    
    int previous_mouse_pos_size = previous_mouse_positions.size();
    
    for (int i=0; i<previous_mouse_pos_size; i++)
    {
        previous_mouse_positions.pop();
    }
    
    repaint();
}


void NoteGridRulerComponent::drawComponent (Graphics& g)
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
