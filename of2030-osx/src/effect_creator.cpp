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
        ofRemoveListener(m_interface->effects_collection.modelAddedEvent, this, &EffectCreator::onNewEffectModel);
    }

    m_interface = interface;

    if(m_interface){
        // subscribe to new effect model events of specified interface
        ofAddListener(m_interface->effects_collection.modelAddedEvent, this, &EffectCreator::onNewEffectModel);
    }
}

// callback to process new effect models on the interface
// based on the information in the effect model, we'll create
// an instane of the appropriate effect class
void EffectCreator::onNewEffectModel(CMS::Model &model){
    std::string type = model.get("type");

    if(type == "OFF"){
        effects::Off* effect = new effects::Off();
        m_player->realtime_composition.add(*(effects::Effect*)effect);
        return;
    }

    if(type == "COLOR"){
        effects::Color* effect = new effects::Color();
        effect->color.set(ofColor(
            ofToChar(model.get("r", "0")),
            ofToChar(model.get("g", "0")),
            ofToChar(model.get("b", "0"))));

        m_player->realtime_composition.add(*(effects::Effect*)effect);
        return;
    }

    ofLogWarning() << "[EffectCreator] got unknown effect model type: " << type;
}

