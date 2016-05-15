//
//  effect.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "effects.hpp"
#include "video_manager.hpp"
#include "shader_manager.hpp"

using namespace of2030::effects;

int Effect::cidCounter = 0;

Effect::Effect() : name("default"), startTime(NO_TIME), endTime(NO_TIME), duration(NO_TIME), trigger(""), shader(NULL) {
    // every effect instance gets a unique cid (client-side-id)
    cid = cidCounter;
    cidCounter++;
}

void Effect::setup(Context &context){
    if(!hasStartTime()){
        startTime = context.time;
    }

    duration = context.effect_setting.getValue("duration", 3.0f);

    if(hasDuration() && hasStartTime() && !hasEndTime()){
        endTime = startTime + duration;
    }
    
    // load any shaders based on comma-seperated "shaders" effect setting
    string shader_name = context.effect_setting.getValue("shader", "");
    if(shader_name != ""){
        this->shader = ShaderManager::instance()->get(shader_name);
    }
}

void Effect::draw(Context &context){
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());

    if(!shader){
        // draw iwthout shader stuff
        ofSetColor(255);
        ofDrawRectangle(0, 0, resolution.x, resolution.y);
        return;
    }

    EffectLogic logic((Effect*)this, &context);
    
    // gather some data we'll be putting as uniforms in our shaders
    ofPoint pos = ofPoint(context.effect_setting.getValue("pos_x", 0.0f),
                          context.effect_setting.getValue("pos_y", 0.0f),
                          context.effect_setting.getValue("pos_z", 0.0f));
    
    ofVec2f screenWorldSize(context.screen_setting.getValue("world_width", 2.67f),
                            context.screen_setting.getValue("world_height", 2.0f));
    
    
    float progress = logic.getGlobalProgress();
    float duration = logic.getGlobalDuration();
    float iterations = context.effect_setting.getValue("iterations", 1.0f);
    float localPanoStart = context.screen_setting.getValue("pano_start", 0.0f);
    float localPanoEnd = context.screen_setting.getValue("pano_end", 0.0f);
    float gain = context.effect_setting.getValue("gain", 1.0f);
    
    //    ofCamera cam;
    //    cam.setPosition(context.screen_setting.getValue("cam_pos_x", 0.0f),
    //                    context.screen_setting.getValue("cam_pos_y", 0.0f),
    //                    context.screen_setting.getValue("cam_pos_z", 0.0f));
    //    cam.lookAt(ofVec3f(context.screen_setting.getValue("cam_look_at_x", 0.0f),
    //                       context.screen_setting.getValue("cam_look_at_y", 0.0f),
    //                       context.screen_setting.getValue("cam_look_at_z", 4.5f)));

    // activate shader
    shader->begin();

    // populate shader
    //shader->setUniformMatrix4f("iScreenCamMatrix", cam.getModelViewMatrix());
    shader->setUniform3f("iPos", pos);
    shader->setUniform2f("iScreenWorldSize", screenWorldSize);
    // shader->setUniform1f("iTime", context.time);
    shader->setUniform2f("iResolution", resolution);
    shader->setUniform1f("iProgress", progress);
    shader->setUniform1f("iDuration", duration);
    shader->setUniform1f("iIterations", iterations);
    shader->setUniform1f("iLocalPanoStart", localPanoStart);
    shader->setUniform1f("iLocalPanoEnd", localPanoEnd);
    shader->setUniform1f("iGain", gain);

    // draw
    ofSetColor(255);
    ofDrawRectangle(0, 0, resolution.x, resolution.y);

    // deactivate shader
    shader->end();
}

float Effect::getDuration(){
    if(hasDuration())
        return duration;

    if(hasStartTime() and hasEndTime())
        return endTime - startTime;

    return -1.0;
}

void Effect::setType(EffectType effect_type){
    type = effect_type;
    name = EFFECT_NAMES[effect_type];
}

// === === === === === === === === ===

float EffectLogic::getGlobalTime(){ return context->time - effect->startTime; }
float EffectLogic::getGlobalDuration(){ return effect->endTime - effect->startTime; }
float EffectLogic::getGlobalProgress(){ return getGlobalTime() / getGlobalDuration(); }

// === === === === === === === === ===

Off::Off(){
    setType(EffectType::OFF);
}

void Off::draw(Context &context){
    ofBackground(0);
}

// === === === === === === === === ===

Color::Color(){
    setType(EffectType::COLOR);
    color = ofColor::black;
}


void Color::draw(Context &context){
    ofBackground(color);
}

// === === === === === === === === ===


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
