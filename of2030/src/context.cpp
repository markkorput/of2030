//
//  context.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-28.
//
//

#include "context.hpp"

using namespace of2030;

PreCalc::PreCalc(Context &_context){
    context = &_context;

    //    resolution = screen_setting.getValue("resolution", ofVec2f(768, 576));
    resolution.set(_context.fbo2->getWidth(), _context.fbo2->getHeight());
    color = _context.effect_setting.getValue("color", ofColor(255));
    
    scrWorldSize = _context.screen_setting.getValue("world_size", ofVec2f(2.67f, 2.0f));
    worldToScreenVec2f = resolution / scrWorldSize;
    scrDrawSize = _context.effect_setting.getValue("draw_size", scrWorldSize) * worldToScreenVec2f;

    scrPanoStart = _context.screen_setting.getValue("pano_start", 0.0f);
    scrPanoEnd = _context.screen_setting.getValue("pano_end", 1.0f);
    fxPanoStart = _context.effect_setting.getValue("pano_start", 0.0f);
    fxPanoEnd = _context.effect_setting.getValue("pano_end", 1.0f);
}

ofRectangle PreCalc::panoDrawRect(){
    float scrStart = context->screen_setting.getValue("pano_start", 0.0f);
    float scrEnd = context->screen_setting.getValue("pano_end", 1.0f);
    float fxStart = context->effect_setting.getValue("pano_start", 0.0f);
    float fxEnd = context->effect_setting.getValue("pano_end", 1.0f);

    float minX = ofMap(fxPanoStart, scrPanoStart, scrPanoEnd, 0.0, resolution.x);
    float maxX = ofMap(fxPanoEnd, scrPanoStart, scrPanoEnd, 0.0, resolution.x);

    return ofRectangle(minX, 0.0, maxX-minX, resolution.y);
}

ofRectangle PreCalc::tunnelDrawRect(){
    float scrStart = context->screen_setting.getValue("tunnel_start", 0.0f);
    float scrEnd = context->screen_setting.getValue("tunnel_end", 1.0f);
    float fxStart = context->effect_setting.getValue("tunnel_start", 0.0f);
    float fxEnd = context->effect_setting.getValue("tunnel_end", 1.0f);

    // start of tunnel
    float x1 = ofMap(fxStart, scrStart, scrEnd, 0.0, resolution.x);
    // start of visible part of tunnel
    float x2 = ofMap(fxEnd, scrStart, scrEnd, 0.0, resolution.x);
    // draw "hider" for invisible part _before_ visible part
    return ofRectangle(x1, 0.0, x2-x1, resolution.y);
}