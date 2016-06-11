//
//  renderer.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "renderer.hpp"
#include "effect_manager.hpp"
#include "xml_settings.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(Renderer)

Renderer::Renderer() : fbo(NULL),
                        fbo2(NULL),
                        fbo3(NULL),
                        player(NULL),
                        client_id(""),
                        bCallbacksRegistered(false),
                        lastFrameTime(0.0f),
                        effect_configs_instance(NULL){
    // runs effect's setup
    //overlayEffect = EfficientEffectManager::instance()->get("overlay");
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
        ofVec2f resolution(screenWidth, screenHeight);

        if(client_id != ""){
            XmlItemSetting* pItem = XmlConfigs::screens()->getItem(client_id);
            if(pItem){
                resolution = pItem->getValue("resolution", resolution);
            }
        }

        fbo->allocate(resolution.x, resolution.y);
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
    effect_configs_instance = XmlConfigs::instance(); // let's cache this one, might speed loading effect configs up a bit
    

    fillContextClientInfo(cached_context);
    bScreenConfigCached = XmlSettings::instance()->cache_screen_configs;
    if(bScreenConfigCached){
        // preload screen config cache
        fillScreenSetting(cached_context.screen_setting);
    }
}

void Renderer::destroy(){
    if(bCallbacksRegistered)
        registerRealtimeEffectCallback(false);
}

void Renderer::draw(){
    if(!bScreenConfigCached){
        cached_context.screen_setting.data.clear();
        fillScreenSetting(cached_context.screen_setting);
    }

    bool fboFirst = XmlSettings::instance()->drawToFboFirst;
    if(fboFirst){
        fbo->begin();
        ofClear(0.0f,0.0f,0.0f,0.0f);
    } else {
        ofPushMatrix();
        ofScale(screenWidth / fbo->getWidth(), screenHeight / fbo->getHeight(), 1.0f);
        ofScale(cached_context.screen_setting.getValue("screen_scale", ofVec3f(1.0)));
        ofTranslate(cached_context.screen_setting.getValue("screen_translate", ofVec3f(0.0)));
        ofVec3f rot = cached_context.screen_setting.getValue("screen_rotate", ofVec3f(0.0));
        ofRotateX(rot.x);
        ofRotateY(rot.y);
        ofRotateZ(rot.z);
    }

    float dt = ofGetElapsedTimef() - lastFrameTime;
    lastFrameTime += dt;

    cached_context.time = player->getTime();

    // draw all active effects
    vector<Effect*> effects = player->getActiveEffects();
    int count=effects.size();
    Effect* effect;

    for(int i=0; i<count; i++){
        effect = effects[i];

        cached_context.effect_setting.data.clear();
        fillEffectSetting(*effect, cached_context.effect_setting);

        ofEnableBlendMode(effect->getBlendMode());
        effect->draw(cached_context, dt);
    }

    if(fboFirst){
        fbo->end();

        ofPushMatrix();
        ofScale(screenWidth / fbo->getWidth(), screenHeight / fbo->getHeight(), 1.0f);
        ofScale(cached_context.screen_setting.getValue("screen_scale", ofVec3f(1.0)));
        ofTranslate(cached_context.screen_setting.getValue("screen_translate", ofVec3f(0.0)));
        ofVec3f rot = cached_context.screen_setting.getValue("screen_rotate", ofVec3f(0.0));
        ofRotateX(rot.x);
        ofRotateY(rot.y);
        ofRotateZ(rot.z);

        ofSetColor(255);
        ofRectMode(OF_RECTMODE_CORNER);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        fbo->draw(0,0);
        ofPopMatrix();
    } else {
        ofPopMatrix();
    }
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
    if(!bScreenConfigCached){
        cached_context.screen_setting.data.clear();
        fillScreenSetting(cached_context.screen_setting);
    }
    
    cached_context.effect_setting.data.clear();
    fillEffectSetting(effect, cached_context.effect_setting);
    cached_context.time = player->getTime();
    effect.setup(cached_context);
}

void Renderer::fillContextClientInfo(Context &context){
    // context.time = player->getTime();
    // context.fbo = fbo;
    context.fbo2 = fbo2;
    context.fbo3 = fbo3;

    context.tunnel_size.set(XmlSettings::instance()->room_size.z, XmlSettings::instance()->room_size.y);
    context.pano_size.set((XmlSettings::instance()->room_size.z+XmlSettings::instance()->room_size.x)*2, XmlSettings::instance()->room_size.y);
}

void Renderer::fillEffectSetting(Effect &effect, XmlItemSetting &fxsetting){
    // effect- (trigger)-specific config
    string query = effect.trigger;
    XmlItemSetting *pSetting = effect_configs_instance->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

#ifdef __EXTENDED_EFFECT_CONFIG__
    // effect/trigger-specific config (has priority over song/clip-specific configs)
    pSetting = effect_configs_instance->getItem(query+"."+effect.trigger);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song specific effect config
    query += "." + player->getSong();
    pSetting = effect_configs_instance->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song/clip specific effect config
    query += "." + player->getClip();
    pSetting = effect_configs_instance->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);

    // song/clip/trigger specific configs
    query += "." + effect.trigger;
    pSetting = effect_configs_instance->getItem(query);
    if(pSetting)
        fxsetting.merge(*pSetting);
#endif
}

void Renderer::fillScreenSetting(XmlItemSetting &setting){
    XmlItemSetting *pSetting = XmlConfigs::screens()->getItem(client_id);

    if(pSetting)
        setting.merge(*pSetting);
}
