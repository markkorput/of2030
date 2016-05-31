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

    val = _context.effect_setting.getValue("operation", "");
    if(val != ""){
        if(val == "load_video"){
            val = _context.effect_setting.getValue("video", "");
            VideoManager::instance()->get(val, _context.effect_setting.getValue("video_alias", val), true);
        }

        if(val == "unload_video"){
            val = _context.effect_setting.getValue("video_alias", "");
            if(val == "")
                val = _context.effect_setting.getValue("video", "");
            VideoManager::instance()->unload(val);
        }
        
        truncate();
        return;
    }

    // make sure we have endTime and duration initialized AND consistent
    if(!hasEndTime() ){
        setDuration(_context.effect_setting.getValue("duration", 30.0f));
    }

    // load/start/configure video if specified
    val = _context.effect_setting.getValue("video", "");
    if(val != ""){
        // load video (get video player instance from the VideoManager)
        video_player = VideoManager::instance()->get(val, _context.effect_setting.getValue("video_alias", val), true);

        if(video_player){
            if(_context.effect_setting.getValue("loop", "0") == "1"){
                // TODO; this player might currently be used by other effects?
                video_player->setLoopState(OF_LOOP_NORMAL);
                // indefinite
                endTime = NO_TIME;
            } else {
                // set none-looping
                video_player->setLoopState(OF_LOOP_NONE);
                
                // wanna freeze on first or last frame when done? remove end time, go on indefinite
                if(_context.effect_setting.hasValue("freeze")){
                    endTime = NO_TIME;
                }
                
                // video_player->getDuration() is causing problems on PI (maybe its the animation codec)
                // we'll just truncate in the draw function, when the end of the file is reached (and we're not feezing)
                /* else if(getDuration() > video_player->getDuration()){
                    // make sure the effect's duration time is not longer than the non-looping video's duration
                    setDuration(video_player->getDuration());
                }*/
            }

            // reset to start of video (this video player might have been used already by other effects
            if(_context.effect_setting.getValue("reset", "1") == "1")
                video_player->setPosition(0.0);

            video_player->play();
        } else {
            ofLogWarning() << "Effect::setup could not get video player for " << val;
            ofLog() << "setting effect duration to zero";
            // this will effectively abort the effect
            truncate();
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
    string val;
    

    ofShader* maskShader = ShaderManager::instance()->get("mask");
    
    // draw 4-point coordinate mask in fbo2
    context->fbo2->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofSetColor(255);
    
        val = context->effect_setting.getValue("mask_coords_name", "");
        if(val != ""){
            // draw 4-point coordinates mask
            // (if no coordinates specfied, this will give a full white frame)
            drawMask(val);
        } else {
            ofRectangle rect = precalc->panoTunnelDrawRect();
            ofDrawRectangle(rect);
        }
    context->fbo2->end();

    // draw content to fbo3
    context->fbo3->begin();
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofSetColor(255);
        drawContent();
    context->fbo3->end();

    // draw content of fbo3 through mask of fbo2
    maskShader->begin();
        // pass mask texture to shader
        maskShader->setUniformTexture("iMask", context->fbo2->getTexture(), 2);
        // ofSetColor(precalc->color); / --> doesn't work because shader doesn't use this color
        context->fbo3->draw(0.0f, 0.0f);

//        // draw content from fbo3 (masked)
//        ofMesh mesh;
//        mesh.addVertex(ofPoint(0.0f, 0.0f)); // top left
//        mesh.addVertex(ofPoint(precalc->resolution.x, 0.0f)); // top right
//        mesh.addVertex(ofPoint(precalc->resolution.x, precalc->resolution.y)); // bottom right
//        mesh.addVertex(ofPoint(0.0f, precalc->resolution.y)); // bottom left
//
//        mesh.addTexCoord(ofVec2f(0.0f, 1.0f)*precalc->resolution);
//        mesh.addTexCoord(ofVec2f(1.0f, 1.0f)*precalc->resolution);
//        mesh.addTexCoord(ofVec2f(1.0f, 0.0f)*precalc->resolution);
//        mesh.addTexCoord(ofVec2f(0.0f, 0.0f)*precalc->resolution);
//
//        mesh.addTriangle(0, 1, 2);
//        mesh.addTriangle(0, 2, 3);
//
//        context->fbo3->getTexture().bind();
//        mesh.draw();
//        context->fbo3->getTexture().unbind();
    maskShader->end();

    context = NULL;
    precalc = NULL;
}

void Effect::drawContent(){
    string val;

    ofPushMatrix();
    // ofTranslate(context->effect_setting.getValue("translate", ofVec3f(0.0)) * precalc->worldToScreenVec2f);
    // ofScale(context->effect_setting.getValue("scale", ofVec3f(1.0)));

    ofRotateX(precalc->rotate.x);
    ofRotateY(precalc->rotate.y);
    ofRotateZ(precalc->rotate.z);
    ofScale(precalc->scale);
    ofTranslate(precalc->translate);
    

    // draw; video?
    if(video_player){
        if(video_player->isLoaded() && video_player->getTexture().isAllocated()){
            
            // movie done?
            if(video_player->getIsMovieDone() && context->effect_setting.getValue("loop", "0") != "1"){
                // if effect is configured to free (at first or last frame)
                val = context->effect_setting.getValue("freeze", "");
                if(val == "first"){
                    video_player->setPosition(0.0);
                // no freezing; end effect
                } else if(val == ""){
                    truncate(); // end this effect
                }
            }

            bool bAlphaBlack = (context->effect_setting.getValue("alphablack", "0") == "1");
            ofShader* vidShader;

            vidShader = ShaderManager::instance()->get("video");
            vidShader->begin();
            vidShader->setUniform1i("alphaBlack", bAlphaBlack ? 1 : 0);

            // ofBackground(255,0,0);
            drawVideo();
            
            vidShader->end();
        }
    // not video
    } else {
        
        if(shader){
            // activate shader
            shader->begin();
            
            // populate shader
            shader->setUniform2f("iResolution", precalc->resolution);
            shader->setUniform3f("iPos", context->effect_setting.getValue("pos", ofVec3f(0.0f)));
            shader->setUniform3f("iSize", context->effect_setting.getValue("size", ofVec3f(0.0f)));
            shader->setUniform1f("iProgress", getProgress());
            shader->setUniform1f("iDuration", getDuration());
            shader->setUniform2f("iScreenWorldSize", precalc->scrWorldSize);
            shader->setUniform1f("iGain", context->effect_setting.getValue("gain", 1.0f));
        }

        string pattern = context->effect_setting.getValue("pattern", "");

        // pattern?
        if(pattern != ""){
            drawPattern(pattern);
        // simple rectangle
        } else {
            ofDrawRectangle(0, 0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        }

        if(shader){
            // deactivate shader
            shader->end();
        }
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
//    string coord_prefix = "";
//    if(context->effect_setting.getValue("is_tunnel", "0") == "1"){
//        coord_prefix = "tunnel_coord";
//    } else if(context->effect_setting.getValue("is_pano", "0") == "1"){
//        coord_prefix = "pano_coord";
//    }

//    if(coord_prefix == ""){
        video_player->draw(0.0, 0.0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
    
//    }
    
//    // set up mesh with vertices and tex coords
//    ofVec2f vidSize = ofVec2f(video_player->getWidth(), video_player->getHeight());
//    ofVec2f coords[4];
//    coords[0] = context->screen_setting.getValue(coord_prefix+"1", ofVec2f(0.0f, 0.0f));
//    coords[1] = context->screen_setting.getValue(coord_prefix+"2", ofVec2f(1.0f, 0.0f));
//    coords[2] = context->screen_setting.getValue(coord_prefix+"3", ofVec2f(1.0f, 1.0f));
//    coords[3] = context->screen_setting.getValue(coord_prefix+"4", ofVec2f(0.0f, 1.0f));
//
//    ofMesh mesh;
//    mesh.addVertex(ofPoint(0.0f, 0.0f)); // top left
//    mesh.addVertex(ofPoint(precalc->resolution.x, 0.0f)); // top right
//    mesh.addVertex(ofPoint(precalc->resolution.x, precalc->resolution.y)); // bottom right
//    mesh.addVertex(ofPoint(0.0f, precalc->resolution.y)); // bottom left
//
//    mesh.addTexCoord(coords[0]*vidSize);
//    mesh.addTexCoord(coords[1]*vidSize);
//    mesh.addTexCoord(coords[2]*vidSize);
//    mesh.addTexCoord(coords[3]*vidSize);
//
//    mesh.addTriangle(0, 1, 2);
//    mesh.addTriangle(0, 2, 3);
//    
//    // bind video texture
//    video_player->bind();
//    ofSetColor(255);
//    mesh.draw();
//    video_player->unbind();
}

//float Effect::resolveDuration() const {
//    if(hasDuration())
//        return getDuration();
//
//    if(hasStartTime() and hasEndTime())
//        return endTime - startTime;
//
//    return -1.0;
//}
