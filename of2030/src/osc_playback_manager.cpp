//
//  osc_playback_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#include "osc_playback_manager.hpp"
#include "osc_receiver.hpp"

using namespace of2030;

SINGLETON_CLASS_IMPLEMENTATION_CODE(OscPlaybackManager)


void OscPlaybackManager::start(string name){
    // load file
    OscAsciiFile* file = new OscAsciiFile();
    file->load(nameToPath(name));
    // start playback
    OscPlayback* playback = new OscPlayback(*file);
    playback->start();
    // save it
    add(*playback);
}

string OscPlaybackManager::nameToPath(const string &name){
    string p = ofToDataPath("osc/"+name);
    if(ofFile::doesFileExist(p))
        return p;

    if(ofFile::doesFileExist(name))
        return name;
}

void OscPlaybackManager::update(){
    for(auto playback: playbacks){
        // update
        if(!playback->update()){
            // done?
            remove(playback);
        }
    }
}

void OscPlaybackManager::add(OscPlayback &playback){
    // save for continued use
    playbacks.push_back(&playback);
    // register callback
    ofAddListener(playback.messageEvent, this, &OscPlaybackManager::onMessage);
}

bool OscPlaybackManager::remove(OscPlayback *playback){
    for(int i=playbacks.size()-1; i>=0; i--){
        if(playbacks[i] == playback){
            // found it! remove from list
            playbacks.erase(playbacks.begin()+i);
            // unregister from event
            ofRemoveListener(playback->messageEvent, this, &OscPlaybackManager::onMessage);
            // delete
            delete playback;
            return true;
        }
    }
    // not found
    return false;
}

void OscPlaybackManager::clear(){
    for(auto playback: playbacks){
        remove(playback);
    }
}

void OscPlaybackManager::onMessage(ofxOscMessage &message){
    of2030::OscReceiver::instance()->processMessage(message);
}