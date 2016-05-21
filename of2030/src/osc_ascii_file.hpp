//
//  osc_ascii_file.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#ifndef osc_ascii_file_hpp
#define osc_ascii_file_hpp

#include <stdio.h>
#include "ofxOsc.h"

typedef struct {
    float timestamp;
    ofxOscMessage message;
} OscAsciiLine;

class OscAsciiFile {
public: // methods
    OscAsciiFile();
    ~OscAsciiFile(){ destroy(); }
    void destroy();

    void load(string path);
    OscAsciiLine* next_line();

private: //attributes
    OscAsciiLine last_line;
    std::ifstream *file;
};

#endif /* osc_ascii_file_hpp */
