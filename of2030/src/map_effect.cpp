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
    if(!context.screen_setting.hasValue(coordsName+"_x"))
        return;

    //ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());

    ofVec2f coords[4];
    coords[0] = context.screen_setting.getValue(coordsName+"1", ofVec2f(0.1f, 0.1f));
    coords[1] = context.screen_setting.getValue(coordsName+"2", ofVec2f(0.9f, 0.1f));
    coords[2] = context.screen_setting.getValue(coordsName+"3", ofVec2f(0.9f, 0.3f));
    coords[3] = context.screen_setting.getValue(coordsName+"4", ofVec2f(0.1f, 0.3f));

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
    shader->setUniformTexture("iMask", context.fbo2->getTexture(), 1);

    ofColor clr = context.effect_setting.getValue("color", ofColor(255));
    ofSetColor(clr);
    ofDrawRectangle(0.0f, 0.0f, context.fbo->getWidth(), context.fbo->getHeight());
    shader->end();
}
