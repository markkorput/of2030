//
//  vid.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#include "vid.hpp"
#include "video_manager.hpp"

using namespace of2030::effects;

Vid::Vid(){
    setType(EffectType::VID);
}

void Vid::setup(Context &context){
    Effect::setup(context);
    video_player = VideoManager::instance()->get("fingers.mov", true);
    video_player->setLoopState(OF_LOOP_NORMAL);
    video_player->play();
}

void Vid::draw(Context &context){
    video_player->update();
    video_player->draw(0,0);
}