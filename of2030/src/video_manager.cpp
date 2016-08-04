//
//  video_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-03.
//
//

#include "video_manager.hpp"
#include "xml_settings.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(VideoManager)

VideoManager::VideoManager(){
#ifdef __APPLE__
    folder_path = "vids/_osx/";
#else
    folder_path = "vids/_raspi/";
#endif // __APPLE__
}

//void VideoManager::setup(){
//    
//}

void VideoManager::update(){
    ofVideoPlayer* player;

    // first all players queued for removal
    for(int i=deprecations.size()-1; i>=0; i--){
        // players map pair for current deprecation alias
        std::map<string,ofVideoPlayer*>::iterator it = players.find(deprecations[i]);

        // found?
        if(it != players.end()){
            // take player from pair
            player = it->second;
            // if player is still playing, stop it and unload at next iteration
            if(player && player->isPlaying()){
                player->stop();
                player->closeMovie();
                player->close();
                // skip removal of depreaction alias; we still need it next iteration
                continue;
            // player not playing, unload now
            } else {
                unload(deprecations[i]);
            }
        }

        // this depraction is done
        deprecations.erase(deprecations.begin()+i);
    }

    // then update all active players
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
        players[alias] = player;
        ofLog() << alias << " loaded. Video count: " << players.size();
    }

    // return it
    return player;
}

void VideoManager::unloadAll(){
    string alias;

    // don't iterate like normal, because the iterator gets corrupted and causes
    // BAD ACCESS errors when the map gets modified during its iterations
    // we'll just take the first item every time and remove it, until there's nothing left
    while(!players.empty()){
        alias = players.begin()->first;
        unload(alias);
    }

    players.clear();
}

#define MAX_IDLE_PLAYERS 7

bool VideoManager::unload(string alias){
    // find specified player
    std::map<string,ofVideoPlayer*>::iterator it = players.find(alias);

    // not found, abort
    if(it == players.end()){
        ofLogWarning() << "video not found for unload: " << alias;
        return false;
    }

    ofVideoPlayer* player = it->second;

    if(player){
        player->stop();
        player->closeMovie();
        player->close();
        ofNotifyEvent(unloadEvent, *player, this);
        // delete instance from memory
        idle_players.push_back(player);
        
        if(idle_players.size() > MAX_IDLE_PLAYERS){
            player = idle_players[0];
            idle_players.erase(idle_players.begin());
            delete player;
        }
    }

    // remove from our list
    players.erase(it);

    // log and report
    ofLog() << alias << " unloaded, video count: " << players.size();
    return true;
}

void VideoManager::unload(ofVideoPlayer *player){
    if(player == NULL){
        ofLog() << "VideoManager got NULL player to unload";
        return;
    }

    string alias = "";

    // find player
    for (auto& pair: players) {
        // this one?
        if(pair.second == player){
            alias = pair.first;
            break;
        }
    }
    
    if(alias == ""){
        ofLog() << "VideoManager could not find specified player to unload";
        return;
    }

    unload(alias);
}

void VideoManager::deprecate(ofVideoPlayer *player){
    string alias = "";

    for(std::map<string,ofVideoPlayer*>::iterator it = players.begin(); it != players.end(); ++it){
        if(it->second == player){
            alias = it->first;
            break;
        }
    }
    
    if(alias == ""){
        ofLogWarning() << "video not found for deprecation";
        return;
    }

    deprecate(alias);
}

void VideoManager::deprecateAll(){
    string alias;

    // add all active player sto the deprecation list
    for(std::map<string,ofVideoPlayer*>::iterator it = players.begin(); it != players.end(); ++it){
        alias = it->first;
        deprecate(alias);
    }
}

ofVideoPlayer* VideoManager::createPlayer(const string &video_name){
    string path = video_name_to_path(video_name);

    // ofLog() << "loading vid: " << path; //player->getMoviePath();
    
    if(!ofFile::doesFileExist(path)){
        ofLogWarning() << "could not find video file to load: " << path;
        return NULL;
    }

    ofVideoPlayer *player;

    if(!idle_players.empty()){
        player = idle_players[0];
        ofLog() << "recycling idle video player";
        idle_players.erase(idle_players.begin());
    } else {
        player = new ofVideoPlayer;
    }

    if(XmlSettings::instance()->rgbaVidPixels){
        player->setPixelFormat(OF_PIXELS_RGBA);
    }
    player->loadAsync(path);
    player->setVolume(0.0f);
    return player;
}

void VideoManager::destroyIdlePlayers(){
    for(int i=idle_players.size()-1; i>=0; i--){
        delete idle_players[i];
    }
    
    idle_players.clear();
}