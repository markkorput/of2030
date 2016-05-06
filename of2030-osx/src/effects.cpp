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

Effect::Effect() : startTime(NO_TIME), endTime(NO_TIME), type(EffectType::OFF) {
    // every effect instance gets a unique cid (client-side-id)
    cid = cidCounter;
    cidCounter++;
    duration = 3.0;
}

void Effect::setup(Context &context){
    if(!hasStartTime()){
        startTime = context.time;
    }

    if(hasDuration() && hasStartTime() && !hasEndTime()){
        endTime = startTime + duration;
    }
}

void Effect::draw(Context &context){
    // ofBackground(effect.color);
}

float Effect::getDuration(){
    if(hasDuration())
        return duration;

    if(hasStartTime() and hasEndTime())
        return endTime - startTime;

    return -1.0;
}


//
//void Off::setup(Context &context){
//}


void Off::draw(Context &context){
    ofBackground(0);
}




Color::Color(){
    type = EffectType::COLOR;
    color = ofColor::black;
}

//void Color::setup(Context &context){
//}

void Color::draw(Context &context){
    ofBackground(color);
}




//void Cursor::setup(Context &context){
//    Effect::Setup
//    if(!hasStartTime()){
//        startTime = context.time;
//    }
//}

Cursor::Cursor(){
    type = EffectType::CURSOR;
}

void Cursor::draw(Context &context){
    int idx = context.client_index;
    int client_count = context.client_count;

    float duration = getDuration();
    float effectTime = context.time - startTime;
    float localDuration = duration / client_count;
    float localStart = localDuration * idx;

    if(effectTime < localStart)
        // nothing for us to do (yet)
        return;

    float localEffectTime = effectTime - localStart;

    if(localEffectTime > localDuration)
        // our part is done
        return;

    float localProgress = localEffectTime / localDuration;

    ofSetColor(255);
    ofDrawRectangle(localProgress * context.fbo->getWidth(),
                    0,
                    3,
                    context.fbo->getHeight());
}


// ==============
// Shader Effects
// ==============

void ShaderEffect::setup(Context &context){
    Effect::setup(context);
    shader = ShaderManager::instance()->get(shaderName);
}

void ShaderEffect::draw(Context &context){
    shader->begin();
    ofDrawRectangle(0, 0, context.fbo->getWidth(), context.fbo->getHeight());
    shader->end();
};


Stars::Stars(){
    type = EffectType::STARS;
    shaderName = "Starfield01";
}

//void Stars::setup(Context &context){
//}

void Stars::draw(Context &context){
    float progress = ofMap(context.time, startTime, endTime, 250.0f, -50.0f);
    float treshold = ofMap(context.time, startTime, endTime, 0.99999f, 0.96f);
    // ofLog() << "stars progress" << progress << ", time: " << context.time;

    ofSetColor(255);
    shader->setUniform2f("iPos", ofVec2f(0.0f, progress));
    shader->setUniform1f("iThreshold", treshold);
    ShaderEffect::draw(context);
}



Worms::Worms(){
    type = EffectType::WORMS;
    shaderName = "worms";
}

//void Worms::setup(Context &context){
//    ShaderEffect::setup(context);
//}

void Worms::draw(Context &context){
    ofSetColor(255);
    ShaderEffect::draw(context);

    ofSetColor(255);
    shader->setUniform1f("iTime", context.time);
    ShaderEffect::draw(context);
}


// ============
// Video Effect
// ============



Vid::Vid(){
    type = EffectType::WORMS;
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