//
//  NoteGridEditorComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#ifndef NoteGridEditorComponent_hpp
#define NoteGridEditorComponent_hpp

#include <stdio.h>


#include <queue>
#include <vector>

#include "GraphicsComponentBase.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "NoteGridProperties.hpp"
#include "MidiClockUtilities.hpp"

using std::vector;

class NoteGridComponent;
class NoteGridViewport;
class NoteGridRulerComponent;

//==============================================================================
class NoteGridEditorComponent  : public GraphicsComponentBase
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
    
    
    NoteGridEditorComponent ();
    ~NoteGridEditorComponent();
    
    void resized() override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    bool mouseGridStepPosition(int &x, int &y);
    void drawComponent (Graphics& g) override;

private:
    // MIDI File properties
    BarBeatTime clip_length;
    
    
    ComponentBoundsConstrainer* component_bounds;
    
    NoteGridProperties properties_;
    
    NoteGridComponent* note_grid;
    NoteGridRulerComponent* grid_ruler;
    
    NoteGridViewport* grid_viewport;
    NoteGridViewport* ruler_viewport;
};





#endif /* NoteGridEditorComponent_hpp */
