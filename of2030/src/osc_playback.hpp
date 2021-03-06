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
    OscPlayback(OscAsciiFile &_file){
        bLoop = false;
        file = &_file;
    }
    
    void start();
    // returns true if not ended yet
    bool update(float dt);

    inline OscAsciiFile* getFile() const { return file; }

public: // events
    ofEvent<ofxOscMessage> messageEvent;
    ofEvent<OscPlayback> loopEvent;

private: // attributes
    bool bLoop;
    OscAsciiLine *pending_line;
    float time;
    OscAsciiFile *file;
};

#endif /* osc_playback_hpp */
