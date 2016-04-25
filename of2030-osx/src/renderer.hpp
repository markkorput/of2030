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
    
    protected:
        
        void drawEffect(effects::Off &effect);
        void drawEffect(effects::Color &effect);
        void setupEffect(effects::Cursor &effect);
        void drawEffect(effects::Cursor &effect);
        
    private: // callbacks
        
        void registerRealtimeEffectCallback(bool reg=true);
        void onRealtimeEffect(effects::Effect &effect);
        

    public: // properties
        
        Player *player;
        ClientInfo *m_client_info;
        ofFbo* m_fbo;
    };
}

#endif /* renderer_hpp */
