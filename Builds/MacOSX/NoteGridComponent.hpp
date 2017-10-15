//
//  NoteGridComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/14/17.
//
//

#ifndef NoteGridComponent_hpp
#define NoteGridComponent_hpp

#include <stdio.h>
#include <vector>

#include "GraphicsDemoBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

using std::vector;


//==============================================================================
class NoteGridComponent  : public GraphicsDemoBase
{
public:
    NoteGridComponent ()
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




#endif /* NoteGridComponent_hpp */
