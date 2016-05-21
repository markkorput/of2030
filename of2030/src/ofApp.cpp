#include "ofApp.h"

#include "shared2030.h"

#ifdef __MULTI_CLIENT_ENABLED__
    #include "multi_client.hpp"
#endif

#include "interface.hpp"
#include "osc_receiver.hpp"
#include "xml_configs.hpp"
#include "xml_triggers.hpp"
#include "shader_manager.hpp"
#include "xml_settings.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "interface_player_bridge.hpp"
#include "effect_manager.hpp"
#include "video_manager.hpp"
#include "osc_playback_manager.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofLogVerbose() << "Redirect logging to log.txt";
    ofLogToFile("log.txt", true);

    // load settings xml
    ofLogVerbose() << "Loading settings.xml";
    of2030::XmlSettings::instance()->load();

    // apply log-level setting
    ofLogVerbose() << "Set log level based on setting: " << of2030::XmlSettings::instance()->log_level_name;
    ofSetLogLevel(of2030::XmlSettings::instance()->log_level);

    // load effects xml
    ofLogVerbose() << "Loading effects.xml";
    of2030::XmlEffects::instance()->load();

    // load screens xml
    ofLogVerbose() << "Loading screens.xml";

#ifndef __MULTI_CLIENT_ENABLED__
    // our screens.xml loader only needs to load the configuration for our own screen
    of2030::XmlConfigs::screens()->setNameFilter(of2030::XmlSettings::instance()->client_id);
#endif
    of2030::XmlConfigs::screens()->load();

    // load triggers xml
    ofLogVerbose() << "Loading triggers.xml";
    of2030::XmlTriggers::instance()->load();

    // load and start player
    ofLogVerbose() << "Starting player";
    of2030::Player::instance()->start();

    // This bridge updates the player with new effects, songnames and clipnames
    // when events on the interface are triggered
    // it auto-initializes with the interface and player singleton instances
    ofLogVerbose() << "Setting up InterfacePlayerBridge";
    of2030::InterfacePlayerBridge::instance()->setup();


#ifdef __MULTI_CLIENT_ENABLED__
    ofLogVerbose() << "Setting up MultiClient";
    of2030::MultiClient::instance()->setup();

    // only load singleton renderer when not in multi-mode
    if(!of2030::MultiClient::instance()->enabled){
        // Load renderer
        ofLogNotice() << "MultiClient not enabled, setting up singleton renderer";
        of2030::Renderer::instance()->setClientId(of2030::XmlSettings::instance()->client_id);
        of2030::Renderer::instance()->setup();
    }
#else
    ofLogVerbose() << "Setting up Renderer";
    of2030::Renderer::instance()->setClientId(of2030::XmlSettings::instance()->client_id);
    of2030::Renderer::instance()->setup();
#endif

    ofAddListener(of2030::Interface::instance()->controlEvent, this, &ofApp::onControl);

    // load & start OscReceiver; let the messages come!
    ofLogVerbose() << "Starting OscReceiver";
    of2030::OscReceiver::instance()->configure(of2030::XmlSettings::instance()->osc_setting);
    of2030::OscReceiver::instance()->setup();

    of2030::OscPlaybackManager::instance()->start("clock_spot.csv");
    ofClear(0);
}

//--------------------------------------------------------------
void ofApp::update(){
    of2030::OscPlaybackManager::instance()->update();
    of2030::OscReceiver::instance()->update();
    of2030::Player::instance()->update();
    of2030::VideoManager::instance()->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef __MULTI_CLIENT_ENABLED__
    if(of2030::MultiClient::instance()->enabled){
        of2030::MultiClient::instance()->draw();
    } else {
        of2030::Renderer::instance()->draw();
    }
#else
    of2030::Renderer::instance()->draw();
#endif
}

//--------------------------------------------------------------
void ofApp::exit(ofEventArgs &args){
    // TODO; call delete_instance for all singleton instance implementations
    of2030::EfficientEffectManager::delete_instance();
    of2030::VideoManager::delete_instance();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


void ofApp::onControl(string &type){

    if(type == CTRL_RELOAD_SHADERS){
        ofLog() << "reloading shader";
        of2030::Player::instance()->clearEffects();
        of2030::ShaderManager::instance()->clear();
        return;
    }

    if(type == CTRL_RELOAD_EFFECTS){
        ofLog() << "reloading effects";
        of2030::XmlTriggers::instance()->load();
        of2030::XmlConfigs::instance()->load();
        return;
    }

    if(type == CTRL_RELOAD_SCREENS){
        ofLog() << "reloading screens";
        of2030::XmlConfigs::screens()->load();
#ifdef __MULTI_CLIENT_ENABLED__
        of2030::MultiClient::instance()->setup();
#endif
        return;
    }

    if(type == CTRL_RELOAD_SETTINGS){
        ofLog() << "reloading settings";
        of2030::XmlSettings::instance()->load(true);
        ofSetLogLevel(of2030::XmlSettings::instance()->log_level);
        of2030::OscReceiver::instance()->configure(of2030::XmlSettings::instance()->osc_setting);
#ifdef __MULTI_CLIENT_ENABLED__
        of2030::MultiClient::instance()->setup();
#endif
        return;
    }

    if(type == CTRL_RELOAD_CLIENTS){
        ofLogWarning() << "reload clients is deprecated";
        // of2030::XmlClients::instance()->load();
        return;
    }
}
