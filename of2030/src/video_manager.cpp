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
        ofLog() << alias << " loaded.";
        players[alias] = player;
    }

    // return it
    return player;
}

void VideoManager::unloadAll(){
    // don't iterate like normal, because the iterator gets corrupted and causes
    // BAD ACCESS errors when the map gets modified during its iterations
    // we'll just take the first item every time and remove it, until there's nothing left
    while(!players.empty()){
        unload(players.begin()->first);
    }
    
    players.clear();
}

bool VideoManager::unload(const string &alias){
    ofLog() << "VideoManager::unload with " << alias;

    // No specific player specified? destroy all
//    if(alias == ""){
//        destroy();
//        return true;
//    }

    // find specified player
    std::map<string,ofVideoPlayer*>::iterator it = players.find(alias);

    // not found, abort
    if(it == players.end()){
        ofLogWarning() << "VideoManager::unload player not found";
        return false;
    }

    ofNotifyEvent(unloadEvent, *it->second, this);

    if(it->second){
        // close player/video file
        it->second->close();
        // delete instance from memory
        delete it->second;
    }

    // remove from our list
    players.erase(it);


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
    if(XmlSettings::instance()->rgbaVidPixels){
        player->setPixelFormat(OF_PIXELS_RGBA);
    }
    player->loadAsync(path);
    player->setVolume(0.0f);
    return player;
}
