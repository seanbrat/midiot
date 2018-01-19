//
//  MidiotFileUtils.hpp
//  Midiot
//
//  Created by Sean Bratnober on 1/19/18.
//
//

#ifndef MidiotFileUtils_hpp
#define MidiotFileUtils_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class MidiotFileUtils {
public:
    static const String getMidiotDataFolderPath();
    static const String getMidiotPatchFolderPath();
    static const String getInstrumentPatchFolderPath(const String manufacturer,
                                                     const String model_name);

    static File getInstrumentPatchFolder(const String manufacturer,
                                  const String model_name);
    
    static String getPatchFileExtension() { return String(".mdp"); }
    static String generatePatchFileName(const String manufacturer_name, const String model_name);
};

#endif /* MidiotFileUtils_hpp */
