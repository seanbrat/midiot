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

void NoteGridComponent::clearSelectedNotes()
{
    NoteComponent** selected_note_iter;
    for (selected_note_iter = selected_notes_.begin();
         selected_note_iter != selected_notes_.end();
         selected_note_iter++)
    {
        NoteComponent* selected_note = *selected_note_iter;
        selected_note->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);
        
    }
    selected_notes_.deselectAll();
}


void NoteGridComponent::mouseMove(const MouseEvent& e)
{
    repaint();
}

void NoteGridComponent::mouseUp (const MouseEvent& e)
{
    note_lasso_.endLasso();
    removeChildComponent(&note_lasso_);
    
    if (! (e.mouseWasDraggedSinceMouseDown() || e.mods.isAnyModifierKeyDown()))
    {
        NoteComponent** selected_note_iter;
        for (selected_note_iter = selected_notes_.begin();
             selected_note_iter != selected_notes_.end();
             selected_note_iter++)
        {
            NoteComponent* selected_note = *selected_note_iter;
            selected_note->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::firebrick);

        }
        selected_notes_.deselectAll();
    }

    repaint();
}

void NoteGridComponent::setSelectedNote(NoteComponent *note_component)
{
    clearSelectedNotes();
    selected_notes_.addToSelection(note_component);
    note_component->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::lightblue);

}


void NoteGridComponent::mouseDown (const MouseEvent& e)
{
    clearSelectedNotes();
    
    addChildComponent(note_lasso_);
    note_lasso_.setAlpha(0.5);
    note_lasso_.beginLasso(e, this);
}

void NoteGridComponent::mouseDrag (const MouseEvent& e)
{
    note_lasso_.toFront(false);
    note_lasso_.dragLasso(e);
}

void NoteGridComponent::changeListenerCallback(ChangeBroadcaster*)
{
    repaint();
}

void NoteGridComponent::findLassoItemsInArea (Array <NoteComponent*>& results, const Rectangle<int>& area)
{
    const Rectangle<int> lasso (area);// - subCompHolder->getPosition());
    
    for (int i=0; i<note_components.size(); i++)
    {
        NoteComponent* note = note_components[i];
        
        if (note->getBounds().intersects(lasso))
        {
            results.add(note);
        }
    }
}

SelectedItemSet<NoteComponent*>& NoteGridComponent::getLassoSelection()
{
    return selected_notes_;
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
    return -round((y / properties_->step_height_) - num_midi_notes_ + 1);
}

int NoteGridComponent::getNoteOnTime(int x)
{
    return round(x / properties_->tick_to_pixel_x_factor_);
}

int NoteGridComponent::getNoteOffTime(int note_on_time, int width)
{
    return round(width / properties_->tick_to_pixel_x_factor_) + note_on_time;
}

bool NoteGridComponent::doesNoteOverlap(MIDINote& selected_note,
                                        MIDINote& check_note,
                                        bool debug_print)
{
    if (false)//debug_print)
    {
        printf("\ndoesNoteOverlap comparing selected_note: %d\tat note_on_time: %d\twith note_off_time: %d\n", selected_note.note_num_, selected_note.note_on_time_, selected_note.note_off_time_);
        printf("with check_note: %d\tat note_on_time: %d\twith note_off_time: %d\n", check_note.note_num_, check_note.note_on_time_, check_note.note_off_time_);
    }
    
    if (selected_note.note_num_ == check_note.note_num_)
    {
        if ((selected_note.note_on_time_ >= check_note.note_on_time_
             && selected_note.note_on_time_ < check_note.note_off_time_)
            || (selected_note.note_off_time_ > check_note.note_on_time_
                && selected_note.note_off_time_ < check_note.note_off_time_)
            || (selected_note.note_on_time_ < check_note.note_on_time_
                && selected_note.note_off_time_ >= check_note.note_off_time_))
        {
            if (debug_print)
            {
                if (debug_print)
                {
                    printf("\ndoesNoteOverlap comparing selected_note: %d\tat note_on_time: %d\twith note_off_time: %d\n", selected_note.note_num_, selected_note.note_on_time_, selected_note.note_off_time_);
                    printf("with check_note: %d\tat note_on_time: %d\twith note_off_time: %d\n", check_note.note_num_, check_note.note_on_time_, check_note.note_off_time_);
                }
                
                printf("returning true\n");
            }
            return true;
        }
    }
    
    if (debug_print)
    {
        //printf("returning false\n");
    }
    
    return false;
}

#define USE_ORIGINAL_SELECTED   0

void NoteGridComponent::updateSelectedNotes()
{
    
    Rectangle<int> grid_bounds;
    NoteComponent* selected_note_component;
    
    selected_note_on_time_ = getNoteOnTime(grid_bounds.getX());
    selected_note_num_ = getNoteNum(grid_bounds.getY());
    selected_note_off_time_ = getNoteOffTime(selected_note_on_time_, grid_bounds.getWidth());

#if USE_ORIGINAL_SELECTED
    for (int selected_note_index=0;
         selected_note_index<selected_note_components_.size();
         selected_note_index++)
    {
        selected_note_component = selected_note_components_[selected_note_index];
        
#else
    NoteComponent** selected_note_iter;
    for (selected_note_iter=selected_notes_.begin();
         selected_note_iter!=selected_notes_.end();
         selected_note_iter++)
    {
        selected_note_component = *selected_note_iter;
#endif
        MIDINote selected_note = selected_note_component->getMidiNote();
        Rectangle<int> selected_note_bounds = selected_note_component->getBoundsInParent();
        selected_note_num_ = getNoteNum(selected_note_bounds.getY());
        selected_note_on_time_ = getNoteOnTime(selected_note_bounds.getX());
        selected_note_off_time_ = getNoteOffTime(selected_note_on_time_,
                                                 selected_note_bounds.getWidth());

        //printf("selected_note_num_: %d selected_note_on_time_: %d selected_note_off_time_: %d\n", selected_note_num_, selected_note_on_time_, selected_note_off_time_);
        selected_note.note_num_ = getNoteNum(selected_note_bounds.getY());
        selected_note.note_on_time_ = getNoteOnTime(selected_note_bounds.getX());
        selected_note.note_off_time_ = getNoteOffTime(selected_note_on_time_,
                                                 selected_note_bounds.getWidth());
        
        selected_note_component->setMidiNote(selected_note);
        
        for (int overlap_note_index=0;
             overlap_note_index<note_components.size();
             overlap_note_index++)
        {
 
            NoteComponent* overlap_note_component = note_components[overlap_note_index];
#if !USE_ORIGINAL_SELECTED
            if (selected_note_component == overlap_note_component)
            {
                continue;
            }
#endif
            
            MIDINote overlap_note = overlap_note_component->getMidiNote();
            
            if (overlap_note.note_num_ == selected_note.note_num_)
            {
                //printf("overlap_note.note_num_: %d overlap_note.note_on_time_: %d overlap_note.note_off_time_: %d\n", overlap_note.note_num_, overlap_note.note_on_time_, overlap_note.note_off_time_);
            }
            
            if (doesNoteOverlap(selected_note, overlap_note))
            {
                printf("found OVERLAP!\n");
                overlap_note_component->setVisible(false);
                note_components.remove(overlap_note_index, false);
                note_remove_pool.addSorted(*note_sorter_, overlap_note_component);
            }
        }
    }
    
    for (int restore_index=0;
         restore_index<note_remove_pool.size();
         restore_index++)
    {
        NoteComponent* restore_note_component = note_remove_pool[restore_index];
        MIDINote restore_note = restore_note_component->getMidiNote();
        
        bool should_restore_note = true;
        
#if USE_ORIGINAL_SELECTED
        for (int check_note_index=0;
             check_note_index<selected_note_components_.size();
             check_note_index++)
        {
            NoteComponent* check_note_component = selected_note_components_[check_note_index];
#else
        NoteComponent** selected_note_iter;
        for (selected_note_iter=selected_notes_.begin();
             selected_note_iter!=selected_notes_.end();
             selected_note_iter++)
        {
            NoteComponent* check_note_component = *selected_note_iter;
#endif
            MIDINote check_note = check_note_component->getMidiNote();
            
            if (doesNoteOverlap(restore_note, check_note, false))
            {
                should_restore_note = false;
            }
        }
        
        if (should_restore_note)
        {
            restore_note_component->setVisible(true);
            note_remove_pool.remove(restore_index, false);
            note_components.addSorted(*note_sorter_,
                                      restore_note_component);
        }
    }
    
    repaint();
}


void NoteGridComponent::grabSelectedNoteComponent(NoteComponent* selected_note_component)
{
#if !USE_ORIGINAL_SELECTED
    return;
#endif
    int selected_note_index = note_components.indexOf(selected_note_component);
    note_components.remove(selected_note_index, false);
    selected_note_components_.add(selected_note_component);
}

void NoteGridComponent::releaseSelectedNoteComponent()
{
#if !USE_ORIGINAL_SELECTED
    return;
#endif
    for (int i=0; i<selected_note_components_.size(); i++)
    {
        NoteComponent* restore_selected_note = selected_note_components_.removeAndReturn(i);
        note_components.addSorted(*note_sorter_, restore_selected_note);
    }
}

void NoteGridComponent::flushNoteRemovePool()
{
    note_remove_pool.clear();
}

void NoteGridComponent::updateNoteComponentBounds(NoteComponent* note_component)
{
    if (!note_component)
    {
        return;
    }
    
    MIDINote note = note_component->getMidiNote();
    int num_notes = 128;
    int note_height = properties_->step_height_;
    int note_width = (note.note_off_time_ - note.note_on_time_) * properties_->tick_to_pixel_x_factor_;
    int note_pos_x = note.note_on_time_ * properties_->tick_to_pixel_x_factor_;
    int note_pos_y = (properties_->step_height_)*(num_notes - note.note_num_ - 1);
    
    note_component->setBounds(note_pos_x, note_pos_y, note_width, note_height);

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
        for (int note_index=0; note_index<note_components.size(); note_index++)
        {
            updateNoteComponentBounds(note_components[note_index]);
        }

        for (int note_index=0; note_index<selected_note_components_.size(); note_index++)
        {
            updateNoteComponentBounds(selected_note_components_[note_index]);
        }
     
        properties_->init_grid_ = false;
    }
    
    NoteComponent** selected_note_iter;
    for (selected_note_iter = selected_notes_.begin();
         selected_note_iter != selected_notes_.end();
         selected_note_iter++)
    {
        NoteComponent* selected_note = *selected_note_iter;
        selected_note->setColour(NoteComponent::TextButton::ColourIds::buttonColourId, Colours::lightblue);
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
