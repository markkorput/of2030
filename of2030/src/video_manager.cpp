//
//  video_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-03.
//
//

#include "video_manager.hpp"

using namespace of2030;

SINGLETON_CLASS_IMPLEMENTATION_CODE(VideoManager)

VideoManager::VideoManager(){

}

VideoManager::~VideoManager(){
    for(auto player: players){
        delete player;
    }

    players.clear();
}

//void VideoManager::setup(){
//    
//}

void VideoManager::update(){
    for(auto player: players){
        player->update();
    }
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

bool VideoManager::unload(string &video_name){
    if(video_name == ""){
        for(auto player: players){
            unload(player);
        }
    }

    ofVideoPlayer* player = get(video_name, false);
    if(player){
        unload(player);
        return true;
    }

    return false;
}

void VideoManager::unload(ofVideoPlayer *player){
    if(player == NULL){
        return;
    }

    // rmeove from our list
    for(int i=players.size()-1; i>=0; i--){
        // this one?
        if(players[i] == player){
            // remove from list
            players.erase(players.begin()+i);
        }
    }

    player->close();
    delete player;
}