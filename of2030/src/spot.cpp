//
//  spot.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#include "spot.hpp"

using namespace of2030::effects;

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