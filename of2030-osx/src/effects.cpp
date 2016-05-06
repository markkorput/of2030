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

float EffectLogic::getEffectTime(){
    return context->time - effect->startTime;
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
    CursorLogic logic((Effect*)this, &context);
    
    ofSetColor(255);
    ofDrawRectangle(logic.getLocalProgress() * context.fbo->getWidth(),
                    0,
                    3,
                    context.fbo->getHeight());
}

float CursorLogic::getGlobalDuration(){     return effect->endTime - effect->startTime; }
float CursorLogic::getIterations(){         return 1.0; } // not supported yet
float CursorLogic::getIterationDuration(){  return getGlobalDuration() / getIterations(); } // not supported yet
int CursorLogic::getCurrentIteration(){     return floor(getEffectTime() / getIterationDuration()); }
float CursorLogic::getIterationTime(){      return getEffectTime() - getCurrentIteration() * getIterationDuration(); }
float CursorLogic::getIterationProgress(){  return getIterationTime() / getIterationDuration(); }
float CursorLogic::getLocalProgress(){
    return ofMap(getIterationProgress(),
                 context->client_setting->pano_start,
                 context->client_setting->pano_end,
                 0.0, 1.0);
}

// ==============
// Shader Effects
// ==============

void ShaderEffect::setup(Context &context){
    Effect::setup(context);
    shader = ShaderManager::instance()->get(shaderName);
}

//void ShaderEffect::draw(Context &context){
//
//};


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
    shader->begin();
        shader->setUniform2f("iPos", ofVec2f(0.0f, progress));
        shader->setUniform1f("iThreshold", treshold);
        ofDrawRectangle(0, 0, context.fbo->getWidth(), context.fbo->getHeight());
    shader->end();
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
    shader->begin();
        shader->setUniform1f("iTime", context.time);
        ofDrawRectangle(0, 0, context.fbo->getWidth(), context.fbo->getHeight());
    shader->end();
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