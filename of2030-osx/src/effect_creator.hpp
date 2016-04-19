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

namespace of2030{
    
    class EffectCreator{
    
    public: // methods
        
        EffectCreator();
        // ~EffectCreator();

        void setInterface(Interface *interface);

    private: // callbacks

        void onNewEffectModel(CMS::Model &model);

    private: // attributes

        Interface* m_interface;
        Player* m_player;
    };
}

#endif /* effect_creator_hpp */
