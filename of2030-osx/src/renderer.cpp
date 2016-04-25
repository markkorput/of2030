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
    m_client_info = ClientInfo::instance();
}

Renderer::~Renderer(){
    destroy();
}

void Renderer::setup(){
    registerRealtimeEffectCallback();
}

void Renderer::destroy(){
    registerRealtimeEffectCallback(false);
}


void Renderer::draw(){
    int size = player->active_effects.size();
    ofLogVerbose() << "[Renderer] active effects: " << size;

    for(int i=0; i<size; i++){
        Effect* effect = player->active_effects[i];
        switch(effect->type){
            case EffectType::OFF:
                drawEffect(*(Off*)effect);
                break;
            case EffectType::COLOR:
                drawEffect(*(Color*)effect);
                break;
            case EffectType::CURSOR:
                drawEffect(*(Cursor*)effect);
                break;
            default:
                ofLogWarning() << "[Renderer] Unknown effect type: " << effect->type;
        }
    }
}

void Renderer::drawEffect(Off &effect){
    ofBackground(0);
}

void Renderer::drawEffect(Color &effect){
    // ofLogVerbose() << "drawing COLOR-effect: " << effect.color;
    ofBackground(effect.color);
}

void Renderer::setupEffect(Cursor &effect){
    if(!effect.hasStartTime()){
        effect.startTime = player->getTime();
    }
}

void Renderer::drawEffect(Cursor &effect){
    int idx = m_client_info->client_index;
    int count = m_client_info->client_count;
    
    float duration = effect.getDuration();
    float effectTime = player->getTime() - effect.startTime;
    float localDuration = duration / count;
    float localStart = localDuration * idx;
    
    if(effectTime < localStart)
        // nothing for us to do (yet)
        return;

    float localEffectTime = effectTime - localStart;

    if(localEffectTime > localDuration)
        // our part is done
        return;

    float localProgress = localEffectTime / localDuration;
    
    ofSetColor(255);
    ofDrawRectangle(localProgress * ofGetWidth(), 0, 3, ofGetHeight());
}

void Renderer::registerRealtimeEffectCallback(bool reg){
    if(reg){
        ofAddListener(player->realtime_composition.newEffectEvent, this, &Renderer::onRealtimeEffect);
    } else {
        ofRemoveListener(player->realtime_composition.newEffectEvent, this, &Renderer::onRealtimeEffect);
    }
}

void Renderer::onRealtimeEffect(Effect &effect){
    switch(effect.type){
        case EffectType::CURSOR:
            setupEffect(*((Cursor*)&effect));
            break;
        case EffectType::OFF:
        case EffectType::COLOR:
            ;
    }
}