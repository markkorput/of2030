//
//  video_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-03.
//
//

#include "video_manager.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(VideoManager)

VideoManager::VideoManager(){
#ifdef __APPLE__
    folder_path = "vids/_osx/";
#else
    folder_path = "vids/_raspi/";
#endif // __APPLE__
}

void VideoManager::destroy(){
    for (auto& pair: players) {
        delete pair.second;
    }

    players.clear();
}
//void VideoManager::setup(){
//    
//}

void VideoManager::update(){
    for(auto& pair: players){
        pair.second->update();
    }
}

ofVideoPlayer* VideoManager::get(const string &video_name, bool load){
    // assume alias IS the video's path
    return get(video_name, video_name, load);
}

ofVideoPlayer* VideoManager::get(const string &video_name, const string &alias, bool load){
    std::map<string,ofVideoPlayer*>::iterator it = players.find(alias);

    // found it
    if(it != players.end()){
        return it->second;
    }
    
    // not found, no loading
    if(!load)
        return NULL;

    // (try to) create a player
    ofVideoPlayer* player = createPlayer(video_name);

    // store it
    if(player){
        ofLog() << "VideoManager loaded video alias: " << alias;
        players[alias] = player;
    }

    // return it
    return player;
}

bool VideoManager::unload(const string &video_name){
    ofLog() << "VideoManager::unload with " << video_name;

    // No specific player specified? destroy all
    if(video_name == ""){
        destroy();
        return true;
    }

    // find specified player
    std::map<string,ofVideoPlayer*>::iterator it = players.find(video_name);

    // not found, abort
    if(it == players.end()){
        ofLogWarning() << "VideoManager::unload player not found";
        return false;
    }

    // remove from our list
    players.erase(it);
    // close player/video file
    it->second->close();
    // delete instance from memory
    delete it->second;

    // log and report
    ofLog() << "Video players still loaded: " << players.size();
    return true;
}

void VideoManager::unload(ofVideoPlayer *player){
    if(player == NULL){
        return;
    }

    // find player
    for (auto& pair: players) {
        // this one?
        if(pair.second == player){
            unload(pair.first);
        }
    }
}

ofVideoPlayer* VideoManager::createPlayer(const string &video_name){
    string path = video_name_to_path(video_name);

    ofLog() << "VideoManager::createPlayer loading: " << path; //player->getMoviePath();
    
    if(!ofFile::doesFileExist(path)){
        ofLogWarning() << "could not find video file.";
        return NULL;
    }
    
    ofVideoPlayer *player = new ofVideoPlayer;
    
    player->loadAsync(path);
    player->setVolume(0.0f);
    return player;
}
