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
#include "interface.hpp"
#include "player.hpp"
#include "effects.hpp"
#include "setting_types.h"

namespace of2030{
    
    class InterfacePlayerBridge{
    
    public: // methods
        
        InterfacePlayerBridge();
        ~InterfacePlayerBridge();

        void start();
        void stop();

        void setInterface(Interface *interface);

    private: // callbacks

        void registerInterfaceCallbacks(bool _register=true);
        void onTrigger(string &trigger);
        void onEffect(string &effect);
        void onShaderEffect(string &shader);
        void onEffectConfig(EffectConfig &cfg);
        void onSong(string &name);
        void onClip(string &name);

    private: // helpers methods
        effects::Effect* createEffect(string &name);

    private: // attributes

        Interface* m_interface;
        Player* m_player;
        bool m_bStarted;
    };
}

#endif /* effect_creator_hpp */
