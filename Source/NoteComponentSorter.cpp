//
//  NoteComponentSorter.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/25/17.
//
//

#include "NoteComponentSorter.hpp"
#include "NoteComponent.hpp"

int NoteComponentSorter::compareElements(NoteComponent* note_a, NoteComponent* note_b)
{
    MIDINote midi_note_a = note_a->getMidiNote();
    MIDINote midi_note_b = note_b->getMidiNote();
    
    if (midi_note_a.note_on_time_ < midi_note_b.note_on_time_)
    {
        return -1;
    }
    else if (midi_note_a.note_on_time_ == midi_note_b.note_on_time_)
    {
        if (midi_note_a.note_num_ < midi_note_b.note_num_)
        {
            return -1;
        }
        else if (midi_note_a.note_num_== midi_note_b.note_num_)
        {
            if (midi_note_a.note_off_time_ < midi_note_b.note_off_time_)
            {
                return -1;
            }
            else if (midi_note_a.note_off_time_ == midi_note_b.note_off_time_)
            {
                return 0;
            }
        }
    }
    
    return 1;
}
