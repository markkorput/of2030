//
//  voice.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#include "voice.hpp"
#include "shader_manager.hpp"

using namespace of2030::effects;


Voice::Voice(){
    setType(EffectType::VOICE);
}

void Voice::setup(Context &context){
    Effect::setup(context);
    if(shader == NULL){
        shader = ShaderManager::instance()->get("voice");
    }
}
void Voice::draw(Context &context){
    // screen must be voice-enabled
    if(!(context.screen_setting.hasValue("voice_x1") &&
         context.screen_setting.hasValue("voice_y1"))){
        return;
    }
    
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
    
    ofVec2f coords[4];
    coords[0] = ofVec2f(context.screen_setting.getValue("voice_x1", 0.0f), context.screen_setting.getValue("voice_y1", 0.0f)) * resolution;
    coords[1] = ofVec2f(context.screen_setting.getValue("voice_x2", 1.0f), context.screen_setting.getValue("voice_y2", 0.1f)) * resolution;
    coords[2] = ofVec2f(context.screen_setting.getValue("voice_x3", 1.0f), context.screen_setting.getValue("voice_y3", 0.1f)) * resolution;
    coords[3] = ofVec2f(context.screen_setting.getValue("voice_x4", 0.0f), context.screen_setting.getValue("voice_y4", 0.0f)) * resolution;
    
    // create mask
    context.fbo2->begin();
    ofBackground(0);
    ofSetColor(255);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[1].x, coords[1].y, coords[2].x, coords[2].y);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[2].x, coords[2].y, coords[3].x, coords[3].y);
    context.fbo2->end();
    
    EffectLogic logic(this, &context);
    
    // draw
    shader->begin();
    shader->setUniformTexture("iVoiceMask", context.fbo2->getTexture(), 1);
    
    float prog = logic.getGlobalProgress();
    float w = context.fbo->getWidth() * prog;
    
    ofSetColor(255);
    
    ofDrawRectangle((context.fbo->getWidth()-w)*0.5,
                    0.0f,
                    w,
                    context.fbo->getHeight());
    shader->end();
    
}
