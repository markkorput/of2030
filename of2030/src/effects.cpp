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

Effect::Effect() : name("default"), startTime(NO_TIME), endTime(NO_TIME), duration(NO_TIME), trigger("") {
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
    string shader_names = context.effect_setting.getValue("shaders", "");
    string shader_name;
    std::size_t pos;
    bool done = false;

    while(!done){
        // find next "comma"
        pos = shader_names.find(",");
        
        // not found? use the whole (rest) of the shader_names value
        if(pos == std::string::npos){
            shader_name = shader_names;
            done = true;
        } else {
            // get next part
            shader_name = shader_names.substr(0, pos);
            // remove this part from the shader_names string, preparing it for next iteration
            shader_names = shader_names.substr(pos+1);
        }

        // try to get the shader with the name we just found
        ofShader *pShader = ShaderManager::instance()->get(shader_name);
        if(pShader)
            // add it to our list
            shaders.push_back(pShader);
    }
}

void Effect::draw(Context &context){
    EffectLogic logic((Effect*)this, &context);

    // gather some data we'll be putting as uniforms in our shaders
    ofPoint pos = ofPoint(context.effect_setting.getValue("pos_x", 0.0f),
                          context.effect_setting.getValue("pos_y", 0.0f),
                          context.effect_setting.getValue("pos_z", 0.0f));

    ofVec2f screenWorldSize(context.screen_setting.getValue("world_width", 2.67f),
                            context.screen_setting.getValue("world_height", 2.0f));

    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
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

    // activate & populate shaders
    for(auto & shader: shaders){
        shader->begin();
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
    }
    
    // draw
    ofSetColor(255);
    ofDrawRectangle(0, 0, resolution.x, resolution.y);

    // de-activate shaders
    for(auto & shader: shaders){
        shader->end();
    }
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

ShaderEffect::ShaderEffect(){
    setType(EffectType::SHADER);
    // empty shaderName, means the ShaderEffect
    // will use its name attribute as shader name
    shaderName = "";
}

void ShaderEffect::setup(Context &context)  {
    Effect::setup(context);
    shader = ShaderManager::instance()->get(shaderName == "" ? name : shaderName);
}

void ShaderEffect::draw(Context &context){
    if(!shader->isLoaded()) return;

    EffectLogic logic((Effect*)this, &context);

    ofPoint pos = ofPoint(context.effect_setting.getValue("pos_x", 0.0f),
                        context.effect_setting.getValue("pos_y", 0.0f),
                        context.effect_setting.getValue("pos_z", 0.0f));
//    ofCamera cam;
//    cam.setPosition(context.screen_setting.getValue("cam_pos_x", 0.0f),
//                    context.screen_setting.getValue("cam_pos_y", 0.0f),
//                    context.screen_setting.getValue("cam_pos_z", 0.0f));
//    cam.lookAt(ofVec3f(context.screen_setting.getValue("cam_look_at_x", 0.0f),
//                       context.screen_setting.getValue("cam_look_at_y", 0.0f),
//                       context.screen_setting.getValue("cam_look_at_z", 4.5f)));
    shader->begin();
        //shader->setUniformMatrix4f("iScreenCamMatrix", cam.getModelViewMatrix());
        shader->setUniform2f("iScreenWorldSize", ofVec2f(context.screen_setting.getValue("world_width", 2.67f), context.screen_setting.getValue("world_height", 2.0f)));
        shader->setUniform3f("iPos", pos);
    
        // shader->setUniform1f("iTime", context.time);
        shader->setUniform2f("iResolution", ofVec2f(context.fbo->getWidth(), context.fbo->getHeight()));
        shader->setUniform1f("iProgress", logic.getGlobalProgress());
        shader->setUniform1f("iDuration", logic.getGlobalDuration());
        shader->setUniform1f("iIterations", context.effect_setting.getValue("iterations", 1.0f));
        shader->setUniform1f("iLocalPanoStart", context.screen_setting.getValue("pano_start", 0.0f));
        shader->setUniform1f("iLocalPanoEnd", context.screen_setting.getValue("pano_end", 0.0f));
        shader->setUniform1f("iGain", context.effect_setting.getValue("gain", 1.0f));
        ofSetColor(255);
        ofDrawRectangle(0, 0, context.fbo->getWidth(), context.fbo->getHeight());
    shader->end();
}

void ShaderEffect::setShader(string _name){
    shaderName = _name;
    name = "shader-" + _name;
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
