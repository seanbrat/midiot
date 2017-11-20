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
    
    bool isNoteSelected(NoteComponent* note_component);
    void setSelectedNote(NoteComponent* note_component);
    void initSelectedNotes();
    void clearSelectedNotes();
    
    
    
    void dragSelectedNotes(const MouseEvent& e,
                           NoteComponent* dragged_note,
                           int y_drag_compensation);
    void doDragSelectedNotes();

    void resizeSelectedNotes(const MouseEvent& e,
                           NoteComponent* resize_note,
                           int y_resize_compensation);
    void doResizeSelectedNotes();

    void startDrag();
    void endDrag();
    
    void setResizeDownWidth(int width) { resize_down_width_ = width; };
    
    void updateSelectedNotes();
    void updateNoteComponentBounds(NoteComponent* note_component);
    
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
    
    NoteComponent* dragged_note_;
    int drag_x_distance_;
    int drag_y_distance_;
    int drag_y_compensation_;
    int note_grid_viewpos_x_;

    NoteComponent* resized_note_;
    int resize_x_distance_;
    int resize_y_distance_;
    int resize_y_compensation_;
    int resize_grid_viewpos_x_;
    int resize_down_width_;
};




#endif /* NoteGridComponent_hpp */
