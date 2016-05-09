//
//  renderer.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef renderer_hpp
#define renderer_hpp

#include "shared2030.h"
#include "player.hpp"
#include "effects.hpp"

namespace of2030 {
    
    class Renderer{
        SINGLETON_CLASS_HEADER_CODE(Renderer)

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
        void fillEffectSetting(effects::Effect &effect, XmlItemSetting &fxsetting);
        void fillScreenSetting(effects::Effect &effect, XmlItemSetting &setting);

    public: // properties
        
        Player *player;
        string client_id;
        ofFbo* fbo;
    };
}

#endif /* renderer_hpp */
