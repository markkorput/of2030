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
#include "client_info.hpp"
#include "setting_types.h"

namespace of2030 {
    
    class Renderer{
        
    public:
        const static int WIDTH = 768;
        const static int HEIGHT = 576;
        
    public: // methods
        Renderer();
        ~Renderer();

        void setup();
        // void update();
        void destroy();
        void draw();
            
    private: // callbacks
        
        void registerRealtimeEffectCallback(bool reg=true);
        void onRealtimeEffect(effects::Effect &effect);
        void fillContext(effects::Context &context, effects::Effect &effect);
        void fillContextClientInfo(effects::Context &context);
        void fillEffectSetting(effects::Effect &effect, EffectSetting &fxsetting);

    public: // properties
        
        Player *player;
        ClientInfo *client_info;
        ofFbo* fbo;
    };
}

#endif /* renderer_hpp */
