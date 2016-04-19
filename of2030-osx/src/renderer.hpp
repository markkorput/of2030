//
//  renderer.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef renderer_hpp
#define renderer_hpp

//#include <stdio.h>
#include "player.hpp"
#include "effects.hpp"

namespace of2030 {
    
    class Renderer{
        
    public: // methods
        Renderer();
        // ~Renderer(){}

        // void setup();
        // void update();
        void draw();
        void drawEffect(effects::Off &effect);
        void drawEffect(effects::Color &effect);
        // void destroy();
        

    public: // properties
        
        Player *player;

    private: // attributes

        
        
    };
}

#endif /* renderer_hpp */
