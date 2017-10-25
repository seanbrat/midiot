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
#include "NoteComponentSorter.hpp"

NoteGridComponent::NoteGridComponent(NoteGridProperties* properties,
                                     NoteGridViewport* viewport)
: GraphicsComponentBase ("NoteGridComponent"),
grid_viewport(viewport),
properties_(properties),
selected_note_num_(-1),
selected_note_on_time_(0),
selected_note_off_time_(0)
{
    setName(String("NoteGridComponent"));
    
    note_sorter_ = new NoteComponentSorter();
    
    MidiFile inputMidiFile;
    
    File midiFile = File::createFileWithoutCheckingPath (String("/Users/seanb/Development/JUCE/Midiot/Resources/basic808.mid"));
    FileInputStream midiStream (midiFile);
    inputMidiFile.readFrom (midiStream);
    
    String log_message_string;
    
    int num_tracks = inputMidiFile.getNumTracks();
    
    midi_msg_seq = inputMidiFile.getTrack(0);
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
                                                              note_off_time,
                                                              midi_event_ptr,
                                                              midi_event_ptr->noteOffObject,
                                                              this);
            note_component->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);
            
            addAndMakeVisible(*note_component);
            
            note_components.addSorted(*note_sorter_, note_component);
            
        }
    }
    
    component_bounds = new ComponentBoundsConstrainer();
}

NoteGridComponent::~NoteGridComponent()
{
    delete component_bounds;
    delete note_sorter_;
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

int NoteGridComponent::getNoteNum(int y)
{
    return -((y / properties_->step_height_) - num_midi_notes_ + 1);
}

int NoteGridComponent::getNoteOnTime(int x)
{
    return x / properties_->tick_to_pixel_x_factor_;
}

int NoteGridComponent::getNoteOffTime(int note_on_time, int width)
{
    return (width / properties_->tick_to_pixel_x_factor_) + note_on_time;
}

void NoteGridComponent::checkNoteGridBounds(Rectangle<int> grid_bounds, NoteComponent* note_component)
{
    //printf("\ncheckNoteGridBounds at x: %d\t\ty: %d\t\twidth: %d\t\theight: %d\n", grid_bounds.getX(), grid_bounds.getY(), grid_bounds.getWidth(), grid_bounds.getHeight());
    
    selected_note_on_time_ = getNoteOnTime(grid_bounds.getX());
    selected_note_num_ = getNoteNum(grid_bounds.getY());
    selected_note_off_time_ = getNoteOffTime(selected_note_on_time_, grid_bounds.getWidth());
    
    int selected_note_index = note_components.indexOf(note_component);
    
    //printf("note_num: %d\t\tnote_on_time: %d\t\tnote_off_time: %d\n", selected_note_num_, selected_note_on_time_, selected_note_off_time_);
    
    // check for overlapping notes
    
    if (selected_note_num_ >= 0)
    {
        for (int note_index=0; note_index<note_components.size(); note_index++)
        {
            if (note_index == selected_note_index)
            {
                continue;
            }
            
            MIDINote check_note = note_components[note_index]->getMidiNote();
            
            if (selected_note_num_ == check_note.note_num_)
            {
                if (selected_note_on_time_ >= check_note.note_on_time_
                    && selected_note_on_time_ < check_note.note_off_time_)
                {
                    printf("1 overlap at note: %d at note_on_time_: %d\n", check_note.note_num_, check_note.note_on_time_);
                    note_components[note_index]->setVisible(false);
                }
                else if (selected_note_off_time_ > check_note.note_on_time_
                         && selected_note_off_time_ < check_note.note_off_time_)
                {
                    printf("2 overlap at note: %d at note_on_time_: %d\n", check_note.note_num_, check_note.note_on_time_);
                    
                    note_components[note_index]->setVisible(false);
                }
                else if (selected_note_on_time_ <= check_note.note_on_time_
                         && selected_note_off_time_ >= check_note.note_off_time_)
                {
                    printf("3 overlap at note: %d at note_on_time_: %d\n", check_note.note_num_, check_note.note_on_time_);
                    
                    note_components[note_index]->setVisible(false);
                }
            }
        }
    }
    
    repaint();
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
        int note_array_size = note_components.size();
        
        for (int note_index=0; note_index<note_array_size; note_index++)
        {
            NoteComponent* note_component = note_components[note_index];
            MIDINote note = note_component->getMidiNote();
            
            int note_height = step_height;
            int note_width = (note.note_off_time_ - note.note_on_time_) * properties_->tick_to_pixel_x_factor_;
            int note_pos_x = note.note_on_time_ * properties_->tick_to_pixel_x_factor_;
            int note_pos_y = (step_height)*(num_notes - note.note_num_ - 1);
            
            note_component->setBounds(note_pos_x, note_pos_y, note_width, note_height);
        }
    
        properties_->init_grid_ = false;
    }
    
    grid_step_x = -(getWidth() / 2) + 2;
    grid_step_y = -(getHeight() / 2) + 336;
    
    g.setColour (Colours::white);
    
    GlyphArrangement ga;
    //g.fillRect (ga.getBoundingBox (0, ga.getNumGlyphs(), true).getSmallestIntegerContainer().expanded (4));
    
    ga.addFittedText (displayFont,
                      "Selected Note: " + String(selected_note_num_)
                      + "\nNote On Time: " + String(selected_note_on_time_)
                      + "\nNote Off Time: " + String(selected_note_off_time_),
                      (float)grid_step_x,
                      (float)grid_step_y,
                      128, 128, Justification::topLeft, 3);
    
    ga.draw (g);
}
