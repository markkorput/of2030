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
        void add(effects::Effect* effect);
        effects::Effect* findByType(effects::EffectType typ);
        bool remove(effects::Effect* effect);

    protected:
        effects::EffectType typeStringToType(string typ);
        effects::Effect* createEffect(string typ);
        void deleteEffect(effects::Effect* effect);

        vector<effects::Effect*> effects;
    };

    class EfficientEffectManager : public EffectManager{
        SINGLETON_CLASS_HEADER_CODE(EfficientEffectManager)
    public:
        effects::Effect* get(string typ);
        void finish(effects::Effect* effect);
    protected:
        EffectManager idle_manager;
    };
}

#endif /* effect_manager_hpp */
