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

Effect::Effect(){
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
    mask_video_player = NULL;
    layer = 0;
}


void Effect::update(float dt){
    pano_pos += pano_velocity * dt;
    auto_pos += auto_velocity * dt;
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
    if(!hasEndTime() && _context.effect_setting.hasValue("duration")){
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
            if(_context.effect_setting.getValue("reset", "0") == "1"){
                video_player->setPosition(0.0);
            }

            video_player->play();
        } else {
            ofLogWarning() << "Effect::setup could not get video player for " << val;
            ofLog() << "setting effect duration to zero";
            // this will effectively abort the effect
            truncate();
        }
    }

    // load/start/configure video_mask if specified
    val = _context.effect_setting.getValue("video_mask", "");
    if(val != ""){
        // load video (get video player instance from the VideoManager)
        mask_video_player = VideoManager::instance()->get(val, _context.effect_setting.getValue("video_mask_alias", val), true);

        if(mask_video_player){
            if(_context.effect_setting.getValue("video_mask_loop", "0") == "1"){
                // TODO; this player might currently be used by other effects?
                mask_video_player->setLoopState(OF_LOOP_NORMAL);
            } else {
                // set none-looping
                mask_video_player->setLoopState(OF_LOOP_NONE);
            }

            // reset to start of video (this video player might have been used already by other effects
            if(_context.effect_setting.getValue("video_mask_reset", "0") == "1"){
                mask_video_player->setPosition(0.0);
            }

            mask_video_player->play();
        }/* else {
            ofLogWarning() << "Effect::setup could not get video player for " << val;
            ofLog() << "setting effect duration to zero";
            // this will effectively abort the effect
            truncate();
        }*/
    }

    // load any shaders based shader param
    val = _context.effect_setting.getValue("shader", "");
    if(val != ""){
        this->shader = ShaderManager::instance()->get(val);
    }

    auto_pos = _context.effect_setting.getValue("auto_pos", ofVec3f(0.0f));
    auto_velocity = _context.effect_setting.getValue("auto_velocity", ofVec3f(0.0f));

    // TODO; remove
    pano_pos = _context.effect_setting.getValue("pano_pos", 0.0f);
    pano_velocity = _context.effect_setting.getValue("pano_velocity", 0.0f);

    layer = _context.effect_setting.getValue("layer", 0);
}

void Effect::draw(Context &_context){
    // make some context, logic and precalculate data available to the entire class
    PreCalc prec;
    prec.load(_context);
    context = &_context;
    precalc = &prec;
    string val;
    
    // this value can be updates during the effect (was already initialized in setup())
    auto_velocity = _context.effect_setting.getValue("auto_velocity", ofVec3f(0.0f));

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
        ofPushMatrix();
            drawContent();
        ofPopMatrix();
    context->fbo3->end();

    // draw content of fbo3 through mask of fbo2
    maskShader->begin();
        // pass mask texture to shader
        maskShader->setUniformTexture("iMask", context->fbo2->getTexture(), 2);
        // ofSetColor(precalc->color); / --> doesn't work because shader doesn't use this color
        context->fbo3->draw(0.0f, 0.0f);
    maskShader->end();

    context = NULL;
    precalc = NULL;
}

void Effect::drawContent(){
    string val;

    // ofTranslate(context->effect_setting.getValue("translate", ofVec3f(0.0)) * precalc->worldToScreenVec2f);
    // ofScale(context->effect_setting.getValue("scale", ofVec3f(1.0)));

    ofRotateX(precalc->rotate.x);
    ofRotateY(precalc->rotate.y);
    ofRotateZ(precalc->rotate.z);
    ofScale(precalc->scale);
    ofTranslate(precalc->translate);

    ofRotateX(precalc->effect_rotate.x);
    ofRotateY(precalc->effect_rotate.y);
    ofRotateZ(precalc->effect_rotate.z);
    ofScale(precalc->effect_scale);
    ofTranslate(precalc->effect_translate + auto_pos);
    

    // draw; video?
    if(video_player){
        if(!(video_player->isLoaded() && video_player->getTexture().isAllocated())){
            return;
        }
        
        // movie done?
        if(video_player->getIsMovieDone() && context->effect_setting.getValue("loop", "0") != "1"){
            // if effect is configured to free (at first or last frame)
            val = context->effect_setting.getValue("freeze", "");
            if(val == "first"){
                video_player->setPosition(0.0);
                //video_player->setPaused(true);
            // no freezing; end effect
            } else if(val == ""){
                truncate(); // end this effect
                return;
            }
        }

        bool bAlphaBlack = (context->effect_setting.getValue("alphablack", "0") == "1");
        ofShader* vidShader;

        if(mask_video_player){
            if(!(mask_video_player->isLoaded() && mask_video_player->getTexture().isAllocated())){
                return;
            }

            if(mask_video_player->getIsMovieDone() && context->effect_setting.getValue("video_mask_loop", "0") != "1"){
                float freeze_pos = context->effect_setting.getValue("video_mask_freeze", -1.0f);
                if(freeze_pos >= 0.0f){
                    mask_video_player->setPosition(freeze_pos);
                }
            }

            vidShader = ShaderManager::instance()->get("mask");
            vidShader->begin();
            vidShader->setUniformTexture("iMask", mask_video_player->getTexture(), 2);
        }

        // draw video texture
        video_player->draw(0.0, 0.0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        
        if(mask_video_player){
            vidShader->end();
        }

        return;
    }


    // not video

        
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
        ofSetColor(precalc->color);
        ofDrawRectangle(0.0f, 0.0f, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
    }

    if(shader){
        // deactivate shader
        shader->end();
    }
}

void Effect::drawPattern(const string &patternName){
    //
    // SPOT
    //

    if(patternName == "spot"){
        int spotNumber = context->effect_setting.getValue("spot", (int)1);
        string prefix = "spot" + ofToString(spotNumber);


        ofVec2f spotPos = context->screen_setting.getValue(prefix, ofVec2f(-10.0f)) * precalc->resolution;
        ofVec2f spotSize = context->screen_setting.getValue(prefix+"size", ofVec2f(0.0f)) * precalc->resolution;

        ofVec2f fxSpotPos = context->effect_setting.getValue("pos", ofVec2f(0.0f, 0.0f));

        // spot reposition according to effect setting (interpret as real-world-meters)
        spotPos += fxSpotPos / precalc->scrWorldSize * precalc->resolution;

        if(!shader){
            // draw without shader stuff
            // ofDrawRectangle(0, 0, precalc->resolution.x, precalc->resolution.y);
            ofDrawEllipse(0.0f, 0.0f, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
            return;
        }

        // shader is already activated, just add some params

        //    shader->setUniform2f("iResolution", precalc->resolution);
        shader->setUniform2f("iSpotPos", spotPos);
        shader->setUniform2f("iSpotSize", spotSize);
        shader->setUniform1f("iGain", context->effect_setting.getValue("gain", 1.0f));

        // quarter; 1 means top right, 2 means bottom right, 3 bottom left, 4 means top left, zero means none
        int q = context->effect_setting.getValue("quarter_on", 0);
        shader->setUniform1i("iQuarterOn", q);
        q = context->effect_setting.getValue("quarter_off", 0);
        shader->setUniform1i("iQuarterOff", q);

        spotPos = spotPos - spotSize * 0.5;
        ofDrawRectangle(0.0f, 0.0f, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
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
