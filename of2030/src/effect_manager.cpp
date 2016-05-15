//
//  effect_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-15.
//
//

#include "effect_manager.hpp"

using namespace of2030;

SINGLETON_CLASS_IMPLEMENTATION_CODE(EffectManager)

EffectManager::~EffectManager(){
    for(auto & effect: active_effects){
        remove(effect);
    }
}

effects::Effect* EffectManager::get(string typ){
    return create(typ);
    // TODO; use (recycle) idle_effects
}

void EffectManager::finish(effects::Effect* effect){
    remove(effect);
}

effects::Effect* EffectManager::create(string typ){
    effects::Effect* pEffect;
    
    if(typ == "vid"){
        pEffect = (effects::Effect*) new effects::Vid();
    } else {
        // default type, just set name to whatever was specified
        pEffect = new effects::Effect();
        pEffect->name = typ;
    }

    // add new instance to our list
    active_effects.push_back(pEffect);
    return pEffect;
}

void EffectManager::remove(effects::Effect* effect){
    // find effect in our list
    for(int i=active_effects.size()-1; i>=0; i--){
        // this one?
        if(active_effects[i] == effect){
            // remove from list
            active_effects.erase(active_effects.begin()+i);

            // figure out effect type and delete from memory
            if(effect->type == effects::EffectType::VID){
                // turn into Vid effect pointer before deleting, to delete appropriate class type
                delete (effects::Vid*) effect;
                return;
            }

            if(effect->type == effects::EffectType::COLOR){
                delete (effects::Color*) effect;
                return;
            }

            // default; simply effects::Effect
            delete effect;
            return;
        }
    }

    // not found
    ofLogWarning() << "[EffectManager::remove] could not find specified effect in list, ignoring";
}