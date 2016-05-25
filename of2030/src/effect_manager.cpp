//
//  effect_manager.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-15.
//
//

#include "effect_manager.hpp"

using namespace of2030;

EffectManager::~EffectManager(){
    for(auto effect: effects){
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
    ofNotifyEvent(effectAddedEvent, *effect, this);
}

effects::EffectType EffectManager::typeStringToType(string typ){
    if(typ == "vid") return effects::EffectType::VID;
    if(typ == "spot") return effects::EffectType::SPOT;
    if(typ == "pos") return effects::EffectType::POS;
    return effects::EffectType::DEFAULT;
}

effects::Effect* EffectManager::createEffect(string typ){
    ofLogVerbose() << "EffectManager::createEffect with: " << typ;

    effects::Effect* pEffect;

    if(typ == "vid"){
        pEffect = (effects::Effect*) new effects::Vid();
    } else if(typ == "spot"){
        pEffect = (effects::Effect*) new effects::Spot();
    } else if(typ == "pos"){
        pEffect = (effects::Effect*) new effects::Pos();
    }else {
        // default type, just set name to whatever was specified
        pEffect = new effects::Effect();
        pEffect->name = typ;
    }

    return pEffect;
}

#define IF_TYP_DEL(__x__,__y__) \
    if(effect->type == effects::EffectType::__x__){\
        delete (effects::__y__*) effect;\
        return;\
    }\

void EffectManager::deleteEffect(effects::Effect* effect){
    ofLogVerbose() << "EffectManager::deleteEffect";
    
    IF_TYP_DEL(VID, Vid)
    IF_TYP_DEL(SPOT, Spot)
    IF_TYP_DEL(POS, Pos)
//    // figure out effect type and delete from memory
//    if(effect->type == effects::EffectType::VID){
//        // turn into Vid effect pointer before deleting, to delete appropriate class type
//        delete (effects::Vid*) effect;
//        return;
//    }
//    
//    if(effect->type == effects::EffectType::SPOT){
//        delete (effects::Spot*) effect;
//        return;
//    }
//    
//    if(effect->type == effects::EffectType::VOICE){
//        delete (effects::Voice*) effect;
//        return;
//    }
    
    // default; simply effects::Effect
    delete effect;
}

bool EffectManager::remove(effects::Effect* effect){
    // find effect in our list
    for(int i=effects.size()-1; i>=0; i--){
        // this one?
        if(effects[i] == effect){
            // remove from list
            effects.erase(effects.begin()+i);
            // notify
            ofNotifyEvent(effectRemovedEvent, *effect, this);
            // done
            return true;
        }
    }

    // not found
    ofLogWarning() << "[EffectManager::remove] could not find specified effect in list, ignoring";
    return false;
}

void EffectManager::clear(){
    for(auto effect: effects){
        remove(effect);
    }
}

int EffectManager::getCountByType(effects::EffectType typ){
    int count = 0;
    for(auto effect: effects)
        if(effect->type == typ)
            count++;
    return count;
}




//
// EfficientEffectManager
//

SINGLETON_CLASS_IMPLEMENTATION_CODE(EfficientEffectManager)

const int idle_cache_limit_per_type = 3;

effects::Effect* EfficientEffectManager::get(string typ){
    // convert string (typ) to EffectType (type)
    effects::EffectType type = typeStringToType(typ);
    // find already allocated effect in idle manager
    effects::Effect* pEffect = idle_manager.findByType(type);

    // no existing idle instance found, default to "normal behaviour" (allocate new instance)
    if(!pEffect){
        // no idle effect found; create new one like usual
        return EffectManager::get(typ);
    }
    
    // found one, remove it from idle manager
    idle_manager.remove(pEffect);
    // reset its time values (and some other attributes)
    pEffect->reset();
    // update name if necessary
    if(pEffect->type == effects::EffectType::DEFAULT){
        pEffect->name = typ;
    }

    // add to our own list
    add(pEffect);

    // done
    return pEffect;
}

void EfficientEffectManager::finish(effects::Effect* effect){
    // ofLogVerbose() << "EfficientEffectManager::finish";

    // remove from our "active list"
    if(!remove(effect)){
        // remove failed, meaning this effect could not be found in our list, abort
        return;
    }

    // check if cache of idle instance has reached its limits yet
    if(idle_manager.getCountByType(effect->type) >= idle_cache_limit_per_type){
        // idle cache full, just delete this instance
        deleteEffect(effect);
        return;
    }

    // add it to our idle_manager, so we can recycle this instance later
    // ofLogVerbose() << "idle_manager count before: " << idle_manager.getCount();
    idle_manager.add(effect);
    ofLogVerbose() << "[EfficientEffectManager] idle effects count: " << idle_manager.getCount();
}

