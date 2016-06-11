//
//  osc_playback_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#include "osc_playback_manager.hpp"
#include "osc_receiver.hpp"
#include "osc_sender.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(OscPlaybackManager)


bool OscPlaybackManager::start(const string &name){
    // load file
    OscAsciiFile* file = new OscAsciiFile();
    if(!file){
        return false;
    }

    string path = nameToPath(name);
    if(!file->load(path)){
        ofLog() << "could not load osc file: " << path;
        delete file;
        return false;
    }

    // start playback
    OscPlayback* playback = new OscPlayback(*file);

    if(!playback){
        delete file;
        return false;
    }

    playback->start();
    // save it
    add(*playback);

    // ofLogVerbose() << "Started playback of: " << file->getReadPath();
    return true;
}

bool OscPlaybackManager::stop(const string &name){
    if(name == ""){ // stop all
        for(int i=playbacks.size()-1; i>=0; i--){
            remove(playbacks[i]);
        }

        playbacks.clear();
        return true;
    }

    OscPlayback *playback = getPlayback(name);
    if(!playback) return false;
    remove(playback);
    return true;
}

OscPlayback* OscPlaybackManager::getPlayback(const string &name){
    string p = nameToPath(name);

    for(int i=playbacks.size()-1; i>=0; i--){
        if(playbacks[i]->getFile()->getReadPath() == p){
            return playbacks[i];
        }
    }
    return NULL;
}

string OscPlaybackManager::nameToPath(const string &name){
    string p = ofToDataPath("osc/"+name+".csv");
    if(ofFile::doesFileExist(p))
        return p;

    p = ofToDataPath("osc/"+name);
    if(ofFile::doesFileExist(p))
        return p;

    if(ofFile::doesFileExist(name))
        return name;
}

void OscPlaybackManager::update(float dt){
    OscPlayback *playback;

    for(int i=playbacks.size()-1; i>=0; i--){
        playback = playbacks[i];
        if(!playback->update(dt)){
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
    // find specified playback
    for(int i=playbacks.size()-1; i>=0; i--){
        if(playbacks[i] == playback){
            // found it! remove from list
            playbacks.erase(playbacks.begin()+i);
            // unregister from event
            ofRemoveListener(playback->messageEvent, this, &OscPlaybackManager::onMessage);
            // delete
            delete playback->getFile();
            delete playback;
            return true;
        }
    }
    // not found
    return false;
}

void OscPlaybackManager::clear(){
    for(int i=playbacks.size()-1; i>=0; i--){
        remove(playbacks[i]);
    }
}

void OscPlaybackManager::onMessage(ofxOscMessage &message){
#ifdef __OSC_SENDER_ENABLED__
    if(OscSender::instance()->isEnabled()){
        // send out to specified address (used for debugging; the receiving end can
        // broadcast everything and so it might come right back to us)
        OscSender::instance()->sender.sendMessage(message);
    } else {
        // deal with messages locally
        OscReceiver::instance()->processMessage(message);
    }
#else
    // the raspi version doesn't send osc out, deal with these locally
    OscReceiver::instance()->processMessage(message);
#endif
}

