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

#include "MidiClockUtilities.hpp"

using std::vector;

enum MouseMode {
    NormalMouseMode = 0,
    LeftEdgeResizeMouseMode,
    RightEdgeResizeMouseMode
};

//==============================================================================
class NoteGridComponent  : public GraphicsDemoBase
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
    
    class NoteComponentBoundsConstrainer : public ComponentBoundsConstrainer
    {
    public:
        NoteComponentBoundsConstrainer() :
            mouse_drag_x_(0),
            mouse_drag_y_(0),
            mouse_drag_mode_(NormalMouseMode),
            resize_amount_(0)
        {
        }
        
        void checkBounds(Rectangle< int > & 	bounds,
                         const Rectangle< int > & 	previousBounds,
                         const Rectangle< int > & 	limits,
                         bool 	isStretchingTop,
                         bool 	isStretchingLeft,
                         bool 	isStretchingBottom,
                         bool 	isStretchingRight) override
        {
            int previous_x = previousBounds.getX();
            int previous_y = previousBounds.getY();
            int previous_width = previousBounds.getWidth();
            int previous_height = previousBounds.getHeight();

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
        
        int get_mouse_drag_x() { return mouse_drag_x_; }
        int get_mouse_drag_y() { return mouse_drag_y_; }
        void set_mouse_drag_x(int mouse_drag_x) { mouse_drag_x_ = mouse_drag_x; }
        void set_mouse_drag_y(int mouse_drag_y) { mouse_drag_y_ = mouse_drag_y; }
        
        int get_resize_amount() { return resize_amount_; }
        
        void set_mouse_drag_pos(int x, int y)
        {
            set_mouse_drag_x(x);
            set_mouse_drag_y(y);
        }
        
        void reset()
        {
            set_mouse_drag_pos(0, 0);
        }
        
        void set_mouse_mode(int mouse_mode)
        {
            mouse_drag_mode_ = mouse_mode;
        }
        
    private:
        int mouse_drag_x_;
        int mouse_drag_y_;
        int mouse_drag_mode_;
        int resize_amount_;
    };
    

    
    class NoteComponent : public TextButton, ComponentDragger
    {
    public:
        
        NoteComponent() :
        normal_mouse_cursor(MouseCursor::StandardCursorType::NormalCursor),
        left_edge_mouse_cursor(MouseCursor::StandardCursorType::LeftEdgeResizeCursor),
        right_edge_mouse_cursor(MouseCursor::StandardCursorType::RightEdgeResizeCursor),
        mouse_cursor_mode(NormalMouseMode)
        {
            note_bounds = new NoteComponentBoundsConstrainer();
            
            left_edge = new ResizableEdgeComponent(this, 0, ResizableEdgeComponent::Edge::leftEdge);
            addAndMakeVisible(left_edge);
            right_edge = new ResizableEdgeComponent(this, 0, ResizableEdgeComponent::Edge::rightEdge);
            addAndMakeVisible(right_edge);
        }
        
        ~NoteComponent()
        {
            delete left_edge;
            delete right_edge;
            delete note_bounds;
        }
        
        void resized() override
        {
            left_edge->setBounds(0, 0, 2, getHeight());
            right_edge->setBounds(getWidth()-2, 0, 2, getHeight());
        }
        

        MouseCursor getMouseCursor() override
        {
            switch (mouse_cursor_mode)
            {
                case LeftEdgeResizeMouseMode:
                    return left_edge_mouse_cursor;
                case RightEdgeResizeMouseMode:
                    return right_edge_mouse_cursor;
                case NormalMouseMode:
                default:
                    return normal_mouse_cursor;
            };
        }
        
        void mouseMove (const MouseEvent& e) override
        {
        }
        
        void mouseDown (const MouseEvent& e) override
        {
            note_bounds->set_mouse_drag_pos(e.x, e.y);
            
            mouse_drag_x = e.x;
            mouse_drag_y = e.y;
            
            startDraggingComponent(left_edge, e);
            startDraggingComponent(right_edge, e);
            startDraggingComponent(this, e);

        }
        
        void mouseDrag (const MouseEvent& e) override
        {
            note_bounds->set_mouse_drag_pos(e.x, e.y);
            
            mouse_drag_x = e.x;
            mouse_drag_y = e.y;
            
            dragComponent(left_edge, e, note_bounds);
            dragComponent(right_edge, e, note_bounds);
            dragComponent(this, e, note_bounds);

            left_edge->setBounds(0, 0, 2, getHeight());
            right_edge->setBounds(getWidth()-2, 0, 2, getHeight());
        }
        
        void mouseUp (const MouseEvent& e) override
        {
            note_bounds->reset();
            
            mouse_drag_x = -1;
            mouse_drag_y = -1;
        }
    
    private:
        ScopedPointer<NoteComponentBoundsConstrainer> note_bounds;

        int mouse_drag_x;
        int mouse_drag_y;
        
        ResizableEdgeComponent *left_edge;
        ResizableEdgeComponent *right_edge;
        
        int mouse_cursor_mode;
        
        MouseCursor normal_mouse_cursor;
        MouseCursor left_edge_mouse_cursor;
        MouseCursor right_edge_mouse_cursor;
        
    };
    
    NoteGridComponent ()
    : GraphicsDemoBase ("NoteGridComponent"),
    colour1 (Colours::red),
    colour2 (Colours::green),
    num_steps(16),
    num_rows(4),
    grid_thickness(12.0),
    mouse_xpos(0),
    mouse_ypos(0),
    mouse_click_xpos(0),
    mouse_click_ypos(0),
    grid_resolution(SixteenthNote),
    division_ppq(24),
    tick_pos_multiplier(2),
    tick_to_pixel_x_factor(2.0),
    tick_to_pixel_y_factor(2.0),
    init_grid(true)
    {
        
        MidiFile inputMidiFile;
        
        File midiFile = File::createFileWithoutCheckingPath (String("/Users/seanb/Development/JUCE/Midiot/Resources/basic808.mid"));
        FileInputStream midiStream (midiFile);
        inputMidiFile.readFrom (midiStream);
        
        String log_message_string;
        
        int num_tracks = inputMidiFile.getNumTracks();
        
        const MidiMessageSequence* midi_msg_seq(inputMidiFile.getTrack(0));
        int num_events = midi_msg_seq->getNumEvents();
        
        for (int i=0; i<num_events; i++)
        {
            MidiMessageSequence::MidiEventHolder* midi_event_ptr(midi_msg_seq->getEventPointer(i));
            MidiMessage midi_msg(midi_event_ptr->message);
            
            if (midi_msg.isNoteOn() && midi_event_ptr->noteOffObject)
            {
                NoteComponent *note_component = new NoteComponent();
                note_components.add(note_component);
                note_component->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);
                
                addAndMakeVisible(*note_component);
            }
        }
        
        component_bounds = new ComponentBoundsConstrainer();
        
        addAndMakeVisible(grid_viewport);
//        grid_viewport.setViewedComponent(<#juce::Component *newViewedComponent#>)
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
        
        g.setColour (Colours::grey);
        g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
        
        int grid_width = getWidth();
        int grid_height = getHeight();
        
        int grid_border_x = 96;
        int grid_border_y = 20;
        
        step_width = division_ppq * tick_to_pixel_x_factor;
        step_height = division_ppq * tick_to_pixel_y_factor;
        
        int grid_step_x = -(getWidth() / 2) + grid_border_x;
        int grid_step_y = -(getHeight() / 2) + grid_border_y;
        
        int num_grid_steps = 16;
        int num_notes = 6;
        
        int border_x = -(getWidth() / 2) + grid_border_x;
        int border_y = -(getHeight() / 2) + grid_border_y;
        int border_width = 384 * tick_to_pixel_x_factor;
        int border_height = step_height * num_notes;
        
        g.setColour (Colours::darkgrey);

        g.drawRect((float)border_x,
                   (float)border_y,
                   (float)border_width,
                   (float)border_height,
                   2.0);

        g.setColour (Colours::darkgrey);

        for (int note_row = 0; note_row < num_notes; note_row++)
        {
            grid_step_x = -(getWidth() / 2) + grid_border_x;

            for (int step = 0; step < num_grid_steps; step++)
            {
                g.drawRect((float)grid_step_x,
                           (float)grid_step_y,
                           step_width,
                           step_height,
                           0.5);
                
                grid_step_x += step_width;
            }
            
            grid_step_y += step_height;
        }
        
        int grid_button_index = 0;
        
        MidiFile inputMidiFile;
        
        File midiFile = File::createFileWithoutCheckingPath (String("/Users/seanb/Development/JUCE/Midiot/Resources/basic808.mid"));
        FileInputStream midiStream (midiFile);
        inputMidiFile.readFrom (midiStream);
        
        String log_message_string;
        
        int num_tracks = inputMidiFile.getNumTracks();
        
        const MidiMessageSequence* midi_msg_seq(inputMidiFile.getTrack(0));
        int num_events = midi_msg_seq->getNumEvents();
        
        g.setColour (Colours::firebrick);
        
        int note_button_index = 0;
        
        
        if (init_grid)
        {
            for (int i=0; i<num_events; i++)
            {
                MidiMessageSequence::MidiEventHolder* midi_event_ptr(midi_msg_seq->getEventPointer(i));
                MidiMessage midi_msg(midi_event_ptr->message);
                
                if (midi_msg.isNoteOn() && midi_event_ptr->noteOffObject)
                {
                    int note_on_time(midi_msg_seq->getEventTime(i));
                    int note_off_time = midi_msg_seq->getTimeOfMatchingKeyUp(i);

                    int note_num = midi_msg.getNoteNumber() - 60;
                    
                    int note_y = -(getHeight() / 2) + grid_border_y + (step_height)*abs(num_notes-note_num-1);
                    int note_height = step_height;
                    int note_x = -(getWidth() / 2) + grid_border_x + note_on_time * tick_to_pixel_x_factor;
                    int note_width = (note_off_time - note_on_time) * tick_to_pixel_x_factor;
                    /*
                    g.setColour (Colours::firebrick);
                    g.fillRect(note_x, note_y, note_width, note_height);
                    g.setColour (Colours::yellow);
                    g.drawRect(note_x, note_y, note_width, note_height, 2.0);
                    */
                    
                    int note_pos_x = grid_border_x + note_on_time * tick_to_pixel_x_factor;
                    int note_pos_y = 20 + (step_height)*abs(num_notes-note_num-1);
                    
                    note_components[note_button_index++]->setBounds(note_pos_x, note_pos_y, note_width, note_height);
                }
            }
            
            init_grid = false;
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
    
    OwnedArray<NoteComponent> note_components;
    
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
    
    Viewport grid_viewport;

};




#endif /* NoteGridComponent_hpp */
