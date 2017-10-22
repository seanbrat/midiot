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

#include "GraphicsComponentBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "NoteGridViewport.hpp"
#include "NoteGridProperties.hpp"

#include "MidiClockUtilities.hpp"

using std::vector;

enum MouseMode {
    NormalMouseMode = 0,
    LeftEdgeResizeMouseMode,
    RightEdgeResizeMouseMode
};

//==============================================================================
class NoteGridComponent  : public GraphicsComponentBase
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

            //printf("previous_x: %d\t\t\tprevious_y: %d\n", previous_x, previous_y);
            
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
    
    class NoteEdgeComponent : public ResizableEdgeComponent
    {
    public:
        NoteEdgeComponent(Component *componentToResize,
                          ComponentBoundsConstrainer *constrainer,
                          Edge edgeToResize,
                          NoteGridViewport* viewport) :
            ResizableEdgeComponent(componentToResize, constrainer, edgeToResize),
            grid_viewport(viewport),
            note_component(componentToResize)
        {
        }
    
        void mouseDrag(const MouseEvent& e) override
        {
            ResizableEdgeComponent::mouseDrag(e);

            int edge_x = this->getX();
            
            if (note_component == nullptr)
            {
                jassertfalse; // You've deleted the component that this resizer was supposed to be using!
                return;
            }

            int component_x = note_component->getX();
            
            printf("NoteEdgeComponent mouseDrag with e.x: %d\t\tedge_x: %d\t\tcomponent_x: %d\n", e.x, edge_x, component_x);
            
            
            grid_viewport->autoScroll(this->getX() + note_component->getX() - grid_viewport->getViewPositionX(),
                                      this->getY() + note_component->getY() - grid_viewport->getViewPositionY(),
                                      1, 2);
            
        }
        
    private:
        NoteGridViewport* grid_viewport;
        Component* note_component;
    };

    
    class NoteComponent : public TextButton, ComponentDragger
    {
    public:
        
        NoteComponent(NoteGridViewport* viewport) :
        grid_viewport(viewport),
        normal_mouse_cursor(MouseCursor::StandardCursorType::NormalCursor),
        left_edge_mouse_cursor(MouseCursor::StandardCursorType::LeftEdgeResizeCursor),
        right_edge_mouse_cursor(MouseCursor::StandardCursorType::RightEdgeResizeCursor),
        mouse_cursor_mode(NormalMouseMode)
        {
            note_bounds = new NoteComponentBoundsConstrainer();
            
            left_edge = new NoteEdgeComponent(this, 0, ResizableEdgeComponent::Edge::leftEdge, grid_viewport);
            addAndMakeVisible(left_edge);
            right_edge = new NoteEdgeComponent(this, 0, ResizableEdgeComponent::Edge::rightEdge, grid_viewport);
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
            
            grid_viewport->autoScroll(e.x + this->getX() - grid_viewport->getViewPositionX(),
                                      e.y + this->getY() - grid_viewport->getViewPositionY(),
                                      1, 2);
            //grid_viewport->updateLinkedNoteGridViewport();
            
            beginDragAutoRepeat(10);

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
        
        NoteGridViewport* grid_viewport;
    };
    
    NoteGridComponent (NoteGridProperties* properties, NoteGridViewport* viewport)
    : GraphicsComponentBase ("NoteGridComponent"),
    grid_viewport(viewport),
    properties_(properties)
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
                NoteComponent *note_component = new NoteComponent(grid_viewport);
                note_components.add(note_component);
                note_component->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);
                
                addAndMakeVisible(*note_component);
            }
        }
        
        component_bounds = new ComponentBoundsConstrainer();
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
    
    void setParentNoteGridViewport(NoteGridViewport* viewport)
    {
        grid_viewport = viewport;
    }
    
    void drawComponent (Graphics& g) override
    {
        printf("NoteGridComponent::drawComponent() called\n");
        int fill_x = getWidth() / 2;
        int fill_y = getHeight() / 2;
        
        float step_width = properties_->step_width_;
        float step_height = properties_->step_height_;
        
        g.addTransform (getTransform());
        
        g.setColour (Colours::grey);
        g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
        
        int grid_width = getWidth();
        int grid_height = getHeight();
        
        int grid_step_x = -(getWidth() / 2);
        int grid_step_y = -(getHeight() / 2);
        
        int num_grid_steps = 16 * 8;
        int num_notes = 128;
        
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

        for (int note_row = 0; note_row < num_notes; note_row++)
        {
            grid_step_x = -(getWidth() / 2);

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
        
        if (properties_->init_grid_)
        {
            for (int i=0; i<num_events; i++)
            {
                MidiMessageSequence::MidiEventHolder* midi_event_ptr(midi_msg_seq->getEventPointer(i));
                MidiMessage midi_msg(midi_event_ptr->message);
                
                if (midi_msg.isNoteOn() && midi_event_ptr->noteOffObject)
                {
                    int note_on_time(midi_msg_seq->getEventTime(i));
                    int note_off_time = midi_msg_seq->getTimeOfMatchingKeyUp(i);

                    int note_num = midi_msg.getNoteNumber();
                    
                    int note_height = step_height;
                    int note_width = (note_off_time - note_on_time) * properties_->tick_to_pixel_x_factor_;
                    
                    int note_pos_x = note_on_time * properties_->tick_to_pixel_x_factor_;
                    int note_pos_y = (step_height)*abs(num_notes - note_num - 1);
                    
                    note_components[note_button_index++]->setBounds(note_pos_x, note_pos_y, note_width, note_height);
                }
            }
            
            properties_->init_grid_ = false;
        }
    }
    
    // row major format
    vector<vector<int>> grid_values;
    
    OwnedArray<NoteComponent> note_components;
    
    
    // MIDI File properties
    BarBeatTime clip_length;
    
    ComponentBoundsConstrainer* component_bounds;
    
    NoteGridViewport* grid_viewport;
    
    NoteGridProperties* properties_;

};




#endif /* NoteGridComponent_hpp */
