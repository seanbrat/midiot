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

#include "NoteGridProperties.hpp"

#include "MidiClockUtilities.hpp"

using std::vector;

class NoteGridViewport;
class NoteComponent;
class NoteComponentSorter;

//==============================================================================
class NoteGridComponent  :  public GraphicsComponentBase,
                            public ChangeListener,
                            public LassoSource<NoteComponent*>
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
    
    NoteGridComponent(NoteGridProperties* properties, NoteGridViewport* viewport);
    ~NoteGridComponent();
    
    void mouseMove(const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    bool mouseGridStepPosition(int &x, int &y);
    void setParentNoteGridViewport(NoteGridViewport* viewport);
    void drawComponent (Graphics& g) override;
    
    void setSelectedNote(NoteComponent *note_component);
    void clearSelectedNotes();
    
    void updateSelectedNotes();
    void updateNoteComponentBounds(NoteComponent* note_component);
    
    void grabSelectedNoteComponent(NoteComponent* selected_note_component);
    void releaseSelectedNoteComponent();
    void flushNoteRemovePool();
    
    int getNoteNum(int y);
    int getNoteOnTime(int x);
    int getNoteOffTime(int note_on_time, int width);

    bool doesNoteOverlap(MIDINote& selected_note,
                         MIDINote& check_note,
                         bool debug_print = false);
    
    void findLassoItemsInArea (Array <NoteComponent*>& results, const Rectangle<int>& area) override;
    SelectedItemSet<NoteComponent*>& getLassoSelection() override;
    void changeListenerCallback(ChangeBroadcaster*) override;

    
private:
    const MidiMessageSequence* midi_msg_seq;
    
    // row major format
    vector<vector<int>> grid_values;
    
    OwnedArray<NoteComponent> note_components;
    OwnedArray<NoteComponent> note_remove_pool;
    OwnedArray<NoteComponent> selected_note_components_;
    
    // MIDI File properties
    BarBeatTime clip_length;
    
    ComponentBoundsConstrainer* component_bounds;
    
    NoteGridViewport* grid_viewport;
    NoteGridProperties* properties_;
    
    NoteComponentSorter* note_sorter_;

    int selected_note_num_;
    int selected_note_on_time_;
    int selected_note_off_time_;
  
    MIDINote selected_note_;
    
    LassoComponent<NoteComponent*> note_lasso_;
    SelectedItemSet<NoteComponent*> selected_notes_;

};




#endif /* NoteGridComponent_hpp */
