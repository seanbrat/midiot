//
//  MainTabbedComponent.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#ifndef MainTabbedComponent_hpp
#define MainTabbedComponent_hpp

#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoUtilities.h"

#include "NoteGridTabComponent.hpp"
#include "MidiInstrumentTabComponent.hpp"

#include <stdio.h>

//==============================================================================
class MainTabbedComponent  : public TabbedComponent,
private ChangeListener
{
public:
    MainTabbedComponent()
    : TabbedComponent (TabbedButtonBar::TabsAtTop)
    {
        const Colour c;
        addTab("Note Grid", c, &note_grid_console_component_, true);
        addTab("Instrument", c, &midi_instrument_console_component_, true);
        setCurrentTabIndex(1);
        updateTabColours();
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        //if (dynamic_cast<MenusDemo*> (source) != nullptr)
        updateTabColours();
    }
    

private:
    
    void updateTabColours()
    {
        bool randomiseColours = ! dynamic_cast<LookAndFeel_V4*> (&LookAndFeel::getDefaultLookAndFeel());
        for (int i = 0; i < getNumTabs(); ++i)
        {
            if (randomiseColours)
                setTabBackgroundColour (i, Colour (Random::getSystemRandom().nextFloat(), 0.1f, 0.97f, 1.0f));
            else
                setTabBackgroundColour (i, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        }
    }
    
    void lookAndFeelChanged() override
    {
        updateTabColours();
    }
    
    NoteGridTabComponent note_grid_console_component_;
    MidiInstrumentTabComponent midi_instrument_console_component_;
};

#endif /* MainTabbedComponent_hpp */
