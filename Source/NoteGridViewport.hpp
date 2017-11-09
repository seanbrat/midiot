//
//  NoteGridViewport.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/19/17.
//
//

#ifndef NoteGridViewport_hpp
#define NoteGridViewport_hpp

#include <stdio.h>

#include "../JuceLibraryCode/JuceHeader.h"

class NoteGridComponent;

class NoteGridViewport : public Viewport
{
public:
    NoteGridViewport(const String& componentName = String());
    ~NoteGridViewport();
    
    virtual void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override;
    void linkViewport(NoteGridViewport* viewport);
    void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& details) override;
    
    bool autoScroll(int mouseX,
                    int mouseY,
                    int distanceFromEdge,
                    int maximumSpeed);
    
    void setNoteGrid(NoteGridComponent* note_grid) { note_grid_ = note_grid; };
    
private:
    NoteGridViewport* linkedViewport;
    NoteGridComponent* note_grid_;
};


#endif /* NoteGridViewport_hpp */
