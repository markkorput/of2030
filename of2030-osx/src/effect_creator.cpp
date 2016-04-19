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
    effects::Effect* pEffect;
    std::string type = model.get("type");
    std::string value;

    // create an instance of the appropriate effects class
    // beased on the model's type attribute
    // and populate type-specific attributes
    // the assign the pEffect pointer to the created instance
    if(type == "OFF"){

        effects::Off* effect = new effects::Off();
        pEffect = (effects::Effect*)effect;

    } else if(type == "COLOR"){

        effects::Color* effect = new effects::Color();
        effect->color.set(ofColor(
            ofToChar(model.get("r", "0")),
            ofToChar(model.get("g", "0")),
            ofToChar(model.get("b", "0"))));
        pEffect = (effects::Effect*)effect;

    } else {

        ofLogWarning() << "[EffectCreator] got unknown effect model type: " << type;
        pEffect = new effects::Effect();

    }

    // process some more (optional) general effect attributes
    value = model.get("start");
    if(value != ""){
        pEffect->startTime = ofToFloat(value);
    }

    value = model.get("end");
    if(value != ""){
        pEffect->endTime = ofToFloat(value);
    } else {
        value = model.get("duration");
        if(value != ""){
            pEffect->endTime = m_player->getTime() + ofToFloat(value);
        }
    }

    // finally, add the effect instance to the realtime_composition of the player
    m_player->realtime_composition.add(pEffect);
//    ofLog() << type << "-type effect added to player's realtime composition";
//    ofLog() << "number: " << pEffect->type;
}

