//
//  effect_creator.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef effect_creator_hpp
#define effect_creator_hpp

//#include <stdio.h>
#include "shared2030.h"
#include "interface.hpp"
#include "player.hpp"
#include "effects.hpp"
#include "setting_types.h"
#include "effect_manager.hpp"

namespace of2030{
    
    class InterfacePlayerBridge{
        SINGLETON_INLINE_HEADER_CODE(InterfacePlayerBridge)
    
    public: // methods
        
        InterfacePlayerBridge();
        ~InterfacePlayerBridge();

        void setup();
        void destroy();

    private: // callbacks

        void registerCallbacks(bool _register=true);
        void onTrigger(string &trigger);
        void onStopTrigger(string &trigger);
        void onEffectConfig(EffectConfig &cfg);
        void onScreenConfig(EffectConfig &cfg);
        void onEffectEnded(Effect &effect);
        void onPlayback(string &name);

    private: // attributes

        Interface* m_interface;
        Player* m_player;
        vector<Effect*> *active_effects;
        bool m_bStarted;
    };
}

#endif /* effect_creator_hpp */
