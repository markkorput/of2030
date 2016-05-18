//
//  vid.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#include "vid.hpp"
#include "video_manager.hpp"

using namespace of2030::effects;

Vid::Vid(){
    setType(EffectType::VID);
}

void Vid::setup(Context &context){
    // generic effect stuff
    Effect::setup(context);
    
    // load video (get video player instance from the VideoManager)
    video_player = VideoManager::instance()->get(context.effect_setting.getValue("video_file", "fingers.mov"), true);

    if(context.effect_setting.getValue("loop", "0") == "1"){
        // TODO; this player might currently be used by other effects?
        video_player->setLoopState(OF_LOOP_NORMAL);
    }

    video_player->play();
    
    //Set up vertices
    mesh.addVertex(ofPoint(0.0f, 0.0f)); // top left
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex_x1", 0.0f),
                             context.screen_setting.getValue("pano_tex_y1", 0.0f)));
    mesh.addColor(ofColor(255, 255, 255));

    mesh.addVertex(ofPoint(context.fbo->getWidth(), 0.0f)); // top right
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex_x2", 1.0f),
                             context.screen_setting.getValue("pano_tex_y2", 0.0f)));
    mesh.addColor(ofColor(255, 255, 255));
    
    mesh.addVertex(ofPoint(context.fbo->getWidth(), context.fbo->getHeight())); // bottom right
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex_x3", 1.0f),
                             context.screen_setting.getValue("pano_tex_y3", 1.0f)));
    mesh.addColor(ofColor(255, 255, 255));
    
    mesh.addVertex(ofPoint(0.0f, context.fbo->getHeight())); // bottom left
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex_x4", 0.0f),
                             context.screen_setting.getValue("pano_tex_y4", 1.0f)));
    mesh.addColor(ofColor(255, 255, 255));

    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(0, 2, 3);
    
    vidtex.allocate(video_player->getWidth(), video_player->getHeight(), GL_RGBA);
}

void Vid::draw(Context &context){
    
    //    video_player->draw(0,0);
    vidtex.bind();
    ofClear(255,255,255, 0);
    video_player->draw(0,0);
//    ofEnableArbTex();
    vidtex.unbind();

    // bind vidtex
    vidtex.bind();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    ofTextureData texData = vidtex.getTextureData();
    if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB) {
        glScalef(vidtex.getWidth(), vidtex.getHeight(), 1.0f);
    } else {
        glScalef(vidtex.getWidth() / texData.tex_w, vidtex.getHeight() / texData.tex_h, 1.0f);
    }
    glMatrixMode(GL_MODELVIEW);

    //video_player->bind();
    ofSetColor(255);
//    ofDrawRectangle(0.0, 0.0, context.fbo->getWidth(), context.fbo->getHeight());
    mesh.draw();
    //video_player->unbind();
    
    // unbind
    vidtex.unbind();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}