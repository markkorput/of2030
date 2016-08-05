//
//  osc_playback_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#include "playback_manager.hpp"
#include "ascii_file_reader.hpp"

using namespace OscToolkit;

SINGLETON_INLINE_IMPLEMENTATION_CODE(PlaybackManager)

PlaybackManager::PlaybackManager(){
    // by default all playbacks send their message the interface singleton
    interface = of2030::OscInterface::instance();
}

Playback* PlaybackManager::start(const string &name){
    // load file
    AsciiFileReader* file = new AsciiFileReader();
    if(!file){
        return NULL;
    }

    string path = nameToPath(name);
    if(!file->load(path)){
        ofLog() << "could not load osc file: " << path;
        delete file;
        return NULL;
    }

    // start playback
    Playback* playback = new Playback(*file);

    if(!playback){
        delete file;
        return NULL;
    }

    // save for continued use
    playbacks.push_back(playback);

    // register callback if we have a "default" interface to send to
    if(interface){
        ofAddListener(playback->messageEvent, interface, &of2030::OscInterface::process);
    }

    // start playback
    playback->start();

    // ofLogVerbose() << "Started playback of: " << file->getReadPath();
    return playback;
}

bool PlaybackManager::stop(const string &name){
    if(name == ""){ // stop all
        for(int i=playbacks.size()-1; i>=0; i--){
            remove(playbacks[i]);
        }

        playbacks.clear();
        return true;
    }

    Playback *playback = getPlayback(name);
    if(!playback) return false;
    remove(playback);
    return true;
}

Playback* PlaybackManager::getPlayback(const string &name){
    string p = nameToPath(name);

    for(int i=playbacks.size()-1; i>=0; i--){
        if(playbacks[i]->getFile()->getReadPath() == p){
            return playbacks[i];
        }
    }
    return NULL;
}

string PlaybackManager::nameToPath(const string &name){
    string p = ofToDataPath("osc/"+name+".csv");
    if(ofFile::doesFileExist(p))
        return p;

    p = ofToDataPath("osc/"+name);
    if(ofFile::doesFileExist(p))
        return p;

    // if(ofFile::doesFileExist(name))
    return name;
}

void PlaybackManager::update(float dt){
    Playback *playback;

    for(int i=playbacks.size()-1; i>=0; i--){
        playback = playbacks[i];
        if(!playback->update(dt)){
            remove(playback);
        }
    }
}

bool PlaybackManager::remove(Playback *playback){
    // find specified playback
    for(int i=playbacks.size()-1; i>=0; i--){
        if(playbacks[i] == playback){
            // found it! remove from list
            playbacks.erase(playbacks.begin()+i);
            // unregister from event
            // ofRemoveListener(playback->messageEvent, this, &PlaybackManager::onMessage);
            // delete
            delete playback->getFile();
            delete playback;
            return true;
        }
    }
    // not found
    return false;
}

void PlaybackManager::clear(){
    for(int i=playbacks.size()-1; i>=0; i--){
        remove(playbacks[i]);
    }
}
