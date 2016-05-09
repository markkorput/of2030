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
    // initialize with a single "OFF" effect in the queue
    active_effects.push_back(&m_offEffect);
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

bool Player::effectActive(effects::Effect &effect){
    // loop over all our active effects
    for(int i=active_effects.size()-1; i>=0; i--){
        effects::Effect* active_effect = active_effects[i];
        // current active effect has the specified client-id?
        if(active_effect->cid == effect.cid){
            return true;
        }
    }
    
    return false;
}

void Player::activateEffect(effects::Effect &effect){
    active_effects.push_back(&effect);
}

void Player::setPlaybackTime(float time){
    // TODO; activate all effects that start between m_time and time
    vector<effects::Effect*> effects = realtime_composition.getEffects();

    for(int i=effects.size()-1; i>=0; i--){
        effects::Effect* effect = effects[i];

        // current effect just got active?
        if((effect->hasStartTime() == false || effect->startTime < m_time) &&
           (effect->hasEndTime() == false || effect->endTime > m_time) &&
           !effectActive(*effect)){
            activateEffect(*effect);
        }
    }

    removeActiveEffectsEndingBefore(time);
    m_time = time;    
}

void Player::removeActiveEffectsEndingBefore(float time){
    // loop over all active effects (it's important to iterate in reverse order,
    // because during the iterations elements might be removed, which
    // mixes up the index values when iterated in forward order).
    for(int i=active_effects.size()-1; i>=0; i--){
        // take the current effect
        effects::Effect* effect = active_effects[i];

        // see if it has an end time that has been reached
        if(effect->hasEndTime() && effect->endTime <= time){
            // remove event from our active stack
            active_effects.erase(active_effects.begin() + i);

            // NOTE; this Player class does not get involed in the memory
            // management surrounding the events; they must be allocated
            // and freed from memory by the owner of this class

            // trigger event
            ofNotifyEvent(effectEndedEvent, *effect, this);
        }
    }
}
