//
//  NoteGridRulerComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/24/17.
//
//

#ifndef NoteGridRulerComponent_hpp
#define NoteGridRulerComponent_hpp

#include <stdio.h>
#include <queue>
#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

#include "GraphicsComponentBase.hpp"
#include "MidiClockUtilities.hpp"

class NoteGridViewport;
class NoteGridProperties;


class NoteGridRulerComponent : public GraphicsComponentBase
{
public:
    NoteGridRulerComponent(NoteGridProperties* properties, NoteGridViewport* viewport);

    void resized() override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void drawComponent (Graphics& g) override;
    
private:
    NoteGridViewport* ruler_viewport;
    
    // MIDI File properties
    BarBeatTime clip_length;
    
    Point<int> previous_mouse_pos;
    
    std::queue<Point<int>> previous_mouse_positions;
    short previous_mouse_queue_size;
    
    NoteGridProperties* properties_;
    
};



#endif /* NoteGridRulerComponent_hpp */
