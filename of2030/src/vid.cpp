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
}

void Vid::draw(Context &context){
    // set up mesh with vertices and tex coords
    ofMesh mesh;
    mesh.addVertex(ofPoint(0.0f, context.fbo->getHeight())); // top left
    mesh.addVertex(ofPoint(context.fbo->getWidth(), context.fbo->getHeight())); // top right
    mesh.addVertex(ofPoint(context.fbo->getWidth(), 0.0f)); // bottom right
    mesh.addVertex(ofPoint(0.0f, 0.0f)); // bottom left

    ofVec2f vidSize = ofVec2f(video_player->getWidth(), video_player->getHeight());

    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex1pos", ofVec2f(0.0f, 0.0f)) * vidSize));
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex2pos", ofVec2f(0.0f, 0.0f)) * vidSize));
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex3pos", ofVec2f(0.0f, 0.0f)) * vidSize));
    mesh.addTexCoord(ofPoint(context.screen_setting.getValue("pano_tex4pos", ofVec2f(0.0f, 0.0f)) * vidSize));

    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(0, 2, 3);
    
    // bind video texture
    video_player->bind();
    ofSetColor(255);
    mesh.draw();
    video_player->unbind();
}