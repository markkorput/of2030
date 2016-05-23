//
//  renderer.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "renderer.hpp"
#include "xml_configs.hpp"

using namespace of2030;
using namespace of2030::effects;

SINGLETON_CLASS_IMPLEMENTATION_CODE(Renderer)

Renderer::Renderer() : fbo(NULL), fbo2(NULL), player(NULL), client_id(""), bCallbacksRegistered(false){
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
}

Renderer::~Renderer(){
    destroy();
}

void Renderer::setup(){
    if(fbo == NULL)
        fbo = &defaultFbo;

    if(!fbo->isAllocated()){
        if(client_id == "")
            fbo->allocate(WIDTH, HEIGHT);
        else {
            //fbo->allocate(WIDTH, HEIGHT);
            XmlItemSetting* pItem = XmlConfigs::screens()->getItem(client_id);
            if(pItem){
                fbo->allocate(pItem->getValue("pixel_width", WIDTH), pItem->getValue("pixel_height", HEIGHT));
            } else {
                fbo->allocate(WIDTH, HEIGHT);
            }
        }
        
    }

    if(fbo2 == NULL)
        fbo2 = &defaultFbo2;
    
    if(!fbo2->isAllocated()){
        fbo2->allocate(fbo->getWidth(), fbo->getHeight());
    }

    if(!player)
        player = Player::instance();

    if(!bCallbacksRegistered)
        registerRealtimeEffectCallback();
}

void Renderer::destroy(){
    if(bCallbacksRegistered)
        registerRealtimeEffectCallback(false);
}

void Renderer::draw(){
    Context context;
    fillContextClientInfo(context);

    fbo->begin();
    ofBackground(0);

    vector<effects::Effect*> effects = player->getActiveEffects();
    for(auto effect: effects){
        fillEffectSetting(*effect, context.effect_setting);
        fillScreenSetting(*effect, context.screen_setting);
        effect->draw(context);
    }

    fbo->end();

    ofSetColor(255);
    fbo->draw(0,0, screenWidth, screenHeight);
}

void Renderer::registerRealtimeEffectCallback(bool reg){
    if(reg){
        ofAddListener(player->effect_manager.effectAddedEvent, this, &Renderer::onEffectAdded);
    } else {
        ofRemoveListener(player->effect_manager.effectAddedEvent, this, &Renderer::onEffectAdded);
    }

    bCallbacksRegistered = reg;
}

void Renderer::onEffectAdded(Effect &effect){
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
    context.fbo2 = fbo2;
}

void Renderer::fillEffectSetting(effects::Effect &effect, XmlItemSetting &fxsetting){
    XmlConfigs *fxs = XmlConfigs::instance();

    // effect config
    string query = effect.name;
    XmlItemSetting *pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song specific effect config
    query += "." + player->getSong();
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song/clip specific effect config
    query += "" + player->getClip();
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // trigger-specific config (has priority over song/clip-specific configs)
    pSetting = fxs->getItem(effect.trigger);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // effect/trigger-specific config (has priority over song/clip-specific configs)
    pSetting = fxs->getItem(effect.name+"."+effect.trigger);
    if(pSetting)
        fxsetting.merge(*pSetting);
    
    // song/clip/trigger specific configs
    query += "." + effect.trigger;
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);
}

void Renderer::fillScreenSetting(effects::Effect &effect, XmlItemSetting &setting){
    XmlItemSetting *pSetting = XmlConfigs::screens()->getItem(client_id);

    if(pSetting)
        setting.merge(*pSetting);
}
