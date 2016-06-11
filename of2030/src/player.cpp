//
//  player.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "player.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(Player)

Player::Player() : m_time(0.0f)/*, m_lastUpdateTime(0.0f), m_bPlaying(false)*/{
}

void Player::setup(){
    active_effects_manager.setSortByLayerAscending(true);
}

void Player::update(float dt){
    m_time += dt;

    const vector<Effect*> *effects = &active_effects_manager.getEffects();
    Effect* effect;
    // First, remove active effects that have ended
    for(int i=effects->size()-1; i>=0; i--){
        effect= (*effects)[i];
        
        if(effectEnded(*effect)){
            ofLog() << "[Player] effect ended: " << effect->trigger;
            // remove from active list
            active_effects_manager.remove(effect);
            // remove altogether
            effect_manager.remove(effect);
        }
    }

    effects = &pending_effects_manager.getEffects();

    // Second, activate pending effects that have started
    for(int i=effects->size()-1; i>=0; i--){
        effect= (*effects)[i];
        
        if(effectStarted(*effect)){
            pending_effects_manager.remove(effect);
            active_effects_manager.add(effect);
        }
    }
}

void Player::addEffect(Effect &effect){
    // this triggers renderer to call setup on the effect (and providing
    // it with the necessary data)
    ofLog() << "Player::addEffect with trigger " << effect.trigger;

    // Renderer monitors add events on this effect manager and runs effect->setup
    // (prividing the necessary data through a Context instance)
    effect_manager.add(&effect);

//    // dead on arrival?
//    if(effectEnded(effect)){
//        ofLog() << "Dead-on-arrival: " << effect.trigger;
//        // this triggers cleanups
//        effect_manager.remove(&effect);
//        return;
//    }

    // put it in the right "folder"
    if(effectStarted(effect)){
        active_effects_manager.add(&effect);
    } else {
        pending_effects_manager.add(&effect);
    }
}

inline bool Player::effectStarted(const Effect &effect) const {
    return !effect.hasStartTime() || effect.getStartTime() <= m_time;
}

inline bool Player::effectEnded(const Effect &effect) const {
    return effect.hasEndTime() && effect.getEndTime() <= m_time;
}

void Player::clearEffects(){
    effect_manager.clear();
    pending_effects_manager.clear();
    active_effects_manager.clear();
}

void Player::stopEffectByTrigger(string &trigger){
    // ofLogVerbose() << "Player::stopEffectByTrigger with " << trigger;

    const vector<Effect*> *effects = &active_effects_manager.getEffects();
    Effect * effect;

    // ofLogWarning() << "Player::stopEffectByTrigger active effects: " << effects->size();
    for(int i=effects->size()-1; i>=0; i--){
        effect = (*effects)[i];

        // if the pecified trigger string is empty; stop all effects
        if(effect->trigger == trigger || trigger == ""){
            // ofLogWarning() << "an active";
            ofLog() << "Stopped active (by trigger): " << effect->trigger;
            active_effects_manager.remove(effect);
            effect_manager.remove(effect);
        }
    }
    
    effects = &pending_effects_manager.getEffects();
    for(int i=effects->size()-1; i>=0; i--){
        effect = (*effects)[i];

        if(effect->trigger == trigger){
            //  ofLogWarning() << "a pending";
            ofLog() << "Stopped pending (by trigger): " << effect->trigger;
            pending_effects_manager.remove(effect);
            effect_manager.remove(effect);
        }
    }
}


void Player::stopEffectsByVideoPlayer(ofVideoPlayer *player){
    const vector<Effect*> *effects = &active_effects_manager.getEffects();
    Effect *effect;

    // ofLogWarning() << "Player::stopEffectByTrigger active effects: " << effects->size();
    for(int i=effects->size()-1; i>=0; i--){
        effect = (*effects)[i];

        // if the pecified trigger string is empty; stop all effects
        if(effect->getVideoPlayer() == player || effect->getMaskVideoPlayer() == player){
            ofLog() << "Stopped active (by vid): " << effect->trigger;
            active_effects_manager.remove(effect);
            effect_manager.remove(effect);
        }
    }

    effects = &pending_effects_manager.getEffects();
    for(int i=effects->size()-1; i>=0; i--){
        effect = (*effects)[i];

        if(effect->getVideoPlayer() == player || effect->getMaskVideoPlayer() == player){
            ofLog() << "Stopped pending (by vid): " << effect->trigger;
            pending_effects_manager.remove(effect);
            effect_manager.remove(effect);
        }
    }
}

void Player::stopEffectsByImage(ofImage &image){
    const vector<Effect*>* effects = &active_effects_manager.getEffects();
    Effect* effect;

    for(int i=effects->size()-1; i>=0; i--){
        effect = (*effects)[i];

        if(effect->getImage() == &image || effect->getMaskImage() == &image){
            ofLog() << "Stopped active (by img): " << effect->trigger;
            active_effects_manager.remove(effect);
            effect_manager.remove(effect);
        }
    }

    effects = &pending_effects_manager.getEffects();

    for(int i=effects->size()-1; i>=0; i--){
        effect = (*effects)[i];

        if(effect->getImage() == &image || effect->getMaskImage() == &image){
            ofLog() << "Stopped pending (by img): " << effect->trigger;
            pending_effects_manager.remove(effect);
            effect_manager.remove(effect);
        }
    }
}
