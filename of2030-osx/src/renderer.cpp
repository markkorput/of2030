//
//  renderer.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "renderer.hpp"

using namespace of2030;
using namespace of2030::effects;

Renderer::Renderer(){
    player = Player::instance();
}

void Renderer::draw(){
    int size = player->active_effects.size();
    ofLogVerbose() << "[Renderer] active effects: " << size;

    for(int i=0; i<size; i++){
        Effect* effect = player->active_effects[i];
        switch(effect->type){
            case EffectType::OFF:
                drawEffect(*(effects::Off*)effect);
                break;
            case EffectType::COLOR:
                drawEffect(*(effects::Color*)effect);
                break;
            default:
                ofLogWarning() << "[Renderer] Unknown effect type: " << effect->type;
        }
    }
}

void Renderer::drawEffect(effects::Off &effect){
    ofBackground(0);
}

void Renderer::drawEffect(effects::Color &effect){
    // ofLogVerbose() << "drawing COLOR-effect: " << effect.color;
    ofBackground(effect.color);
}