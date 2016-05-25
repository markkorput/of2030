//
//  pos.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-22.
//
//

#include "pos.hpp"

using namespace of2030::effects;

Pos::Pos(){
    setType(EffectType::POS);
}

//void Vid::setup(Context &context){
//}

void Pos::draw(Context &context){
    ofVec3f lightpos = context.effect_setting.getValue("pos", ofVec3f(0.0f, 0.0f, 0.0f));
    float lightsize = context.effect_setting.getValue("size", 0.0f);
    ofVec3f mypos = context.screen_setting.getValue("pos", ofVec3f(0.0f, 0.0f, 0.0f));
    float onDistance = context.effect_setting.getValue("on_distance", 3.0f);
    float offDistance = context.effect_setting.getValue("off_distance", 9.0f);
    float distance = mypos.distance(lightpos) - lightsize;
    float gain = ofMap(distance, onDistance, offDistance, 255, 0, true);
    ofSetColor(255, 255, 255, gain);
    ofDrawRectangle(0, 0, context.fbo->getWidth(), context.fbo->getHeight());
    ofSetColor(0);
    drawTunnelMask(context);
}