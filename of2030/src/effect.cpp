//
//  effect.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "effects.hpp"
#include "shader_manager.hpp"
#include "video_manager.hpp"

using namespace of2030::effects;

// int Effect::cidCounter = 0;

Effect::Effect(){
    reset();
    setType(DEFAULT);

    // every effect instance gets a unique cid (client-side-id)
    // cid = cidCounter;
    // cidCounter++;
}

void Effect::reset(){
    startTime = NO_TIME;
    endTime = NO_TIME;
    duration = NO_TIME;
    trigger = "";
    shader = NULL;
}

void Effect::setup(Context &context){
    string val;

    if(!hasStartTime()){
        startTime = context.time;
    }

    duration = context.effect_setting.getValue("duration", 30.0f);

    if(hasDuration() && hasStartTime() && !hasEndTime()){
        endTime = startTime + duration;
    }
    
    // load any shaders based on comma-seperated "shaders" effect setting
    val = context.effect_setting.getValue("shader", "");
    if(val != ""){
        this->shader = ShaderManager::instance()->get(val);
    }

    // load/start/configure video if specified
    val = context.effect_setting.getValue("video", "");
    if(val != ""){
        // load video (get video player instance from the VideoManager)
        ofVideoPlayer* video_player = VideoManager::instance()->get(val, true);

        if(context.effect_setting.getValue("loop", "0") == "1"){
            // TODO; this player might currently be used by other effects?
            video_player->setLoopState(OF_LOOP_NORMAL);
        }
        
        video_player->play();
    }

}

void Effect::draw(Context &context){
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());

    // draw content to fbo3
    context.fbo3->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofColor clr = context.effect_setting.getValue("color", ofColor(255));
        ofSetColor(clr);
        drawContent(context);
        // tunnel 'mask'
        ofSetColor(0);
        drawTunnelMask(context);
        drawPanoMask(context);
    context.fbo3->end();

    // draw alpha mask (if no mask specfied, this will give a full white frame)
    context.fbo2->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofSetColor(255);
        drawMask(context, context.effect_setting.getValue("mask_coords_name", ""), resolution);
    context.fbo2->end();

    // apply alpha mask through mask shader
    ofShader* maskShader = ShaderManager::instance()->get("mask");
    maskShader->begin();
        // pass mask texture to shader
        maskShader->setUniformTexture("iMask", context.fbo2->getTexture(), 2);
        // draw content from fbo3 (masked)
        ofSetColor(255);
        context.fbo3->draw(0.0f, 0.0f, resolution.x, resolution.y);
    maskShader->end();
}

void Effect::drawContent(Context &context){
    EffectLogic logic((Effect*)this, &context);
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
    
    if(!shader){
        // draw without shader stuff
        string vid = context.effect_setting.getValue("video", "");
        if(vid == ""){
            ofDrawRectangle(0, 0, resolution.x, resolution.y);
        } else {
            ofSetColor(255);
            drawVideo(context, vid);
        }
    } else {
        // activate shader
        shader->begin();

        // populate shader
        shader->setUniform2f("iResolution", resolution);
        shader->setUniform3f("iPos", context.effect_setting.getValue("pos", ofVec3f(0.0f)));
        shader->setUniform3f("iSize", context.effect_setting.getValue("size", ofVec3f(0.0f)));
        shader->setUniform2f("iScreenWorldSize", ofVec2f(context.screen_setting.getValue("world_width", 2.67f),
                                                         context.screen_setting.getValue("world_height", 2.0f)));
        shader->setUniform3f("iScreenPos", context.screen_setting.getValue("pos", ofVec3f(0.0f)));

        shader->setUniform1f("iProgress", logic.getGlobalProgress());
        shader->setUniform1f("iDuration", logic.getGlobalDuration());
        shader->setUniform1f("iIterations", context.effect_setting.getValue("iterations", 1.0f));
        
        shader->setUniform1f("iScreenPanoStart", context.screen_setting.getValue("pano_start", 0.0f));
        shader->setUniform1f("iScreenPanoEnd", context.screen_setting.getValue("pano_end", 1.0f));
        shader->setUniform1f("iEffectPanoStart", context.effect_setting.getValue("pano_start", 0.0f));
        shader->setUniform1f("iEffectPanoEnd", context.effect_setting.getValue("pano_end", 1.0f));
        
        shader->setUniform1f("iScreenTunnelStart", context.screen_setting.getValue("tunnel_start", 0.0f));
        shader->setUniform1f("iScreenTunnelEnd", context.screen_setting.getValue("tunnel_end", 1.0f));
        shader->setUniform1f("iEffectTunnelStart", context.effect_setting.getValue("tunnel_start", 0.0f));
        shader->setUniform1f("iEffectTunnelEnd", context.effect_setting.getValue("tunnel_end", 1.0f));
        
        shader->setUniform1f("iGain", context.effect_setting.getValue("gain", 1.0f));
        
        // draw
        
        string vid = context.effect_setting.getValue("video", "");
        if(vid == ""){
            ofDrawRectangle(0, 0, resolution.x, resolution.y);
        } else {
            drawVideo(context, vid);
        }
        
        // deactivate shader
        shader->end();
    }
}

void Effect::drawMask(Context &context, const string &coordsName, const ofVec2f &resolution){
    ofVec2f coords[4];
    coords[0] = context.screen_setting.getValue(coordsName+"1", ofVec2f(0.0f, 1.0f)) * resolution;
    coords[1] = context.screen_setting.getValue(coordsName+"2", ofVec2f(1.0f, 1.0f)) * resolution;
    coords[2] = context.screen_setting.getValue(coordsName+"3", ofVec2f(1.0f, 0.0f)) * resolution;
    coords[3] = context.screen_setting.getValue(coordsName+"4", ofVec2f(0.0f, 0.0f)) * resolution;

    // draw mask content
    ofDrawTriangle(coords[0].x, coords[0].y, coords[1].x, coords[1].y, coords[2].x, coords[2].y);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[2].x, coords[2].y, coords[3].x, coords[3].y);
}

void Effect::drawTunnelMask(Context &context){
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());

    float scrTunnelStart = context.screen_setting.getValue("tunnel_start", 0.0f);
    float scrTunnelEnd = context.screen_setting.getValue("tunnel_end", 1.0f);
    float fxTunnelStart = context.effect_setting.getValue("tunnel_start", 0.0f);
    float fxTunnelEnd = context.effect_setting.getValue("tunnel_end", 1.0f);
    float pixPerTunnel = resolution.x / std::abs(scrTunnelEnd-scrTunnelStart);

//    if(scrTunnelStart > scrTunnelEnd){
//        // swap
//        float tmp = fxTunnelStart;
//        fxTunnelStart = fxTunnelEnd;
//        fxTunnelEnd = tmp;
//    }
//
//    float x = (fxTunnelStart-scrTunnelStart)*pixPerTunnel;
//    ofDrawRectangle(0.0, 0.0, x, resolution.y);
//
//    x = (fxTunnelEnd-scrTunnelStart)*pixPerTunnel;
//    ofDrawRectangle(x, 0.0, resolution.x-x, resolution.y);

    if(scrTunnelStart > scrTunnelEnd){
        // swap
        float tmp = fxTunnelStart;
        fxTunnelStart = fxTunnelEnd;
        fxTunnelEnd = tmp;
    }

    float x = ofMap(fxTunnelStart, scrTunnelStart, scrTunnelEnd, 0.0, resolution.x);
//    float x = (fxTunnelStart-scrTunnelStart)*pixPerTunnel;
    ofDrawRectangle(0.0, 0.0, x, resolution.y);
    x = ofMap(fxTunnelEnd, scrTunnelStart, scrTunnelEnd, 0.0, resolution.x);
//    x = (fxTunnelEnd-scrTunnelStart)*pixPerTunnel;
    
    ofDrawRectangle(x, 0.0, resolution.x-x, resolution.y);
}

void Effect::drawPanoMask(Context &context){
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
    
    float scrStart = context.screen_setting.getValue("pano_start", 0.0f);
    float scrEnd = context.screen_setting.getValue("pano_end", 1.0f);
    float fxStart = context.effect_setting.getValue("pano_start", 0.0f);
    float fxEnd = context.effect_setting.getValue("pano_end", 1.0f);
    float pixPerUnit = resolution.x / (scrEnd-scrStart);
    
//    float minX = ofMap(fxStart, scrStart, scrEnd, 0.0, resolution.x);
    float minX = (fxStart-scrStart)*pixPerUnit;
    float maxX = (fxEnd-scrStart)*pixPerUnit;

    ofDrawRectangle(0.0, 0.0, minX, resolution.y);
    ofDrawRectangle(maxX, 0.0, resolution.x-maxX, resolution.y);
}

void Effect::drawVideo(Context &context, const string &video){
    ofVec2f resolution(context.fbo->getWidth(), context.fbo->getHeight());
    ofVideoPlayer *video_player = of2030::VideoManager::instance()->get(video, true);

    if(context.effect_setting.getValue("is_pano", 0.0f) > 0.0f){
        video_player->draw(0.0f, 0.0f, resolution.x, resolution.y);
    } else {
        // set up mesh with vertices and tex coords
        ofMesh mesh;
        mesh.addVertex(ofPoint(0.0f, context.fbo->getHeight())); // top left
        mesh.addVertex(ofPoint(context.fbo->getWidth(), context.fbo->getHeight())); // top right
        mesh.addVertex(ofPoint(context.fbo->getWidth(), 0.0f)); // bottom right
        mesh.addVertex(ofPoint(0.0f, 0.0f)); // bottom left


        // specify which part of the video texture to show (default values specify the full frame)
        ofVec2f vidSize = ofVec2f(video_player->getWidth(), video_player->getHeight());
        mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_coord1", ofVec2f(0.0f, 0.0f)) * vidSize));
        mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_coord2", ofVec2f(1.0f, 0.0f)) * vidSize));
        mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_coord3", ofVec2f(1.0f, 1.0f)) * vidSize));
        mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_coord4", ofVec2f(0.0f, 1.0f)) * vidSize));

        mesh.addTriangle(0, 1, 2);
        mesh.addTriangle(0, 2, 3);

        // bind video texture
        video_player->bind();
        ofSetColor(255);
        mesh.draw();
        video_player->unbind();
    }
}

float Effect::getDuration() const {
    if(hasDuration())
        return duration;

    if(hasStartTime() and hasEndTime())
        return endTime - startTime;

    return -1.0;
}

void Effect::setType(EffectType effect_type){
    type = effect_type;
    name = EFFECT_NAMES[effect_type];
}


// === === === === === === === === ===


Tunnel::Tunnel(){
    setType(EffectType::TUNNEL);
}

// virtual void setup(Context &context);
void Tunnel::draw(Context &context){
    float tunnelStart = context.screen_setting.getValue("tunnel_start", 0.0f);
    float tunnelEnd = context.screen_setting.getValue("tunnel_end", 1.0f);
}

