//
//  context.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-28.
//
//

#include "context.hpp"
//#include "xml_settings.hpp"

using namespace of2030;

void PreCalc::load(Context &_context){
    context = &_context;

    //    resolution = screen_setting.getValue("resolution", ofVec2f(768, 576));
    resolution.set(_context.fbo2->getWidth(), _context.fbo2->getHeight());
    color = _context.effect_setting.getValue("color", ofColor(255));
    
    scrWorldSize = _context.screen_setting.getValue("world_size", ofVec2f(2.67f, 2.0f));
    worldToScreenVec2f = resolution / scrWorldSize;

    scrPanoStart = _context.screen_setting.getValue("pano_start", 0.0f);
    scrPanoEnd = _context.screen_setting.getValue("pano_end", 1.0f);
    fxPanoStart = _context.effect_setting.getValue("pano_start", 0.0f);
    fxPanoEnd = _context.effect_setting.getValue("pano_end", 1.0f);

    // readTunnelScreenCoords(tunnelScreenCoords);

    // tunnel?
    if(_context.effect_setting.getValue("is_tunnel", "0") == "1"){
        translate = _context.screen_setting.getValue("tunnel_translate", ofVec3f(0.0f));
        rotate = _context.screen_setting.getValue("tunnel_rotate", ofVec3f(0.0f));
        scale = _context.screen_setting.getValue("tunnel_scale", ofVec3f(1.0f));
        scrDrawSize = _context.tunnel_size;
    // pano???
    } else if(_context.effect_setting.getValue("is_pano", "0") == "1"){
        translate = _context.screen_setting.getValue("pano_translate", ofVec3f(0.0f));
        rotate = _context.screen_setting.getValue("pano_rotate", ofVec3f(0.0f));
        scale = _context.screen_setting.getValue("pano_scale", ofVec3f(1.0f));
        scrDrawSize = _context.pano_size;
    // spot?
    } else if (_context.effect_setting.getValue("spot", 0) != 0){
        bIsSpot = true;
        string prefix = "spot" + _context.effect_setting.getValue("spot", "0");

        // convert from screen resolution (per-pixel) to real-world meters
        scale = ofVec3f(resolution/scrWorldSize);
        rotate = context->screen_setting.getValue(prefix+"rotate", ofVec3f(0.0f));
        // default value is way off screen; having no configuration for this spot,
        // means this spot should not be visible on this screen
        translate = context->screen_setting.getValue(prefix, ofVec3f(-100.0f, -100.0f, 0.0f));
        scrDrawSize = context->screen_setting.getValue(prefix+"size", ofVec2f(0.0f));
    // full screen
    } else {
        // not pano not tunnel; just do a regular "fullscreen"
        translate = _context.screen_setting.getValue("single_translate", ofVec3f(0.0f)) + ofVec3f(0.0f, -scrWorldSize.y, 0.0f);
        rotate = _context.screen_setting.getValue("single_rotate", ofVec3f(0.0f));
        scale = _context.screen_setting.getValue("single_scale", ofVec3f(resolution/scrWorldSize)) * ofVec3f(1.0f, -1.0f, 1.0f);
        scrDrawSize = scrWorldSize; //resolution; //_context.effect_setting.getValue("draw_size", scrWorldSize) * worldToScreenVec2f;
    }

    effect_translate = _context.effect_setting.getValue("translate", ofVec3f(0.0f));
    effect_rotate = _context.effect_setting.getValue("rotate", ofVec3f(0.0f));
    effect_scale = _context.effect_setting.getValue("scale", ofVec3f(1.0));
    scrDrawSize = _context.effect_setting.getValue("draw_size", scrDrawSize);
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

void PreCalc::readTunnelScreenCoords(ofVec2f *target){
    // read screen's config; it specifies which portion of the tunnel ("wall") is shown through this screen
    // screen corners respectively; top left, top right, bottom right, bottom left
    ofVec2f screenTunnelCoords[4];
    screenTunnelCoords[0] = context->screen_setting.getValue("tunnel_coord1", ofVec2f(0.0f, 0.0f));
    screenTunnelCoords[1] = context->screen_setting.getValue("tunnel_coord2", ofVec2f(1.0f, 0.0f));
    screenTunnelCoords[2] = context->screen_setting.getValue("tunnel_coord3", ofVec2f(1.0f, 1.0f));
    screenTunnelCoords[3] = context->screen_setting.getValue("tunnel_coord4", ofVec2f(0.0f, 1.0f));

    target[0] = -screenTunnelCoords[0] * resolution;
    target[1] = ofVec2f(1.0 / screenTunnelCoords[1].x, -screenTunnelCoords[1].y)  * resolution;
    target[2] = ofVec2f(1.0 / screenTunnelCoords[2].x, 1.0/screenTunnelCoords[2].y)  * resolution;
    target[3] = ofVec2f(-screenTunnelCoords[3].x, 1.0/screenTunnelCoords[3].y) * resolution;
}










