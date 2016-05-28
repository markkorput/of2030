//
//  context.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-28.
//
//

#include "context.hpp"

using namespace of2030;

void Context::precalc(){
    //    resolution = screen_setting.getValue("resolution", ofVec2f(768, 576));
    resolution.set(fbo2->getWidth(), fbo2->getHeight());
    
}

ofRectangle Context::panoDrawRect(){
    float scrStart = screen_setting.getValue("pano_start", 0.0f);
    float scrEnd = screen_setting.getValue("pano_end", 1.0f);
    float fxStart = effect_setting.getValue("pano_start", 0.0f);
    float fxEnd = effect_setting.getValue("pano_end", 1.0f);

    float minX = ofMap(fxStart, scrStart, scrEnd, 0.0, resolution.x);
    float maxX = ofMap(fxEnd, scrStart, scrEnd, 0.0, resolution.x);

    return ofRectangle(minX, 0.0, maxX-minX, resolution.y);
}

ofRectangle Context::tunnelDrawRect(){
    float scrStart = screen_setting.getValue("tunnel_start", 0.0f);
    float scrEnd = screen_setting.getValue("tunnel_end", 1.0f);
    float fxStart = effect_setting.getValue("tunnel_start", 0.0f);
    float fxEnd = effect_setting.getValue("tunnel_end", 1.0f);
    
    // start of tunnel
    float x1 = ofMap(fxStart, scrStart, scrEnd, 0.0, resolution.x);
    // start of visible part of tunnel
    float x2 = ofMap(fxEnd, scrStart, scrEnd, 0.0, resolution.x);
    // draw "hider" for invisible part _before_ visible part
    return ofRectangle(x1, 0.0, x2-x1, resolution.y);
}