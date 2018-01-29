//
//  MidiInstrumentControllerComponent.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/16/17.
//
//

#include "MidiInstrumentControllerComponent.hpp"

#include "NoteGridComponent.hpp"
#include "NoteGridViewport.hpp"
#include "NoteGridRulerComponent.hpp"
#include "MidiStudio.hpp"
#include "MidiInstrument.hpp"
#include "MidiotFileUtils.hpp"


void MidiControlTab::drawComponent (Graphics& g)
{
    //resized();
}

MidiControlSlider::MidiControlSlider()
: Slider()
{
    setSliderStyle(LinearVertical);
}

void MidiControlSlider::setName(const String& newName)
{
    Component::setName(newName);
    control_label_.setText(newName, dontSendNotification);
    control_label_.attachToComponent(this, false);
}

MidiInstrumentControllerComponent::MidiInstrumentControllerComponent ()
: GraphicsComponentBase("MidiInstrumentControllerComponent"),
keyboard_component_(keyboard_state_, MidiKeyboardComponent::horizontalKeyboard),
control_slider_tabs_(),
patch_selector_menu_("Patch Selector Combo"),
patch_name_label_("Patch Name", "Patch Name"),
midi_instrument_properties_()
{
    addAndMakeVisible(keyboard_component_);
    keyboard_state_.addListener(this);

#if USE_MIDI_COMPONENT
    addAndMakeVisible(control_slider_tabs_);
#endif
    addAndMakeVisible(patch_selector_menu_);
    patch_selector_menu_.setJustificationType(Justification::topLeft);
    patch_selector_menu_.addListener(this);
    
    addAndMakeVisible(patch_name_label_);
    patch_name_label_.setEditable(true);
    patch_name_label_.addListener(this);
    
    addAndMakeVisible(patch_request_button_);
    patch_request_button_.setButtonText("Patch Request");
    patch_request_button_.addListener(this);
    
    addAndMakeVisible(patch_save_button_);
    patch_save_button_.setButtonText("Save Patch");
    patch_save_button_.addListener(this);
}

MidiInstrumentControllerComponent::~MidiInstrumentControllerComponent()
{

}

void MidiInstrumentControllerComponent::addMidiInstrument(MidiInstrument* midi_instrument)
{
    midi_instrument_ = midi_instrument;
}

void MidiInstrumentControllerComponent::addMidiKeyboardStateListener(MidiKeyboardStateListener* const listener)
{
    keyboard_state_.addListener(listener);
}

void MidiInstrumentControllerComponent::resized()
{
    keyboard_component_.setBounds(0, 0, 1000, 80);
    
    int x_pos;
    int y_pos;
    
#if USE_MIDI_COMPONENT
    control_slider_tabs_.setBounds(0, 100, 1100, 500);
    //midi_control_tab_.setBounds(0, 80, 1000, 140);
    x_pos = 0;
    y_pos = 600;
    //control_slider_tabs_.resized();
#else
    int slider_x = 0;
    int slider_y = 100;
    int slider_width = 100;
    int slider_height = 140;
    
    x_pos = slider_x;
    y_pos = slider_y;
    
    for (int i=0; i<control_sliders_.size(); i++)
    {
        if (x_pos + slider_width >= 1000)
        {
            x_pos = 0;
            y_pos += slider_height + 30;
        }
        
        control_sliders_[i]->setBounds(x_pos, y_pos, slider_width, slider_height);
        x_pos += slider_width;
    }
    
    x_pos = 0;
    y_pos += slider_height + 30;
#endif
    patch_selector_menu_.setBounds(x_pos, y_pos, 200, 40);
    x_pos += 220;
    patch_name_label_.setBounds(x_pos, y_pos, 200, 40);
    x_pos = 0;
    y_pos += 40;
    patch_request_button_.setBounds(x_pos, y_pos, 100, 40);
    x_pos += 110;
    patch_save_button_.setBounds(x_pos, y_pos, 100, 40);
}


void MidiInstrumentControllerComponent::mouseDrag(const MouseEvent& e)
{
    //grid_viewport.autoScroll(e.x, e.y, 20, 1);
    //printf("mouseDrag called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void MidiInstrumentControllerComponent::mouseMove(const MouseEvent& e)
{
    repaint();
    //printf("mouseMove called with e.x: %d and e.y: %d\n", e.x, e.y);
}

void MidiInstrumentControllerComponent::mouseUp (const MouseEvent& e)
{
    repaint();
}

bool MidiInstrumentControllerComponent::mouseGridStepPosition(int &x, int &y)
{
    return true;
}

void MidiInstrumentControllerComponent::drawComponent (Graphics& g)
{
}

void MidiInstrumentControllerComponent::buttonClicked (Button* button)
{
    if (button == &patch_request_button_)
    {
        midi_instrument_->sendSysexPatchDumpMessage();
    }
    else if (button == &patch_save_button_)
    {
        savePatch();
    }
}

void MidiInstrumentControllerComponent::labelTextChanged (Label *labelThatHasChanged)
{
    if (labelThatHasChanged == &patch_name_label_)
    {
        midi_instrument_properties_.set_patch_name(patch_name_label_.getText());
        patch_selector_menu_.removeListener(this);
        patch_selector_menu_.setSelectedId(0);
        patch_selector_menu_.addListener(this);
    }
}

void MidiInstrumentControllerComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &patch_selector_menu_)
    {
        String selected_patch_name(patch_selector_menu_.getText());
        if (selected_patch_name.length())
        {
            loadSelectedPatch(selected_patch_name);
        }
    }
}

void MidiInstrumentControllerComponent::loadSelectedPatch(String selected_patch_name)
{
    midi_instrument_properties_.set_patch_name(selected_patch_name);
    patch_name_label_.setText(selected_patch_name, NotificationType::dontSendNotification);
    
    String patch_file_name = selected_patch_name + String(".mdp");
    printf("loading patch file: %s\n", patch_file_name.toRawUTF8());
    String manufacturer_name(midi_instrument_->getManufacturerName());
    String model_name(midi_instrument_->getModelName());
    
    String patch_file_path(MidiotFileUtils::getInstrumentPatchFolderPath(manufacturer_name, model_name));
    patch_file_path += patch_file_name;
    printf("--> at path: %s\n", patch_file_path.toRawUTF8());
    
    File patch_file(patch_file_path);
    if (!patch_file.existsAsFile()) {
        return;
    }
    
    FileInputStream patch_file_stream(patch_file);
    String patch_json_string(patch_file_stream.readEntireStreamAsString());
    
    printf("patch_json_string: %s\n", patch_json_string.toRawUTF8());
    var patch_json(JSON::fromString(patch_json_string));
    
    if (auto* const patch_obj = patch_json.getDynamicObject())
    {
        if (patch_obj->hasProperty("manufacturer") &&
            patch_obj->hasProperty("model_name") &&
            patch_obj->hasProperty("patch_name") &&
            patch_obj->hasProperty("params"))
        {
            printf("VALIDATED patch file\n");
            
            var manufacturer_json = patch_obj->getProperty("manufacturer");
            
            String patch_name = patch_json["patch_name"];
            printf("patch_name: %s\n", patch_name.toRawUTF8());
            
            if (patch_json["manufacturer"] == manufacturer_name &&
                patch_json["model_name"] == model_name)
            {
                printf("validated manufacturer/model\n");
                
                var param_json = patch_json["params"];
                if (auto* const param_obj = param_json.getDynamicObject())
                {
                    for (auto prop : param_obj->getProperties())
                    {
                        String property_name = prop.name.toString();
                        int property_value = param_json[property_name.toRawUTF8()];
                        
                        printf("name: %s\t\tvalue: %d\n", property_name.toRawUTF8(), property_value);
                        
                        midi_instrument_->updateMidiControl(property_name,
                                                            property_value);
                    }
                    
                    midi_instrument_->sendMidiControlPatchData();
                }
            }
        }
    }
}

void MidiInstrumentControllerComponent::handleNoteOn(
                                            MidiKeyboardState* keyboard_state,
                                            int midi_channel,
                                            int midi_note_number,
                                            float velocity)
{
    printf("MidiInstrumentControllerComponent::handleNoteOn() with midi_channel: %d\n", midi_channel);
}

void MidiInstrumentControllerComponent::handleNoteOff(
                                            MidiKeyboardState* keyboard_state,
                                            int midi_channel,
                                            int midi_note_number,
                                            float velocity)
{
}

void MidiInstrumentControllerComponent::savePatch()
{
    String manufacturer_name = midi_instrument_->getManufacturerName();
    String model_name = midi_instrument_->getModelName();
    String patch_name = midi_instrument_properties_.patch_name();
    
    if (patch_name == "")
    {
        patch_name = MidiotFileUtils::generatePatchFileName(manufacturer_name, model_name);
        midi_instrument_properties_.set_patch_name(patch_name);
        patch_name_label_.setText(patch_name, NotificationType::dontSendNotification);
    }
    
    var patch = midi_instrument_->getPatchVar(patch_name);
    String patch_json = JSON::toString(patch);
    printf("patch_json: %s\n", patch_json.toRawUTF8());
    File inst_patch_folder = MidiotFileUtils::getInstrumentPatchFolder(manufacturer_name, model_name);
    
    String patch_file_path = inst_patch_folder.getFullPathName() + File::separatorString + patch_name + MidiotFileUtils::getPatchFileExtension();
    
    File patch_file(patch_file_path);
    patch_file.replaceWithText(patch_json);
    
    updatePatchSelectorMenu(patch_name);
}

void MidiInstrumentControllerComponent::setSelectedPatchByName(String patch_name, bool loadPatch)
{
    patch_selector_menu_.removeListener(this);

    for (int i = 0; i < patch_selector_menu_.getNumItems(); i++)
    {
        String item_text = patch_selector_menu_.getItemText(i);
        if (item_text == patch_name)
        {
            midi_instrument_properties_.set_patch_name(patch_name);
            patch_selector_menu_.setSelectedId(i+1, NotificationType::dontSendNotification);
            break;
        }
    }
    
    patch_selector_menu_.addListener(this);

}

void MidiInstrumentControllerComponent::updatePatchSelectorMenu(String selected_patch_name)
{
    String manufacturer_name = midi_instrument_->getManufacturerName();
    String model_name = midi_instrument_->getModelName();
    File patch_folder(MidiotFileUtils::getInstrumentPatchFolder(manufacturer_name, model_name));
    
    patch_selector_menu_.removeListener(this);
    patch_selector_menu_.clear();
    
    Array<File> patch_files;
    patch_folder.findChildFiles(patch_files, File::findFiles, true, "*.mdp");
    for (int i=0; i<patch_files.size(); i++)
    {
        String patch_file_name(patch_files[i].getFileNameWithoutExtension());
        patch_selector_menu_.addItem(patch_file_name, i+1);
    }
    
    if (selected_patch_name.length())
    {
        setSelectedPatchByName(selected_patch_name);
    }
    
    patch_selector_menu_.addListener(this);
}


MidiControlSlider* MidiInstrumentControllerComponent::addMidiControlSlider(MidiControl* midi_control)
{
    MidiControlSlider* control_slider = new MidiControlSlider();
    control_slider->setName(midi_control->name());
    control_slider->setRange(midi_control->getRangeMinimum(),
                             midi_control->getRangeMaximum(),
                             1.0);
    control_slider->addListener(midi_control);
    control_sliders_.add(control_slider);
#if USE_MIDI_COMPONENT
    control_slider_tabs_.addAndMakeVisible(control_slider);
//    midi_control_tab_.addMidiControlAndMakeVisible(control_slider);
#else
    addAndMakeVisible(control_slider);
#endif
    
    return control_slider;
}

void MidiInstrumentControllerComponent::processNextKeyboardMidiEvent(const MidiMessage& message)
{
    keyboard_state_.processNextMidiEvent (message);
}


void MidiInstrumentControllerComponent::setKeyboardMidiOutputChannel(short output_channel)
{
    keyboard_component_.setMidiChannel(output_channel);
}
