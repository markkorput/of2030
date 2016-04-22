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


namespace of2030{
    
    class InterfacePlayerBridge{
    
    public: // methods
        
        InterfacePlayerBridge();
        // ~InterfacePlayerBridge();

        void setInterface(Interface *interface);

    private: // callbacks

        void onEffect(effects::Effect &effect);

    private: // attributes

        Interface* m_interface;
        Player* m_player;
    };
}

#endif /* effect_creator_hpp */
