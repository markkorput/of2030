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

using namespace of2030;

// int Effect::cidCounter = 0;

Effect::Effect() : video_player(NULL){
    reset();
    // setType
    type = DEFAULT;
    name = EFFECT_NAMES[type];

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
    video_player = NULL;
    layer = 0;
}


void Effect::update(float dt){
    pano_pos += pano_velocity * dt;
}

void Effect::setup(Context &_context){
    string val;

    // make sure we have a start time (default to NOW)
    if(!hasStartTime()){
        startTime = _context.time;
    }

    // make sure we have endTime and duration initialized AND consistent
    if(hasEndTime()){
        duration = endTime - startTime;
    } else {
        if(!hasDuration()){
            duration = _context.effect_setting.getValue("duration", 30.0f);
        }

        endTime = startTime + duration;
    }

    // load/start/configure video if specified
    val = _context.effect_setting.getValue("video", "");
    if(val != ""){
        // load video (get video player instance from the VideoManager)
        video_player = VideoManager::instance()->get(val, true);

        if(video_player){
            if(_context.effect_setting.getValue("loop", "0") == "1"){
                // TODO; this player might currently be used by other effects?
                video_player->setLoopState(OF_LOOP_NORMAL);
            } else {
                // set none-looping
                video_player->setLoopState(OF_LOOP_NONE);
                
                // make sure the effect's duration time is not longer than the non-looping video's duration
                if(duration > video_player->getDuration()){
                    duration = video_player->getDuration();
                    endTime = startTime + duration;
                }
            }

            // reset to start of video (this video player might have been used already by other effects
            if(_context.effect_setting.getValue("reset", "1") == "1")
                video_player->setPosition(0.0);

            video_player->play();
        } else {
            ofLogWarning() << "Effect::setup could not get video player for " << val;
            ofLog() << "setting effect duration to zero";
            // this will effectively abort the effect
            duration = 0.0f;
            startTime=_context.time-1.0f;
            endTime = startTime-1.0f;
        }
    }

    // load any shaders based shader param
    val = _context.effect_setting.getValue("shader", "");
    if(val != ""){
        this->shader = ShaderManager::instance()->get(val);
    }

    pano_pos = _context.effect_setting.getValue("pano_pos", 0.0f);
    pano_velocity = _context.effect_setting.getValue("pano_velocity", 0.0f);

    layer = _context.effect_setting.getValue("layer", 0);
}

void Effect::draw(Context &_context){
    // make some context, logic and precalculate data available to the entire class
    PreCalc prec(_context);
    context = &_context;
    precalc = &prec;

    ofShader* maskShader = ShaderManager::instance()->get("mask");
    
    // draw 4-point coordinates mask
    // (if no coordinates specfied, this will give a full white frame)
    context->fbo3->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofSetColor(255);
        drawMask(context->effect_setting.getValue("mask_coords_name", ""));
    context->fbo3->end();

    // draw pano (masked with above mask) into fbo2
    context->fbo2->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        maskShader->begin();
            maskShader->setUniformTexture("iMask", context->fbo3->getTexture(), 2);
            ofSetColor(255);
            ofRectangle rect = precalc->panoTunnelDrawRect();
            ofDrawRectangle(rect);
        maskShader->end();
    context->fbo2->end();

    // draw content to fbo3
    context->fbo3->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofSetColor(precalc->color);
        drawContent();
    context->fbo3->end();

    // draw content through above combined masks
    maskShader->begin();
        // pass mask texture to shader
        maskShader->setUniformTexture("iMask", context->fbo2->getTexture(), 2);
        // draw content from fbo3 (masked)
        ofSetColor(255);
        context->fbo3->draw(0.0f, 0.0f);
    maskShader->end();
    
    context = NULL;
    precalc = NULL;
}

void Effect::drawContent(){
    ofPushMatrix();
    ofTranslate(context->effect_setting.getValue("translate", ofVec3f(0.0)) * precalc->worldToScreenVec2f);
    ofScale(context->effect_setting.getValue("scale", ofVec3f(1.0)));

    if(shader){
        // activate shader
        shader->begin();

        // populate shader
        shader->setUniform2f("iResolution", precalc->resolution);
        shader->setUniform3f("iPos", context->effect_setting.getValue("pos", ofVec3f(0.0f)));
        shader->setUniform3f("iSize", context->effect_setting.getValue("size", ofVec3f(0.0f)));
        shader->setUniform1f("iProgress", getProgress());
        shader->setUniform1f("iDuration", duration);
        shader->setUniform2f("iScreenWorldSize", precalc->scrWorldSize);
        shader->setUniform1f("iGain", context->effect_setting.getValue("gain", 1.0f));
    }

    // draw; video?
    if(video_player){
        bool bAlphaBlack = (context->effect_setting.getValue("alphablack", "1") == "1");
        ofShader* vidShader;

        if(bAlphaBlack){
            vidShader = ShaderManager::instance()->get("video");
            vidShader->begin();
        }

        drawVideo();

        if(bAlphaBlack){
            vidShader->end();
        }
    // not video
    } else {
        string pattern = context->effect_setting.getValue("pattern", "");

        // pattern?
        if(pattern != ""){
            drawPattern(pattern);
        // simple rectangle
        } else {
            ofDrawRectangle(0, 0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        }
    }

    if(shader){
        // deactivate shader
        shader->end();
    }

    ofPopMatrix();
}

void Effect::drawPattern(const string &patternName){
    //
    // SPOT
    //

    if(patternName == "spot"){
        int spotNumber = context->effect_setting.getValue("number", (int)1);
        string prefix = "spot" + ofToString(spotNumber);

        ofVec2f spotPos = context->screen_setting.getValue(prefix, ofVec2f(-10.0f)) * precalc->resolution;
        ofVec2f spotSize = context->screen_setting.getValue(prefix+"size", ofVec2f(0.0f)) * precalc->resolution;

        ofVec2f fxSpotPos = context->effect_setting.getValue("pos", ofVec2f(0.0f, 0.0f));

        // spot reposition according to effect setting (interpret as real-world-meters)
        spotPos += fxSpotPos / precalc->scrWorldSize * precalc->resolution;

        if(!shader){
            // draw without shader stuff
            ofSetColor(255);
            // ofDrawRectangle(0, 0, precalc->resolution.x, precalc->resolution.y);
            ofDrawEllipse(spotPos.x, spotPos.y, spotSize.x, spotSize.y);
            return;
        }

        // shader is already activated, just add some params

        //    shader->setUniform2f("iResolution", precalc->resolution);
        shader->setUniform2f("iSpotPos", spotPos);
        shader->setUniform2f("iSpotSize", spotSize);
        shader->setUniform1f("iGain", context->effect_setting.getValue("gain", 1.0f));

        // quarter; 1 means top right, 2 means bottom right, 3 bottom left, 4 means top left, zero means none
        int q = std::floor(context->effect_setting.getValue("quarter_on", 0.0f));
        shader->setUniform1i("iQuarterOn", q);
        q = std::floor(context->effect_setting.getValue("quarter_off", 0.0f));
        shader->setUniform1i("iQuarterOff", q);

        spotPos = spotPos - spotSize * 0.5;
        ofSetColor(255);
        ofDrawRectangle(0.0f, 0.0f, precalc->resolution.x, precalc->resolution.y); //spotPos.x, spotPos.y, spotSize.x, spotSize.y);
        return;
    }

    //
    // POS
    //

    if(patternName == "pos"){
        ofVec3f lightpos = context->effect_setting.getValue("pos", ofVec3f(0.0f, 0.0f, 0.0f));
        float lightsize = context->effect_setting.getValue("size", 0.0f);
        ofVec3f mypos = context->screen_setting.getValue("pos", ofVec3f(0.0f, 0.0f, 0.0f));
        float onDistance = context->effect_setting.getValue("on_distance", 3.0f);
        float offDistance = context->effect_setting.getValue("off_distance", 9.0f);
        float distance = mypos.distance(lightpos) - lightsize;
        float gain = ofMap(distance, onDistance, offDistance, 255, 0, true);

        ofSetColor(255, 255, 255, gain);
        ofDrawRectangle(0, 0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        return;
    }

    //
    // CURSOR
    //

    if(patternName == "cursor"){
        float p = context->screen_setting.getValue("pano_pos", pano_pos);
        float x = ofMap(p - floor(p),
                            context->screen_setting.getValue("pano_start", 0.0f),
                            context->screen_setting.getValue("pano_end", 1.0f),
                            0.0,
                            precalc->scrDrawSize.x);

        float gain = context->effect_setting.getValue("gain", 1.0f) * precalc->scrDrawSize.x / precalc->scrWorldSize.x;
        ofDrawRectangle(x-gain*0.5, 0.0f, gain, precalc->scrDrawSize.y);
        return;
    }
}

void Effect::drawMask(const string &coordsName){
    ofVec2f coords[4];

    coords[0] = context->screen_setting.getValue(coordsName+"1", ofVec2f(0.0f, 1.0f)) * precalc->resolution;
    coords[1] = context->screen_setting.getValue(coordsName+"2", ofVec2f(1.0f, 1.0f)) * precalc->resolution;
    coords[2] = context->screen_setting.getValue(coordsName+"3", ofVec2f(1.0f, 0.0f)) * precalc->resolution;
    coords[3] = context->screen_setting.getValue(coordsName+"4", ofVec2f(0.0f, 0.0f)) * precalc->resolution;

    // draw mask content
    ofDrawTriangle(coords[0].x, coords[0].y, coords[1].x, coords[1].y, coords[2].x, coords[2].y);
    ofDrawTriangle(coords[0].x, coords[0].y, coords[2].x, coords[2].y, coords[3].x, coords[3].y);
}

void Effect::drawVideo(){
    // this is checked by the caller
    //    if(!video_player){
    //        return;
    //    }

    float x = 0.0;
    if(context->effect_setting.hasValue("pano_pos")){
        x = precalc->panoWorldToScreen(context->effect_setting.getValue("pano_pos", 0.0f));
    }

    if(!context->effect_setting.hasValue("is_pano")){
        video_player->draw(x, precalc->scrDrawSize.y, precalc->scrDrawSize.x, -precalc->scrDrawSize.y);
    } else {
        // set up mesh with vertices and tex coords
        ofMesh mesh;
        mesh.addVertex(ofPoint(x, 0.0f)); // top left
        mesh.addVertex(ofPoint(x+precalc->scrDrawSize.x, 0.0f)); // top right
        mesh.addVertex(ofPoint(x+precalc->scrDrawSize.x, precalc->scrDrawSize.y)); // bottom right
        mesh.addVertex(ofPoint(x, precalc->scrDrawSize.y)); // bottom left

        // specify which part of the video texture to show (default values specify the full frame)
        ofVec2f vidSize = ofVec2f(video_player->getWidth(), video_player->getHeight());
        mesh.addTexCoord(ofPoint(context->screen_setting.getValue("pano_coord1", ofVec2f(0.0f, 0.0f)) * vidSize));
        mesh.addTexCoord(ofPoint(context->screen_setting.getValue("pano_coord2", ofVec2f(1.0f, 0.0f)) * vidSize));
        mesh.addTexCoord(ofPoint(context->screen_setting.getValue("pano_coord3", ofVec2f(1.0f, 1.0f)) * vidSize));
        mesh.addTexCoord(ofPoint(context->screen_setting.getValue("pano_coord4", ofVec2f(0.0f, 1.0f)) * vidSize));

        mesh.addTriangle(0, 1, 2);
        mesh.addTriangle(0, 2, 3);

        // bind video texture
        video_player->bind();
        ofSetColor(255);
        mesh.draw();
        video_player->unbind();
    }
}

//float Effect::resolveDuration() const {
//    if(hasDuration())
//        return duration;
//
//    if(hasStartTime() and hasEndTime())
//        return endTime - startTime;
//
//    return -1.0;
//}
