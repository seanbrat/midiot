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

#include "GraphicsComponentBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

using std::vector;

//==============================================================================
class StepGridComponent  : public GraphicsComponentBase
{
public:
    
    StepGridComponent ()
    : GraphicsComponentBase ("Step Grid Component"),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0)
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
        repaint();
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
                          "Step Width: " + String(step_width)
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
    
    // row major format
    vector<vector<int>> grid_values;
    
    OwnedArray<TextButton> grid_buttons;
    
};



#endif /* StepGridComponent_hpp */
