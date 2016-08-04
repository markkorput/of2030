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
}

bool OscPlayback::update(float dt){
    time += dt;

    // this is the 'retry-loop' for when the end of the file is reach,
    // only perform this loop twice every iteration so we won't get stuck in and endless
    // loop when the file is empty
    for(int loopCount = 0; loopCount<2; loopCount++){
        if(pending_line){
            if(time < pending_line->timestamp){
                return true; // true means "there is more to come"
            }

            ofNotifyEvent(messageEvent, pending_line->message, this);
            pending_line = NULL;
        }

        // keep reading an processing lines until
        // 1) there are no lines left
        // 2) we reach a line that has a timestamp that is bigger than our timer
        OscAsciiLine *line;
        while((line = file->next_line())){
            if(time >= line->timestamp){
                // process line
                ofNotifyEvent(messageEvent, line->message, this);
            } else {
                // wait until it's this line's time
                pending_line = line;
                return true; // true means "there is more to come"
            }
        }

        if(!bLoop){
            // this playback is done
            return false;
        }

        // time to loop; reset file, notify listeners and try again
        ofLog() << "OscPlayback LOOP " << file->getReadPath();
        file->reset();
        ofNotifyEvent(loopEvent, *this, this);
        start();
    }

    ofLogWarning() << "OscPlayback::update loopCount > 1. Empty file? " << file->getReadPath();
    return false;
}
