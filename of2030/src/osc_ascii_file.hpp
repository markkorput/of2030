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

namespace of2030{
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
        // void setLoop(bool _loop){ loop = _loop; }
        const OscAsciiLine* next_line();

    private: //attributes
        // bool loop;
        OscAsciiLine last_line;
        std::ifstream *file;
    };

}
#endif /* osc_ascii_file_hpp */
