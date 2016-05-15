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
    for(auto & effect: effects){
        deleteEffect(effect);
    }

    effects.clear();
}

effects::Effect* EffectManager::get(string typ){
    // create new instance
    effects::Effect* pEffect = createEffect(typ);
    // add to our internal list
    add(pEffect);
    // return it to caller
    return pEffect;
}

// returns a pointer (or NULL if not found) to an allocated effect instance
// of the specified type in the internal idle_effects vector
effects::Effect* EffectManager::findByType(effects::EffectType typ){
    for(int i=effects.size()-1; i>=0; i--){
        if(effects[i]->type == typ){
            // found!
            return effects[i];
        }
    }

    // not found
    return NULL;
}

void EffectManager::add(effects::Effect* effect){
    effects.push_back(effect);
}

effects::EffectType EffectManager::typeStringToType(string typ){
    if(typ == "vid") return effects::EffectType::VID;
    if(typ == "off") return effects::EffectType::OFF;
    if(typ == "color") return effects::EffectType::COLOR;
    return effects::EffectType::DEFAULT;
}

effects::Effect* EffectManager::createEffect(string typ){
    effects::Effect* pEffect;
    
    if(typ == "vid"){
        pEffect = (effects::Effect*) new effects::Vid();
    } else {
        // default type, just set name to whatever was specified
        pEffect = new effects::Effect();
        pEffect->name = typ;
    }

    return pEffect;
}

bool EffectManager::remove(effects::Effect* effect){
    // find effect in our list
    for(int i=effects.size()-1; i>=0; i--){
        // this one?
        if(effects[i] == effect){
            // remove from list
            effects.erase(effects.begin()+i);
            // done
            return true;
        }
    }

    // not found
    ofLogWarning() << "[EffectManager::remove] could not find specified effect in list, ignoring";
    return false;
}

void EffectManager::deleteEffect(effects::Effect* effect){
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
}




//
// EfficientEffectManager
//

SINGLETON_CLASS_IMPLEMENTATION_CODE(EfficientEffectManager)

effects::Effect* EfficientEffectManager::get(string typ){
    // convert string (typ) to EffectType (type)
    effects::EffectType type = typeStringToType(typ);
    // find already allocated effect in idle manager
    effects::Effect* pEffect = idle_manager.findByType(type);

    if(!pEffect){
        // no idle effect found; create new one like usual
        return EffectManager::get(typ);
    }

    // found one, remove it from idle manager
    idle_manager.remove(pEffect);
    // add to our own list
    add(pEffect);

    // update name if necessary
    if(pEffect->type == effects::EffectType::DEFAULT){
        pEffect->name = typ;
    }

    // done
    return pEffect;
}

void EfficientEffectManager::finish(effects::Effect* effect){
    // remove from our "active list"
    if(remove(effect)){
        // if remove suceeded (meaning the specified effect was found)
        // add it to our idle_manager, so we can recycle this instance later
        idle_manager.add(effect);
    }
}


