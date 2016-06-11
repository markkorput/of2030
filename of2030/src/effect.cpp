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
#include "image_manager.hpp"

using namespace of2030;

// int Effect::cidCounter = 0;

Effect::Effect(){
    reset();

    // every effect instance gets a unique cid (client-side-id)
    // cid = cidCounter;
    // cidCounter++;
}

void Effect::reset(){
    startTime = NO_TIME;
    endTime = NO_TIME;
    trigger = "";
    video_player = NULL;
    mask_video_player = NULL;
    image = NULL;
    mask_image=NULL;
    layer = 0;
    bUnique = true;
    blendMode = OF_BLENDMODE_ADD;
    bUnload=false;
}


//void Effect::update(float dt){
//
//}

void Effect::setup(Context &_context){
    string val;
    int vali;
    
    vali = _context.effect_setting.getValue("blendmode", -1);
    if(vali != -1){
        switch(vali){
            case 0: blendMode = OF_BLENDMODE_DISABLED; break;
            case 1: blendMode = OF_BLENDMODE_ALPHA; break;
            case 2: blendMode = OF_BLENDMODE_ADD; break;
            case 3: blendMode = OF_BLENDMODE_SUBTRACT; break;
            case 4: blendMode = OF_BLENDMODE_MULTIPLY; break;
            case 5: blendMode = OF_BLENDMODE_SCREEN;
        }
    }

    if(_context.effect_setting.getValue("unload", "0") == "1"){
        bUnload = true;
    }

    // make sure we have a start time (default to NOW)
    if(!hasStartTime()){
        startTime = _context.time;
    }

    //
    // video
    //
    video_player = getVideoPlayer(_context);
    if(video_player){
        bVidStarted = false;

        if(_context.effect_setting.getValue("loop", "0") == "1"){
            // TODO; this player might currently be used by other effects?
            video_player->setLoopState(OF_LOOP_NORMAL);
            // indefinite
            endTime = NO_TIME;
        } else {
            // set none-looping
            video_player->setLoopState(OF_LOOP_NONE);
            
            // wanna freeze on a frame when done? remove end time, go on indefinite
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
        
        if(!(video_player->isLoaded() && video_player->getTexture().isAllocated())){
            video_player->play();
            bVidStarted=true;
        } else{
            bVidStarted=false;
        }
    } /*else {
        ofLogWarning() << "Effect::setup could not get video player for " << val;
        ofLog() << "setting effect duration to zero";
        // this will effectively abort the effect
        truncate();
    }*/

    //
    // video mask
    //
    val = _context.effect_setting.getValue("video_mask", "");
    if(val != ""){
        if(val == "self"){
            mask_video_player = video_player;
        } else {
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
    }

    // make sure we have endTime and duration initialized AND consistent
    if(_context.effect_setting.hasValue("duration")){
        setDuration(_context.effect_setting.getValue("duration", 30.0f));
    }

    //
    // load image if specified
    //
    image = loadImage(_context);
    if(image && _context.effect_setting.getValue("image_mask", "") == "self"){
        mask_image = image;
    };

    bUnique = _context.effect_setting.getValue("unique", "1") == "1";

    auto_pos = _context.effect_setting.getValue("auto_pos", ofVec3f(0.0f));
    auto_rotation = _context.effect_setting.getValue("auto_rotation", ofVec3f(0.0f));
    auto_scale = _context.effect_setting.getValue("initial_scale", ofVec3f(0.0f));
    auto_alpha = _context.effect_setting.getValue("initial_alpha", 1.0f);

    layer = _context.effect_setting.getValue("layer", 0);
}

void Effect::draw(Context &_context, float dt){
    if(_context.effect_setting.getValue("visible", "1") != "1"){
        return;
    }

    // make some context, logic and precalculate data available to the entire class
    PreCalc prec;
    prec.load(_context);
    context = &_context;
    precalc = &prec;
    
    string val;

    // these value can be updates during the effect (were already initialized in setup)
    auto_pos += _context.effect_setting.getValue("auto_velocity", ofVec3f(0.0f)) * dt;
    auto_rotation += _context.effect_setting.getValue("auto_rotate", ofVec3f(0.0f)) * dt;
    auto_scale += _context.effect_setting.getValue("auto_scale", ofVec3f(0.0f)) * dt;
    auto_alpha = _context.effect_setting.getValue("current_alpha", auto_alpha);
    auto_alpha += _context.effect_setting.getValue("auto_alpha", 0.0f) * dt;

    ofSetColor(255);

    // draw 4-point coordinate mask in fbo2
    context->fbo2->begin();
        ofClear(0.0f, 0.0f, 0.0f, 255.0f);
    
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
    
    context->fbo3->begin();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        // draw content to fbo3
        ofClear(0.0f, 0.0f, 0.0f, 0.0f);
        ofPushMatrix();
            drawContent();
        ofPopMatrix();
    
        // draw mask with blend mode multiply (essentially only leaving the parts where the mask is white)
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        context->fbo2->draw(0.0f, 0.0f);
    context->fbo3->end();

    // draw the resulting content in fbo 3 with our standard shader (allows for texture-tiling, colorizing and alpha-fading)
    ofShader* effectShader = ShaderManager::instance()->get("standard");

    // draw content of fbo3 through mask of fbo2
    effectShader->begin();
        // pass mask texture to shader
        effectShader->setUniform4f("iColor", precalc->color);
        effectShader->setUniform1f("iAlpha", auto_alpha);
        effectShader->setUniform2f("iResolution", precalc->resolution);
        // ofSetColor(precalc->color); // --> doesn't work because shader doesn't use this color
        effectShader->setUniform2f("iTexCoordMultiply", ofVec2f(1.0f));
        effectShader->setUniform2f("iTexCoordOffset", ofVec2f(0.0f));
        context->fbo3->draw(0.0f, 0.0f);
    effectShader->end();

    context = NULL;
    precalc = NULL;
}

void Effect::drawContent(){
    string val;
    ofShader* vidShader;

    // ofTranslate(context->effect_setting.getValue("translate", ofVec3f(0.0)) * precalc->worldToScreenVec2f);
    // ofScale(context->effect_setting.getValue("scale", ofVec3f(1.0)));

    ofScale(precalc->scale);
    ofTranslate(precalc->translate);
    ofRotateX(precalc->rotate.x);
    ofRotateY(precalc->rotate.y);
    ofRotateZ(precalc->rotate.z);

    ofScale(precalc->effect_scale+auto_scale);
    ofTranslate(precalc->effect_translate + auto_pos);
    ofRotateX(precalc->effect_rotate.x + auto_rotation.x);
    ofRotateY(precalc->effect_rotate.y + auto_rotation.y);
    ofRotateZ(precalc->effect_rotate.z + auto_rotation.z);

    if(precalc->bIsSpot){
        ofSetRectMode(OF_RECTMODE_CENTER);
    }else{
        ofSetRectMode(OF_RECTMODE_CORNER);
    }
    
    // draw image?

    if(image){
        if(mask_image){
            vidShader = ShaderManager::instance()->get("mask");
            vidShader->begin();
            vidShader->setUniformTexture("iMask", mask_image->getTexture(), 2);
        } else {
            vidShader = ShaderManager::instance()->get("standard");
            vidShader->begin();
        }

        vidShader->setUniform4f("iColor", ofColor::white);
        vidShader->setUniform1f("iAlpha", 1.0f);
        vidShader->setUniform2f("iTexCoordMultiply", context->effect_setting.getValue("texcoord_multiply", ofVec2f(1.0f, 1.0f)));
        vidShader->setUniform2f("iTexCoordOffset", context->effect_setting.getValue("texcoord_offset", ofVec2f(0.0f)));
        vidShader->setUniform2f("iResolution", ofVec2f(image->getWidth(), image->getHeight()));

        image->draw(0.0, 0.0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        vidShader->end();
        return;
    }

    // draw; video?

    if(video_player){
        if(!(video_player->isLoaded() && video_player->getTexture().isAllocated())){
            ofLogVerbose() << "Effect::draw: video not yet loaded";
            return;
        }
        
        if(!bVidStarted){
            video_player->play();
            bVidStarted=true;
        }

        // movie done?
        if(video_player->getIsMovieDone() && context->effect_setting.getValue("loop", "0") != "1"){
            // if effect is configured to freeze (at certain position, usually first or last frame)
            float freeze_pos = context->effect_setting.getValue("freeze", (float)-1.0);
            if(freeze_pos >= 0.0f){
                video_player->setPosition(freeze_pos);
                video_player->setPaused(true);
            // no freezing; end effect
            } else {
                truncate(); // end this effect
                return;
            }
        }

        bool bAlphaBlack = (context->effect_setting.getValue("alphablack", "0") == "1");
        

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
        } else {
            vidShader = ShaderManager::instance()->get("standard");
            vidShader->begin();
        }

        vidShader->setUniform4f("iColor", ofColor::white);
        vidShader->setUniform1f("iAlpha", 1.0f);
        vidShader->setUniform2f("iTexCoordMultiply", context->effect_setting.getValue("texcoord_multiply", ofVec2f(1.0f, 1.0f)));
        vidShader->setUniform2f("iTexCoordOffset", context->effect_setting.getValue("texcoord_offset", ofVec2f(0.0f)));
        vidShader->setUniform2f("iResolution", ofVec2f(video_player->getTexture().getWidth(), video_player->getTexture().getHeight()));

        // draw video texture

        video_player->draw(0.0, 0.0, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        vidShader->end();
        return;
    }

    // not video, pattern?
    val = context->effect_setting.getValue("pattern", "");

    if(val != ""){
        drawPattern(val);
    }

    // don't draw anything by default; we don't want 'accidental' effects appear
}

void Effect::drawPattern(const string &patternName){
    if(patternName == "rect"){
        ofDrawRectangle(0.0f, 0.0f, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
        return;
    }

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


        // draw without shader stuff
        // ofDrawRectangle(0, 0, precalc->resolution.x, precalc->resolution.y);
        ofDrawEllipse(0.0f, 0.0f, precalc->scrDrawSize.x, precalc->scrDrawSize.y);
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
        ofSetColor(255);
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

inline ofVideoPlayer* Effect::getVideoPlayer(Context &contxt){
    string val = contxt.effect_setting.getValue("video", "");

    if(val != ""){
        // load video (get video player instance from the VideoManager)
        return VideoManager::instance()->get(val, contxt.effect_setting.getValue("video_alias", val), true);
    }

    
    int i=1;
    while(contxt.effect_setting.getValue("video_option"+ofToString(i), "") != "")
        i++;

    if(i == 1)
        return NULL;

    val = contxt.effect_setting.getValue("video_option"+ofToString(floor(ofRandom(i))), "");
    return VideoManager::instance()->get(val, contxt.effect_setting.getValue("video_alias", val), true);
}

inline ofImage* Effect::loadImage(Context &contxt){
    string val = contxt.effect_setting.getValue("image", "");

    if(val != ""){
        return ImageManager::instance()->get(val,
                                      contxt.effect_setting.getValue("image_alias", val),
                                      true);
    }

    int i=1;
    while(contxt.effect_setting.getValue("image_option"+ofToString(i), "") != "")
        i++;
    
    if(i == 1)
        return NULL;
    
    val = contxt.effect_setting.getValue("image_option"+ofToString(floor(ofRandom(i))), "");
    return ImageManager::instance()->get(val, contxt.effect_setting.getValue("image_options_alias"+ofToString(floor(ofRandom(i))), val), true);
}