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

    EffectLogic logic((Effect*)this, &context);

    ofVec2f v2f;
    ofVec3f v3f;
    
    // draw content to fbo3
    context.fbo3->begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofColor clr = context.effect_setting.getValue("color", ofColor(255));
    ofSetColor(clr);

    if(!shader){
        // draw without shader stuff
        ofDrawRectangle(0, 0, resolution.x, resolution.y);
    } else {
        // activate shader
        shader->begin();

        // populate shader
        shader->setUniform2f("iResolution", resolution);
        shader->setUniform3f("iPos", context.effect_setting.getValue("pos", ofVec3f(0.0f)));
        shader->setUniform3f("iSize", context.effect_setting.getValue("size", ofVec3f(0.0f)));

        v2f.set(context.screen_setting.getValue("world_width", 2.67f),
                context.screen_setting.getValue("world_height", 2.0f));
        shader->setUniform2f("iScreenWorldSize", v2f);
        shader->setUniform3f("iScreenPos", context.screen_setting.getValue("pos", ofVec3f(0.0f)));

        shader->setUniform1f("iProgress", logic.getGlobalProgress());
        shader->setUniform1f("iDuration", logic.getGlobalDuration());
        shader->setUniform1f("iIterations", context.effect_setting.getValue("iterations", 1.0f));

        shader->setUniform1f("iScreenPanoStart", context.screen_setting.getValue("pano_start", 0.0f));
        shader->setUniform1f("iScreenPanoEnd", context.screen_setting.getValue("pano_end", 1.0f));
        shader->setUniform1f("iEffectPanoStart", context.effect_setting.getValue("pano_start", 0.0f));
        shader->setUniform1f("iEffectPanoEnd", context.effect_setting.getValue("pano_end", 1.0f));

        shader->setUniform1f("iScreenTunnelStart", context.screen_setting.getValue("tunnel_start", 0.0f));
        shader->setUniform1f("iScreenTunnelEnd", context.screen_setting.getValue("tunnel_end", 1.0f));
        shader->setUniform1f("iEffectTunnelStart", context.effect_setting.getValue("tunnel_start", 0.0f));
        shader->setUniform1f("iEffectTunnelEnd", context.effect_setting.getValue("tunnel_end", 1.0f));

        shader->setUniform1f("iGain", context.effect_setting.getValue("gain", 1.0f));

        // draw
        ofDrawRectangle(0.0f, 0.0f, resolution.x, resolution.y);

        // deactivate shader
        shader->end();
    }
    context.fbo3->end();

    
    // draw mask (if no mask specfied, this will give a full white frame)
    context.fbo2->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        drawMask(context, context.effect_setting.getValue("mask_coords_name", ""));
    context.fbo2->end();

    
    ofShader* maskShader = ShaderManager::instance()->get("mask");
    
    // enable mask shader
    maskShader->begin();
        // pass mask texture to shader
        maskShader->setUniformTexture("iMask", context.fbo2->getTexture(), 2);
        // draw content (masked)
        ofSetColor(255);
        context.fbo3->draw(0.0, 0.0, resolution.x, resolution.y);
    maskShader->end();
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

void Effect::drawMask(Context &context, const string &coordsName){
    ofSetColor(255);
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
    
    ofVec2f coords[4];
    coords[0] = context.screen_setting.getValue(coordsName+"1", ofVec2f(0.0f, 1.0f)) * resolution;
    coords[1] = context.screen_setting.getValue(coordsName+"2", ofVec2f(1.0f, 1.0f)) * resolution;
    coords[2] = context.screen_setting.getValue(coordsName+"3", ofVec2f(1.0f, 0.0f)) * resolution;
    coords[3] = context.screen_setting.getValue(coordsName+"4", ofVec2f(0.0f, 0.0f)) * resolution;
    
    // draw mask content
    ofDrawTriangle(coords[0].x, coords[0].y, coords[1].x, coords[1].y, coords[2].x, coords[2].y);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[2].x, coords[2].y, coords[3].x, coords[3].y);
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

