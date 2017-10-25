//
//  NoteGridComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/14/17.
//
//

#include "NoteGridComponent.hpp"
#include "NoteGridViewport.hpp"
#include "NoteComponent.hpp"

NoteGridComponent::NoteGridComponent(NoteGridProperties* properties, NoteGridViewport* viewport)
: GraphicsComponentBase ("NoteGridComponent"),
grid_viewport(viewport),
properties_(properties)
{
    setName(String("NoteGridComponent"));
    
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
            int note_num(midi_msg.getNoteNumber());
            int note_velocity(midi_msg.getVelocity());
            int note_on_time(midi_msg_seq->getEventTime(i));
            int note_off_time(midi_msg_seq->getTimeOfMatchingKeyUp(i));
            
            NoteComponent *note_component = new NoteComponent(grid_viewport,
                                                              note_num,
                                                              note_velocity,
                                                              note_on_time,
                                                              note_off_time);
            note_component->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);
            
            addAndMakeVisible(*note_component);
            
            note_comps[note_num].add(note_component);
            
        }
    }
    
    component_bounds = new ComponentBoundsConstrainer();
}

NoteGridComponent::~NoteGridComponent()
{
    delete component_bounds;
}

void NoteGridComponent::mouseMove(const MouseEvent& e)
{
    repaint();
}

void NoteGridComponent::mouseUp (const MouseEvent& e)
{
    repaint();
}

bool NoteGridComponent::mouseGridStepPosition(int &x, int &y)
{
    return true;
}

void NoteGridComponent::setParentNoteGridViewport(NoteGridViewport* viewport)
{
    grid_viewport = viewport;
}

void NoteGridComponent::drawComponent (Graphics& g)
{
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
    
    g.setColour (Colours::firebrick);
    
    if (properties_->init_grid_)
    {
        for (int note_num=0; note_num<num_midi_notes_; note_num++)
        {
            int note_array_size = note_comps[note_num].size();
            
            for (int note_index=0; note_index<note_array_size; note_index++)
            {
                NoteComponent* note_component = note_comps[note_num][note_index];
                MIDINote note = note_component->getMidiNote();
                
                int note_height = step_height;
                int note_width = (note.note_off_time_ - note.note_on_time_) * properties_->tick_to_pixel_x_factor_;
                int note_pos_x = note.note_on_time_ * properties_->tick_to_pixel_x_factor_;
                int note_pos_y = (step_height)*abs(num_notes - note.note_num_ - 1);
                
                note_component->setBounds(note_pos_x, note_pos_y, note_width, note_height);
                
            }
        }
        
        properties_->init_grid_ = false;
    }
}
