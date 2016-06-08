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
#include "effect_manager.hpp"
#include "xml_settings.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(Renderer)

Renderer::Renderer() : fbo(NULL), fbo2(NULL), fbo3(NULL), player(NULL), client_id(""), bCallbacksRegistered(false), lastFrameTime(0.0f){
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();

    // runs effect's setup
    //overlayEffect = EfficientEffectManager::instance()->get("overlay");
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
                ofVec2f resolution = pItem->getValue("resolution", ofVec2f(WIDTH, HEIGHT));
                fbo->allocate(resolution.x, resolution.y);
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

    if(fbo3 == NULL)
        fbo3 = &defaultFbo3;

    if(!fbo3->isAllocated()){
        fbo3->allocate(fbo->getWidth(), fbo->getHeight());
    }

    if(!player)
        player = Player::instance();

    if(!bCallbacksRegistered)
        registerRealtimeEffectCallback();

    lastFrameTime = ofGetElapsedTimef();
}

void Renderer::destroy(){
    if(bCallbacksRegistered)
        registerRealtimeEffectCallback(false);
}

void Renderer::draw(){
    Context context;
    fillContextClientInfo(context);
    fillScreenSetting(context.screen_setting);

#ifdef __RENDER_TO_FBO_FIRST__
    fbo->begin();
    ofClear(0.0f,0.0f,0.0f,0.0f);
#else
    ofPushMatrix();
    ofScale(screenWidth / fbo2->getWidth(), screenHeight / fbo2->getHeight(), 1.0f);
#endif

    float dt = ofGetElapsedTimef() - lastFrameTime;
    lastFrameTime += dt;

    // draw all active effects
    vector<Effect*> effects = player->getActiveEffects();
    for(auto effect: effects){
        context.effect_setting.data.clear();
        fillEffectSetting(*effect, context.effect_setting);
        effect->draw(context, dt);
    }

#ifdef __RENDER_TO_FBO_FIRST__
    fbo->end();
    ofSetColor(255);
    fbo->draw(0,0, screenWidth, screenHeight);
#else
    ofPopMatrix();
#endif
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

void Renderer::fillContext(Context &context, Effect &effect){
    fillContextClientInfo(context);
    fillScreenSetting(context.screen_setting);
    fillEffectSetting(effect, context.effect_setting);
}

void Renderer::fillContextClientInfo(Context &context){
    context.time = player->getTime();
    // context.fbo = fbo;
    context.fbo2 = fbo2;
    context.fbo3 = fbo3;
    
    context.tunnel_size.set(XmlSettings::instance()->room_size.z, XmlSettings::instance()->room_size.y);
    context.pano_size.set((XmlSettings::instance()->room_size.z+XmlSettings::instance()->room_size.x)*2, XmlSettings::instance()->room_size.y);
}

void Renderer::fillEffectSetting(Effect &effect, XmlItemSetting &fxsetting){
    XmlConfigs *fxs = XmlConfigs::instance();

    // effect- (trigger)-specific config
    string query = effect.trigger;
    XmlItemSetting *pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

#ifdef __EXTENDED_EFFECT_CONFIG__
    // effect/trigger-specific config (has priority over song/clip-specific configs)
    pSetting = fxs->getItem(query+"."+effect.trigger);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song specific effect config
    query += "." + player->getSong();
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song/clip specific effect config
    query += "." + player->getClip();
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song/clip/trigger specific configs
    query += "." + effect.trigger;
    pSetting = fxs->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);
#endif
}

void Renderer::fillScreenSetting(XmlItemSetting &setting){
    XmlItemSetting *pSetting = XmlConfigs::screens()->getItem(client_id);

    if(pSetting)
        setting.merge(*pSetting);
}
