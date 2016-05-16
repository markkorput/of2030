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

// int Effect::cidCounter = 0;

Effect::Effect(){
    reset();
    setType(DEFAULT);

    // every effect instance gets a unique cid (client-side-id)
    // cid = cidCounter;
    // cidCounter++;
}

void Effect::reset(){
    startTime = NO_TIME;
    endTime = NO_TIME;
    duration = NO_TIME;
    trigger = "";
    shader = NULL;
}

void Effect::setup(Context &context){
    if(!hasStartTime()){
        startTime = context.time;
    }

    duration = context.effect_setting.getValue("duration", 30.0f);

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
    

//    ofVec3f vel = ofVec3f(context.effect_setting.getValue("vel_x", 0.0f),
//                          context.effect_setting.getValue("vel_y", 0.0f),
//                          context.effect_setting.getValue("vel_z", 0.0f));

//    if(context.effect_setting.hasValue("auto_pano_shift")){
//        float newVal = effectPanoEnd + context.effect_setting.getValue("auto_pano_shift", 0.0f) * logic.getGlobalTime();
//        if(context.effect_setting.hasValue("auto_pano_shift_end")){
//            newVal = std::min(newVal, context.effect_setting.getValue("auto_pano_shift_end", 0.0f));
//        }
//        effectPanoEnd = newVal;
//    }

    //    ofCamera cam;
//    ofVec3f camWorldPos(context.screen_setting.getValue("cam_pos_x", 0.0f),
//                        context.screen_setting.getValue("cam_pos_y", 0.0f),
//                        context.screen_setting.getValue("cam_pos_z", 0.0f));

    //    cam.lookAt(ofVec3f(context.screen_setting.getValue("cam_look_at_x", 0.0f),
    //                       context.screen_setting.getValue("cam_look_at_y", 0.0f),
    //                       context.screen_setting.getValue("cam_look_at_z", 4.5f)));

    ofVec2f v2f;
    ofVec3f v3f;
    // activate shader
    shader->begin();

    // populate shader
    shader->setUniform2f("iResolution", resolution);

    v3f.set(context.effect_setting.getValue("pos_x", 0.0f),
             context.effect_setting.getValue("pos_y", 0.0f),
             context.effect_setting.getValue("pos_z", 0.0f));
    shader->setUniform3f("iPos", v3f);

    v3f.set(context.effect_setting.getValue("size_x", 0.0f),
            context.effect_setting.getValue("size_y", 0.0f),
            context.effect_setting.getValue("size_z", 0.0f));
    shader->setUniform3f("iSize", v3f);

    v2f.set(context.screen_setting.getValue("world_width", 2.67f),
            context.screen_setting.getValue("world_height", 2.0f));
    shader->setUniform2f("iScreenWorldSize", v2f);

    shader->setUniform1f("iProgress", logic.getGlobalProgress());
    shader->setUniform1f("iDuration", logic.getGlobalDuration());
    shader->setUniform1f("iIterations", context.effect_setting.getValue("iterations", 1.0f));
    shader->setUniform1f("iScreenPanoStart", context.screen_setting.getValue("pano_start", 0.0f));
    shader->setUniform1f("iScreenPanoEnd", context.screen_setting.getValue("pano_end", 0.0f));
    shader->setUniform1f("iEffectPanoStart", context.effect_setting.getValue("pano_start", 0.0f));
    shader->setUniform1f("iEffectPanoEnd", context.effect_setting.getValue("pano_end", 1.0f));
    shader->setUniform1f("iGain", context.effect_setting.getValue("gain", 1.0f));

    // draw
    ofSetColor(255);
    ofDrawRectangle(0, 0, resolution.x, resolution.y);

    // deactivate shader
    shader->end();
}

float Effect::getDuration() const {
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

// === === === === === === === === ===

Tunnel::Tunnel(){
    setType(EffectType::TUNNEL);
}

// virtual void setup(Context &context);
void Tunnel::draw(Context &context){
    float tunnelStart = context.screen_setting.getValue("tunnel_start", 0.0f);
    float tunnelEnd = context.screen_setting.getValue("tunnel_end", 1.0f);
}

// === === === === === === === === ===

Spot::Spot(){
    setType(EffectType::SPOT);
}

// virtual void setup(Context &context);
void Spot::draw(Context &context){
    // screen must be spot-enabled
    if(!(context.screen_setting.hasValue("spot_x") &&
         context.screen_setting.hasValue("spot_y") &&
         context.screen_setting.hasValue("spot_w") &&
         context.screen_setting.hasValue("spot_h"))){
        return;
    }

    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());

    ofVec2f spotPos = ofVec2f(context.screen_setting.getValue("spot_x", 0.0f),
                              context.screen_setting.getValue("spot_y", 0.0f)) * resolution;
    ofVec2f spotSize = ofVec2f(context.screen_setting.getValue("spot_w", 1.0f),
                               context.screen_setting.getValue("spot_h", 1.0f)) * resolution;

    if(!shader){
        // draw without shader stuff
        ofSetColor(255);
        // ofDrawRectangle(0, 0, resolution.x, resolution.y);
        ofDrawEllipse(spotPos.x, spotPos.y, spotSize.x, spotSize.y);
        return;
    }

    shader->begin();

    //    shader->setUniform2f("iResolution", resolution);
    shader->setUniform2f("iSpotPos", spotPos);
    shader->setUniform2f("iSpotSize", spotSize);
    shader->setUniform1f("iGain", context.effect_setting.getValue("gain", 1.0f));

    
    // quarter; 1 means top right, 2 means bottom right, 3 bottom left, 4 means top left, zero means none
    int q = std::floor(context.effect_setting.getValue("quarter_on", 0.0f));
    shader->setUniform1i("iQuarterOn", q);
    q = std::floor(context.effect_setting.getValue("quarter_off", 0.0f));
    shader->setUniform1i("iQuarterOff", q);

    spotPos = spotPos - spotSize * 0.5;
    ofSetColor(255);
    ofDrawRectangle(0.0f, 0.0f, resolution.x, resolution.y); //spotPos.x, spotPos.y, spotSize.x, spotSize.y);

    shader->end();
}

