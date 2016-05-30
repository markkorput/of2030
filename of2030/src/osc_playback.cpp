//
//  osc_playback.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#include "osc_playback.hpp"

void OscPlayback::start(){
    time = 0.0f;
    pending_line = NULL;
    last_update_time = ofGetElapsedTimef();
}

bool OscPlayback::update(int recursion_count){
    if(recursion_count > 2){
        ofLogWarning() << "[OscPlayback::update] recursion count > 2";
        return false;
    }

    // update internal timer
    float t = ofGetElapsedTimef();
    time += t - last_update_time;
    last_update_time = t;

    if(pending_line){
        if(time < pending_line->timestamp){
            return true; // true means "there is more to come"
        }

        ofNotifyEvent(messageEvent, pending_line->message, this);
    }

    // keep reading lines until we either
    // 1) reach the end of the file
    // 2) reach a line that has a timestamp that is bigger than our timer
    OscAsciiLine *line;
    while((line = file->next_line())){
        if(time >= line->timestamp){
            ofNotifyEvent(messageEvent, line->message, this);
        } else {
            pending_line = line;
            return true; // true means "there is more to come"
        }
    }

    if(bLoop){
        ofLog() << "OscPlayback LOOP";
        file->reset();
        ofNotifyEvent(loopEvent, *this, this);
        start();
        return update(recursion_count+1);
    }

    return false;
}
