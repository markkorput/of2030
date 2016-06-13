#include "ofApp.h"

#ifdef __MULTI_CLIENT_ENABLED__
    #include "multi_client.hpp"
#endif

#include "interface.hpp"
#include "osc_receiver.hpp"
#include "xml_configs.hpp"
#include "shader_manager.hpp"
#include "xml_settings.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "interface_player_bridge.hpp"
#include "effect_manager.hpp"
#include "video_manager.hpp"
#include "image_manager.hpp"
#include "osc_recorder.hpp"
#include "osc_sender.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofLogVerbose() << "Redirect logging to log.txt";
    ofLogToFile("log.txt", true);
    ofLog() << "window size: " << ofGetWidth() << "x" << ofGetHeight();
    
#ifdef __HIDE_CURSOR__
    ofHideCursor();
#endif

#ifdef __SET_DATA_ROOT_PATH__
    ofSetDataPathRoot("data/");
#endif

    // load settings xml
    ofLogVerbose() << "Loading settings.xml";
    if(!of2030::XmlSettings::instance()->load()){
        ofLogError() << "Could not load settings.xml";
        std::exit(1);
    }

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

    // load and start player
    ofLogVerbose() << "Starting player";
    of2030::Player::instance()->setup();

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

#ifdef __OSC_SENDER_ENABLED__
    of2030::OscSender::instance()->setup(of2030::XmlSettings::instance()->osc_out_host,
                                         of2030::XmlSettings::instance()->osc_out_port);
    osc_out_playback_manager.setToOscSender(true);
#endif // __OSC_SENDER_ENABLED__

    ofAddListener(of2030::Interface::instance()->controlEvent, this, &ofApp::onControl);
    ofAddListener(of2030::Interface::instance()->playbackEvent, this, &ofApp::onPlayback);
    ofAddListener(of2030::Interface::instance()->stopPlaybackEvent, this, &ofApp::onStopPlayback);
    ofAddListener(of2030::Interface::instance()->loadVideoEvent, this, &ofApp::onLoadVideo);
    ofAddListener(of2030::Interface::instance()->unloadVideoEvent, this, &ofApp::onUnloadVideo);
    ofAddListener(of2030::VideoManager::instance()->unloadEvent, this, &ofApp::onVideoPlayerUnload);
    ofAddListener(of2030::Interface::instance()->loadImageEvent, this, &ofApp::onLoadImage);
    ofAddListener(of2030::Interface::instance()->unloadImageEvent, this, &ofApp::onUnloadImage);
    ofAddListener(of2030::ImageManager::instance()->unloadEvent, this, &ofApp::onImageUnload);

    // load & start OscReceiver; let the messages come!
    ofLogVerbose() << "Starting OscReceiver";
    of2030::OscReceiver::instance()->configure(of2030::XmlSettings::instance()->osc_setting);
    of2030::OscReceiver::instance()->setup();
    
    // using the player's time as main timing mechanism
    ofClear(0);
    
    log_alive_interval = of2030::XmlSettings::instance()->log_alive_interval;
    last_update_time = ofGetElapsedTimef();
    next_log_alive_time = last_update_time;
}

//--------------------------------------------------------------
void ofApp::update(){
    float t = ofGetElapsedTimef() - last_update_time;
    float dt = t-last_update_time;
    last_update_time=t;

    of2030::OscPlaybackManager::instance()->update(dt);
    #ifdef __OSC_SENDER_ENABLED__
        osc_out_playback_manager.update(dt);
    #endif
    of2030::OscReceiver::instance()->update();
    of2030::Player::instance()->update(dt);
    of2030::VideoManager::instance()->update();

    // time for a new log message to indicate aliveness
    if(t >= next_log_alive_time){
        ofLog() << "alive time (s): " << t;
        next_log_alive_time = t + log_alive_interval;
//        vector<of2030::Effect*> fxs = of2030::Player::instance()->getActiveEffects();
//        for(int i=fxs.size()-1; i>=0; i--){
//            ofLog() << "fx: " << fxs[i]->trigger;
//        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

#ifdef __MULTI_CLIENT_ENABLED__
    if(of2030::MultiClient::instance()->enabled){
        of2030::MultiClient::instance()->draw();
    } else {
        of2030::Renderer::instance()->draw();
    }
#else
    of2030::Renderer::instance()->draw();
#endif

#ifdef __OSC_RECORDER_ENABLED__
    if(of2030::OscRecorder::instance()->is_recording()){
        ofSetColor(255, 0, 0);
        ofDrawRectangle(0,0, 10, 10);
    }
#endif
}

//--------------------------------------------------------------
void ofApp::exit(ofEventArgs &args){
    ofLog() << "shutting down...\n";

    // TODO; call delete_instance for all singleton instance implementations
    of2030::EfficientEffectManager::delete_instance();
    of2030::ShaderManager::delete_instance();
    of2030::VideoManager::delete_instance();
    of2030::ImageManager::delete_instance();

#ifdef __OSC_RECORDER_ENABLED__
    of2030::OscRecorder::delete_instance();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
#ifdef __OSC_RECORDER_ENABLED__
    if(key == 'r'){
        of2030::OscRecorder::instance()->toggle_record();
    }
#endif

#ifdef __MULTI_CLIENT_ENABLED__
    if(key >= '0' && key <= '9'){
        of2030::MultiClient::instance()->setPreviewClient(key-'0');
        return;
    }
    if(key ==45){ // minus/dash
        of2030::MultiClient::instance()->setPreviewClient(-1);
        return;
    }
    if(key == OF_KEY_LEFT){
        int idx = of2030::MultiClient::instance()->getPreviewClient()-1;
        if(idx < -1){
            idx = of2030::MultiClient::instance()->getClientCount()-1;
        }
        of2030::MultiClient::instance()->setPreviewClient(idx);
        return;
    }
    if(key == OF_KEY_RIGHT){
        int idx = of2030::MultiClient::instance()->getPreviewClient()+1;
        if(idx >= of2030::MultiClient::instance()->getClientCount()){
            idx=-1;
        }
        of2030::MultiClient::instance()->setPreviewClient(idx);
        return;
    }
#endif
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
#ifdef __DRAGNDROP__
    for(auto file: dragInfo.files){
        // if sender is enabled; user must hold the control key to send out
        #ifdef __OSC_SENDER_ENABLED__
            if(of2030::OscSender::instance()->isEnabled() && (ofGetKeyPressed(OF_KEY_COMMAND) || !of2030::XmlSettings::instance()->osc_out_keycheck)){
                osc_out_playback_manager.start(file);
                return;
            }
        #endif

        // default behaviour; just execute locally
        of2030::OscPlaybackManager::instance()->start(file);
    }
#endif // __DRAGNDROP__
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
        of2030::XmlConfigs::instance()->load(true);
        return;
    }

    if(type == CTRL_RELOAD_SCREENS){
        ofLog() << "reloading screens";
        of2030::XmlConfigs::screens()->load(true);
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

void ofApp::onPlayback(string &name){
    of2030::OscPlaybackManager::instance()->start(name);
}

void ofApp::onStopPlayback(string &name){
    of2030::OscPlaybackManager::instance()->stop(name);
}

void ofApp::onLoadVideo(string &name){
    of2030::VideoManager::instance()->get(name, true);
}

void ofApp::onUnloadVideo(string &name){
    // every video that gets unloaded in the VideoManager triggers the onVideoPlayerUnload callback below
    // which takes care of destroying any effects that might still be using the unloaded player(s)
    if(name == ""){
        of2030::VideoManager::instance()->deprecateAll();
    } else {
        of2030::VideoManager::instance()->deprecate(name);
    }
}

void ofApp::onVideoPlayerUnload(ofVideoPlayer &player){
    of2030::Player::instance()->stopEffectsByVideoPlayer(&player);
}

void ofApp::onLoadImage(string &name){
    of2030::ImageManager::instance()->get(name, true);
}

void ofApp::onUnloadImage(string &name){
    if(name == ""){
        of2030::ImageManager::instance()->unloadAll();
    } else{
        of2030::ImageManager::instance()->unload(name);
    }
}

void ofApp::onImageUnload(ofImage &image){
    of2030::Player::instance()->stopEffectsByImage(image);
}