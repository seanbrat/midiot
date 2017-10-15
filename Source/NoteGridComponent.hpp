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
    mouse_click_ypos(0),
    grid_resolution(SixteenthNote),
    division_ppq(24),
    tick_pos_multiplier(2)
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
                TextButton *note_button = new TextButton();
                note_buttons.add(note_button);
                note_button->setColour(TextButton::ColourIds::buttonOnColourId, Colours::firebrick);
                
                addAndMakeVisible(*note_button);
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
        
        g.setColour (Colours::grey);
        g.fillRect (-fill_x, -fill_y, getWidth(), getHeight());
        
        
        int grid_width = getWidth();
        int grid_height = getHeight();
        
        int grid_border_x = 96;
        int grid_border_y = 140;
        
        step_width = division_ppq * 2;
        step_height = step_width;
        
        int grid_step_x = -(getWidth() / 2) + grid_border_x;
        int grid_step_y = (getHeight() / 2) - grid_border_y;
        
        int num_grid_steps = 16;
        int num_notes = 6;
        
        int border_x = -(getWidth() / 2) + grid_border_x;
        int border_y = -(getHeight() / 2) + 20;
        int border_width = 384 * 2;
        int border_height = step_height * num_notes;
        
        g.setColour (Colours::green);
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
            
            grid_step_y -= step_height;
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
        
        for (int i=0; i<num_events; i++)
        {
            MidiMessageSequence::MidiEventHolder* midi_event_ptr(midi_msg_seq->getEventPointer(i));
            MidiMessage midi_msg(midi_event_ptr->message);
            
            if (midi_msg.isNoteOn() && midi_event_ptr->noteOffObject)
            {
                int note_on_time(midi_msg_seq->getEventTime(i));
                int note_off_time = midi_msg_seq->getTimeOfMatchingKeyUp(i);

                int note_num = midi_msg.getNoteNumber() - 60;
                
                int note_y = (getHeight() / 2) - grid_border_y - (step_height)*note_num;
                int note_height = step_height;
                int note_x = -(getWidth() / 2) + grid_border_x + note_on_time * tick_pos_multiplier;
                int note_width = (note_off_time - note_on_time) * tick_pos_multiplier;
                
                g.setColour (Colours::firebrick);
                g.fillRect(note_x, note_y, note_width, note_height);
                g.setColour (Colours::yellow);
                g.drawRect(note_x, note_y, note_width, note_height, 2.0);
                
                int note_pos_x = grid_border_x + note_on_time * tick_pos_multiplier;
                int note_pos_y = 20 + (step_height)*abs(num_notes-note_num-1);
                
                note_buttons[note_button_index++]->setBounds(note_pos_x, note_pos_y, note_width, note_height);
            }
            
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
    
    OwnedArray<TextButton> note_buttons;
  
    
    // Editor properties
    short grid_resolution;
    int tick_pos_multiplier;
    
    // MIDI File properties
    BarBeatTime clip_length;
    int clip_length_ticks;
    int division_ppq;
    int time_sig_numerator;
    int time_sig_denominator;


};




#endif /* NoteGridComponent_hpp */
