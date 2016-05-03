//
//  effect.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "effects.hpp"

using namespace of2030::effects;

int Effect::cidCounter = 0;

Effect::Effect() : startTime(NO_TIME), endTime(NO_TIME), type(EffectType::OFF) {
    // every effect instance gets a unique cid (client-side-id)
    cid = cidCounter;
    cidCounter++;
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
    duration = 3.0;
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

Stars::Stars(){
    type = EffectType::STARS;
    duration = 3.0;
}

void Stars::setup(Context &context){
    Effect::setup(context);

    #ifdef TARGET_OPENGLES
        shader.load("shaders_gles/Starfield01.vert","shaders_gles/Starfield01.frag");
    #else
        if(ofIsGLProgrammableRenderer()){
            shader.load("shaders_gl3/Starfield01.vert", "shaders_gl3/Starfield01.frag");
        }else{
            shader.load("shaders/Starfield01.vert", "shaders/Starfield01.frag");
        }
    #endif
}

void Stars::draw(Context &context){
    ofSetColor(255);
    shader.begin();
    ofDrawRectangle(0, 0, context.fbo->getWidth(), context.fbo->getHeight());
    shader.end();
}
