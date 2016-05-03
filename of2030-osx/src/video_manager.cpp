//
//  video_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-03.
//
//

#include "video_manager.hpp"

using namespace of2030;

VideoManager* VideoManager::singleton = NULL;

VideoManager* VideoManager::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new VideoManager();
    }
    return singleton;
}

VideoManager::VideoManager(){

}

VideoManager::~VideoManager(){
    while(!players.empty()){
        ofVideoPlayer* player = players[-1];
        players.pop_back();
        delete player;
    }
}

void VideoManager::setup(){
    
}

void VideoManager::update(){
    
}

ofVideoPlayer* VideoManager::load(string video_name){
    string path = video_name_to_path(video_name);
    ofVideoPlayer *player = new ofVideoPlayer;
    player->load(path);
    player->setLoopState(OF_LOOP_NORMAL);
    // player->play();
    players.push_back(player);
    return player;
}

ofVideoPlayer* VideoManager::get(string video_name, bool load){
    string path = video_name_to_path(video_name);
    for(int i=players.size()-1; i>=0; i--){
        if(players[i]->getMoviePath() == path){
            return players[i];
        }
    }
    
    if(load){
        return this->load(video_name);
    }

    return NULL;
}

string VideoManager::video_name_to_path(string video_name){
    return "vids/" + video_name;
}