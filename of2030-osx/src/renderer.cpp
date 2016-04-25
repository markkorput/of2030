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

Renderer::Renderer() : fbo(NULL){
    player = Player::instance();
    client_info = ClientInfo::instance();
}

Renderer::~Renderer(){
    destroy();
}

void Renderer::setup(){
    registerRealtimeEffectCallback();
    
    fbo = new ofFbo();
    fbo->allocate(WIDTH, HEIGHT);
}

void Renderer::destroy(){
    registerRealtimeEffectCallback(false);
    if(fbo){
        delete fbo;
        fbo = NULL;
    }
}


void Renderer::draw(){
    fbo->begin();
    
    int size = player->active_effects.size();
    ofLogVerbose() << "[Renderer] active effects: " << size;

    Context context;
    context.time = player->getTime();
    context.cinfo = client_info;
    context.fbo = fbo;
    
    for(int i=0; i<size; i++){
        Effect* effect = player->active_effects[i];
        effect->draw(context);
    }

    fbo->end();
    fbo->draw(0,0);
}

void Renderer::registerRealtimeEffectCallback(bool reg){
    if(reg){
        ofAddListener(player->realtime_composition.newEffectEvent, this, &Renderer::onRealtimeEffect);
    } else {
        ofRemoveListener(player->realtime_composition.newEffectEvent, this, &Renderer::onRealtimeEffect);
    }
}

void Renderer::onRealtimeEffect(Effect &effect){
    Context context;
    context.time = player->getTime();
    context.cinfo = client_info;
    context.fbo = fbo;
    effect.setup(context);
}