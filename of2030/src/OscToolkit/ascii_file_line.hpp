//
//  ascii_file_line.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#ifndef ascii_file_line_hpp
#define ascii_file_line_hpp

#include "ofxOsc.h"

namespace OscToolkit {
    
    class AsciiFileLine {

    public: // attributes
        float timestamp;
        ofxOscMessage message;

    public: // methods
        bool parse(const string &ascii);

    public: // static methods
        static void write(const ofxOscMessage &message, float timestamp, std::ofstream &stream);
    };
}

#endif /* ascii_file_line_hpp */
