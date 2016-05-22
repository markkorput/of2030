//
//  effect.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "effects.hpp"
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

Tunnel::Tunnel(){
    setType(EffectType::TUNNEL);
}

// virtual void setup(Context &context);
void Tunnel::draw(Context &context){
    float tunnelStart = context.screen_setting.getValue("tunnel_start", 0.0f);
    float tunnelEnd = context.screen_setting.getValue("tunnel_end", 1.0f);
}

