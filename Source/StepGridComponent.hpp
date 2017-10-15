//
//  StepGridComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/14/17.
//
//

#ifndef StepGridComponent_hpp
#define StepGridComponent_hpp

#include <stdio.h>
#include <vector>

#include "GraphicsDemoBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

using std::vector;

//==============================================================================
class StepGridComponent  : public GraphicsDemoBase
{
public:
    
    StepGridComponent ()
    : GraphicsDemoBase ("Fill Types: Rectangles"),
    colour1 (Colours::red),
    colour2 (Colours::green),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0),
    mouse_xpos(0),
    mouse_ypos(0),
    mouse_click_xpos(0),
    mouse_click_ypos(0)
    {
        for (int row = 0; row < num_rows; row++)
        {
            for (int step = 0; step < num_steps; step++)
            {
                TextButton *grid_button = new TextButton();
                grid_buttons.add(grid_button);
                grid_button->setClickingTogglesState(true);
                grid_button->setColour(TextButton::ColourIds::buttonOnColourId, Colours::firebrick);
                
                if (step % 4)
                {
                    grid_button->setColour(TextButton::ColourIds::buttonColourId, Colours::darkslategrey);
                }
                
                addAndMakeVisible(*grid_button);
            }
        }
    }
    
    
    void mouseMove(const MouseEvent& e) override
    {
        mouse_xpos = e.x;
        mouse_ypos = e.y;
        
        repaint();
    }
    
    void mouseUp (const MouseEvent& e) override
    {
        mouse_xpos = e.x;
        mouse_ypos = e.y;
        mouse_click_xpos = e.x;
        mouse_click_ypos = e.y;
        
        repaint();
    }
    
    bool mouseGridStepPosition(int &x, int &y)
    {
        return true;
    }
    
    void drawDemo (Graphics& g) override
    {
        int fill_x = getWidth() / 2;
        int fill_y = getHeight() / 2;
        
        g.addTransform (getTransform());
        
        g.setColour (Colours::darkgrey);
        g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
        
        g.setColour (Colours::black);
        
        int grid_width = getWidth();
        int grid_height = getHeight();
        
        step_width = 48;
        step_height = 48;
        
        int grid_step_x;
        int grid_step_y = -(getHeight() / 2) + grid_thickness;
        int grid_pos_x;
        int grid_pos_y = grid_thickness;
        
        int grid_button_index = 0;
        
        for (int row = 0; row < num_rows; row++)
        {
            grid_step_x = -(getWidth() / 2) + grid_thickness;
            grid_pos_x = grid_thickness;
            
            for (int step = 0; step < num_steps; step++)
            {
                //g.fillRect((float)grid_step_x, (float)grid_step_y, step_width, step_height);
                
                //TextButton *grid_button = new TextButton();
                //grid_buttons.add(grid_button);
                //addAndMakeVisible(*grid_button);
                grid_buttons[grid_button_index++]->setBounds(grid_pos_x, grid_pos_y, step_width, step_height);
                
                grid_step_x += grid_thickness + step_width;
                grid_pos_x += grid_thickness + step_width;
            }
            
            grid_step_y += grid_thickness + step_height;
            grid_pos_y += grid_thickness + step_height;
        }
        
        /*
         
         const int rectSize = 100;//jmin (getWidth(), getHeight()) / 2 - 20;
         g.setColour (colour1);//.withAlpha (getAlpha()));
         g.fillRect (-rectSize, -rectSize, rectSize, rectSize);
         
         g.setGradientFill (ColourGradient (colour1, 10.0f, (float) -rectSize,
         colour2, 10.0f + rectSize, 0.0f, false));
         //g.setOpacity (getAlpha());
         g.fillRect (10, -rectSize, rectSize, rectSize);
         
         g.setGradientFill (ColourGradient (colour1, rectSize * -0.5f, 10.0f + rectSize * 0.5f,
         colour2, 0, 10.0f + rectSize, true));
         //g.setOpacity (getAlpha());
         g.fillRect (-rectSize, 10, rectSize, rectSize);
         
         g.setGradientFill (ColourGradient (colour1, 10.0f, 10.0f,
         colour2, 10.0f + rectSize, 10.0f + rectSize, false));
         */
        //g.setOpacity (getAlpha());
        
        int mouse_grid_x = -1;
        int mouse_grid_y = -1;
        
        String selected_grid_step = "none";
        
        if (mouse_grid_x != -1 && mouse_grid_y != -1)
        {
            selected_grid_step = String(mouse_grid_x) + ", " + String(mouse_grid_y);
        }
        
        g.setColour (Colours::white);
        GlyphArrangement ga;
        //g.fillRect (ga.getBoundingBox (0, ga.getNumGlyphs(), true).getSmallestIntegerContainer().expanded (4));
        
        ga.addFittedText (displayFont,
                          "Mouse Pos X:\t\t\t" + String(mouse_xpos)
                          + "\nMouse Pos Y:\t\t\t" + String(mouse_ypos)
                          + "\nMouse Click X:\t" + String (mouse_click_xpos)
                          + "\nMouse Click Y:\t" + String (mouse_click_ypos)
                          + "\nStep Width: " + String(step_width)
                          + " Step Height: " + String(step_height)
                          + "\nSelected Grid Step: " + selected_grid_step,
                          8-fill_x, fill_y-73, 400.0, 400.0, Justification::topLeft, 3);
        
        ga.draw (g);
        
    }
    
    int num_steps;
    int num_rows;
    float grid_thickness;
    
    float step_width;
    float step_height;
    
    Colour colour1, colour2;
    int mouse_xpos;
    int mouse_ypos;
    int mouse_click_xpos;
    int mouse_click_ypos;
    
    // row major format
    vector<vector<int>> grid_values;
    
    OwnedArray<TextButton> grid_buttons;
    
};



#endif /* StepGridComponent_hpp */
