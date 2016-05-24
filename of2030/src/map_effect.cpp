//
//  map_effect.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-24.
//
//

#include "map_effect.hpp"
#include "shader_manager.hpp"

using namespace of2030::effects;

MapEffect::MapEffect() : maskShaderName("mask"), coordsName("mapping_coord"){
}

void MapEffect::setup(Context &context){
    Effect::setup(context);

    this->maskShader = ShaderManager::instance()->get(maskShaderName);
}

void MapEffect::draw(Context &context){
    // screen must be enabled for our mapping
    if(!context.screen_setting.hasValue(coordsName+"1_x"))
        return;

    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());

    ofVec2f coords[4];
    coords[0] = context.screen_setting.getValue(coordsName+"1", ofVec2f(0.1f, 0.1f)) * resolution;
    coords[1] = context.screen_setting.getValue(coordsName+"2", ofVec2f(0.9f, 0.1f)) * resolution;
    coords[2] = context.screen_setting.getValue(coordsName+"3", ofVec2f(0.9f, 0.3f)) * resolution;
    coords[3] = context.screen_setting.getValue(coordsName+"4", ofVec2f(0.1f, 0.3f)) * resolution;

    // create mask
    context.fbo2->begin();
    ofBackground(0);
    ofSetColor(255);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[1].x, coords[1].y, coords[2].x, coords[2].y);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[2].x, coords[2].y, coords[3].x, coords[3].y);
    context.fbo2->end();
    
    EffectLogic logic(this, &context);

    // draw

    maskShader->begin();
    maskShader->setUniformTexture("iMask", context.fbo2->getTexture(), 1);

    ofFloatColor clr = context.effect_setting.getValue("color", ofFloatColor(255));
    float clrparts[4] = {clr.r, clr.g, clr.b, clr.a};
    ofSetColor(clr);
    
    maskShader->setUniform4fv("iColor", clrparts);
    ofDrawRectangle(0.0f, 0.0f, resolution.x, resolution.y);
    maskShader->end();

    context.fbo2->draw(0.0f, 0.0f);
}
