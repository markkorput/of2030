//
//  player.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "player.hpp"

using namespace of2030;

Player* Player::singleton = NULL;

Player* Player::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new Player();
    }
    return singleton;
}

Player::Player() : m_time(0.0f), m_bPlaying(false), song("default"), clip("default"){
}

void Player::update(){
    if(m_bPlaying){
        setPlaybackTime(ofGetElapsedTimef() - m_startTime);
    }
}

void Player::update(float dt){
    if(m_bPlaying){
        setPlaybackTime(m_time + dt);
    }
}

void Player::start(){
    m_startTime = ofGetElapsedTimef();
    m_time = 0.0f;
    m_bPlaying = true;
}

void Player::stop(){
    m_bPlaying = false;
}

void Player::addEffect(effects::Effect &effect){
    // this triggers renderer to call setup on the effect (and providing
    // it with the necessary data)
    ofLogVerbose() << "Player::addEffect";
    effect_manager.add(&effect);

    // dead on arrival?
    if(effectEnded(effect)){
        // this triggers cleanups
        effect_manager.remove(&effect);
        return;
    }

    // put it in the right "folder"
    if(effectStarted(effect)){
        active_effects_manager.add(&effect);
    } else {
        pending_effects_manager.add(&effect);
    }
}

void Player::clearEffects(){
    effect_manager.clear();
    pending_effects_manager.clear();
    active_effects_manager.clear();
}

void Player::setPlaybackTime(float time){
    m_time = time;

    // First, remove active effects that have ended
    const vector<effects::Effect*> *effects = &active_effects_manager.getEffects();
    
    for(auto effect: (*effects)){
        if(effectEnded(*effect)){
            // remove from active list
            active_effects_manager.remove(effect);
            // remove altogether
            effect_manager.remove(effect);
        }
    }

    // Second, activate pending effects that have started
    effects = &pending_effects_manager.getEffects();

    for(auto effect: (*effects)){
        if(effectStarted(*effect)){
            pending_effects_manager.remove(effect);
            active_effects_manager.add(effect);
        }
    }
}

inline bool Player::effectStarted(const effects::Effect &effect){
    return !effect.hasStartTime() || effect.startTime <= m_time;
}

inline bool Player::effectEnded(const effects::Effect &effect){
    return effect.hasEndTime() && effect.endTime <= m_time;
}
