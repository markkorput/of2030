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

    // reading
    bool load(string path);
    OscAsciiLine* next_line();
    inline const string &getReadPath(){ return readpath; }
    inline void reset(){
        load(readpath);
    }


    // writing
    void start_writing(string path);
    void stop_writing();
    void write_line(ofxOscMessage &msg);
    void write_line(const ofxOscMessage &msg, float timestamp);

    inline void setLoop(bool loop){ bLoop = loop; }

private:
    OscAsciiLine* read_line();
    
private: // reading attributes
    OscAsciiLine last_line;
    std::ifstream infile;
    bool bLoop;
    string readpath;

private: // writing attributes
    float start_time, first_message_time;
    std::ofstream outfile;
    
};

#endif /* osc_ascii_file_hpp */
