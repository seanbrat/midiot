#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoUtilities.h"

#include "MainTabbedComponent.hpp"

#include <vector>


using std::vector;
using std::pair;


class MainContentComponent  : public Component,
private Button::Listener,
private ComboBox::Listener
{
public:
    MainContentComponent()
    : main_tabbed_component_()
    {
        setOpaque(true);
        
        addAndMakeVisible(main_tabbed_component_);

        setSize(1200, 1000);
    }
    
    ~MainContentComponent()
    {

    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
    }
    
    void resized() override
    {
        Rectangle<int> area (getLocalBounds());
        main_tabbed_component_.setBounds(area);
    }
    
    
    void buttonClicked (Button* button) override
    {
    }
    
    void comboBoxChanged (ComboBox* box) override
    {
    }
    
    //==============================================================================
    
    MainTabbedComponent main_tabbed_component_;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent);
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
