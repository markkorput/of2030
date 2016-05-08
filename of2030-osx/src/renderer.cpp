//
//  renderer.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "renderer.hpp"
#include "xml_effects.hpp"
#include "xml_screens.hpp"

using namespace of2030;
using namespace of2030::effects;

Renderer::Renderer(){
    fbo = NULL;
    player = NULL;
    client_info = NULL;
}

Renderer::~Renderer(){
    destroy();
}

void Renderer::setup(){
    if(fbo == NULL)
        fbo = new ofFbo();
    
    if(!fbo->isAllocated())
        fbo->allocate(WIDTH, HEIGHT);

    if(!player){
        player = Player::instance();
    }
    
    if(!client_info){
        client_info = ClientInfo::instance();
    }

    registerRealtimeEffectCallback();
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
    // ofLog() << "[Renderer] active effects: " << size;

    Context context;
    fillContextClientInfo(context);
    
    for(auto & effect: player->active_effects){
        fillEffectSetting(*effect, context.effect_setting);
        fillScreenSetting(*effect, context.screen_setting);
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
    fillContext(context, effect);
    effect.setup(context);
}

void Renderer::fillContext(effects::Context &context, Effect &effect){
    fillContextClientInfo(context);
    fillEffectSetting(effect, context.effect_setting);
    fillScreenSetting(effect, context.screen_setting);
}

void Renderer::fillContextClientInfo(effects::Context &context){
    context.time = player->getTime();
    context.fbo = fbo;
}

void Renderer::fillEffectSetting(effects::Effect &effect, XmlItemSetting &fxsetting){
    XmlEffects *fxs = XmlEffects::instance();

    string query = effect.name;
    XmlItemSetting *pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    query += "." + player->song;
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    query += "" + player->clip;
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    query += "." + effect.trigger;
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);
}

void Renderer::fillScreenSetting(effects::Effect &effect, XmlItemSetting &setting){
    XmlScreens* screens = XmlScreens::instance();

    XmlItemSetting *pSetting = screens->getItem(client_info->id);
    if(pSetting)
        setting.merge(*pSetting);
}

