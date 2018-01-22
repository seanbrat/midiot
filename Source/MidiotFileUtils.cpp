//
//  MidiotFileUtils.cpp
//  Midiot
//
//  Created by Sean Bratnober on 1/19/18.
//
//

#include "MidiotFileUtils.hpp"


const String MidiotFileUtils::getMidiotDataFolderPath()
{
    String user_app_data_dir = File::getSpecialLocation(File::SpecialLocationType::userApplicationDataDirectory).getFullPathName();
    
    return  user_app_data_dir +
            File::separatorString +
            String("Midiot") +
            File::separatorString;
}

const String MidiotFileUtils::getMidiotPatchFolderPath()
{
    return  getMidiotDataFolderPath() +
            String("Patches") +
            File::separatorString;
}

const String MidiotFileUtils::getInstrumentPatchFolderPath(const String manufacturer,
                                                 const String model_name)
{
    return  getMidiotPatchFolderPath() +
            manufacturer +
            File::separatorString +
            model_name +
            File::separatorString;
}

File MidiotFileUtils::getInstrumentPatchFolder(const String manufacturer,
                              const String model_name)
{
    String inst_patch_folder_path = getInstrumentPatchFolderPath(manufacturer,
                                                                 model_name);
    File inst_patch_folder(inst_patch_folder_path);

    if (!inst_patch_folder.exists())
    {
        inst_patch_folder.createDirectory();
    }
    
    return inst_patch_folder;
}

String MidiotFileUtils::generatePatchFileName(const String manufacturer_name, const String model_name)
{
    Time current_time = Time::getCurrentTime();
    String patch_name = manufacturer_name + " " + model_name + " ";
    patch_name += String(current_time.getYear()) + String(current_time.getMonth()) + String(current_time.getDayOfMonth()) + "_";
    patch_name += String(current_time.getHours()) + "-" + String(current_time.getMinutes()) + "-" + String(current_time.getSeconds());

    return patch_name;
}
