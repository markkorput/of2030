//
//  composition.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "composition.hpp"

using namespace of2030;

void Composition::add(effects::Effect *effect){
    m_effects.push_back(effect);
    ofNotifyEvent(newEffectEvent, *effect, this);
}