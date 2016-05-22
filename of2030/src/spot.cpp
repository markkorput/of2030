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
    int spotNumber = context.effect_setting.getValue("number", (int)1);
    string prefix = "spot" + ofToString(spotNumber);

//    // screen must be spot-enabled
//    if(!(context.screen_setting.hasValue("spot"+ofToString(spotNumber)+"_x"))
//       return;

    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
    
    ofVec2f spotPos = context.screen_setting.getValue(prefix, ofVec2f(-10.0f)) * resolution;
    ofVec2f spotSize = context.screen_setting.getValue(prefix+"size", ofVec2f(0.0f)) * resolution;

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