//
//  effect_creator.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "effect_creator.hpp"
#include "effects.hpp"

using namespace of2030;

EffectCreator::EffectCreator(){
    setInterface(Interface::instance());
    m_player = Player::instance();
}

void EffectCreator::setInterface(Interface *interface){
    if(m_interface){
        // unsubscribe from new effect model events of previous interface
        ofRemoveListener(m_interface->effectEvent, this, &EffectCreator::onEffect);
    }

    m_interface = interface;

    if(m_interface){
        // subscribe to new effect model events of specified interface
        ofAddListener(m_interface->effectEvent, this, &EffectCreator::onEffect);
    }
}

// callback to process new effect events from the interface
void EffectCreator::onEffect(effects::Effect &effect){
    // finally, add the effect instance to the realtime_composition of the player
    m_player->realtime_composition.add(&effect);
    ofLog() << effect.type << "-type effect added to player's realtime composition";
    //ofLog() << "number: " << effect.type;
}

