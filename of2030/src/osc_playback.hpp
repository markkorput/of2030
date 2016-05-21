//
//  osc_playback.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#ifndef osc_playback_hpp
#define osc_playback_hpp

#include "ofMain.h"
#include "osc_ascii_file.hpp"

class OscPlayback {
public:
    OscPlayback(OscAsciiFile &_file){ file = &_file; }
    
    void start();
    // returns true if not ended yet
    bool update();

public: // events
    ofEvent<ofxOscMessage> messageEvent;

private:
    OscAsciiLine *pending_line;
    float time, last_update_time;
    OscAsciiFile *file;
};

#endif /* osc_playback_hpp */
