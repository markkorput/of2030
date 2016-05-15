//
//  effect_manager.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-15.
//
//

#ifndef effect_manager_hpp
#define effect_manager_hpp

#include "shared2030.h"
#include "effects.hpp"

namespace of2030{
    class EffectManager{
        SINGLETON_CLASS_HEADER_CODE(EffectManager)

    public:
//        EffectManager(){}
        ~EffectManager();
        
        effects::Effect* get(string typ);
        void finish(effects::Effect* effect);

    private:
        effects::Effect* create(string typ);
        void remove(effects::Effect* effect);

        vector<effects::Effect*> active_effects; //, idle_effects;
    };
}

#endif /* effect_manager_hpp */
