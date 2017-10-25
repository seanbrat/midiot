//
//  NoteComponentSorter.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/25/17.
//
//

#ifndef NoteComponentSorter_hpp
#define NoteComponentSorter_hpp

#include <stdio.h>

class NoteComponent;

class NoteComponentSorter
{
public:
    NoteComponentSorter() {};
    ~NoteComponentSorter() {};
    
    static int compareElements(NoteComponent* note_a, NoteComponent* note_b);
};


#endif /* NoteComponentSorter_hpp */
