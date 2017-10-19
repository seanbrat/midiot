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

class NoteGridViewport : public Viewport
{
public:
    NoteGridViewport(const String& componentName = String())
    : Viewport(componentName),
    linkedViewport(NULL)
    {
    }
    
    virtual void visibleAreaChanged(const Rectangle<int>& newVisibleArea)
    {
        if (linkedViewport && isCurrentlyScrollingOnDrag())
        {
            linkedViewport->setViewPosition(this->getViewPositionX(), linkedViewport->getViewPositionY());
        }
    }
    
    void linkViewport(NoteGridViewport* viewport)
    {
        linkedViewport = viewport;
    }
    
    void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& details) override
    {
        Viewport::mouseWheelMove(e, details);
        
        if (linkedViewport)
        {
            linkedViewport->setViewPosition(this->getViewPositionX(), linkedViewport->getViewPositionY());
        }
    }
    
    
    bool autoScroll(int    mouseX,
                    int 	mouseY,
                    int 	distanceFromEdge,
                    int 	maximumSpeed)
    {
        bool retval = Viewport::autoScroll(mouseX, mouseY, distanceFromEdge, maximumSpeed);
        
        if (linkedViewport)
        {
            linkedViewport->setViewPosition(this->getViewPositionX(), linkedViewport->getViewPositionY());
        }
        
        return retval;
    }
    
    
private:
    NoteGridViewport* linkedViewport;
};


#endif /* NoteGridViewport_hpp */
