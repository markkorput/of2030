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
#include "xml_configs.hpp"

namespace of2030 {
    
    class Renderer{
        SINGLETON_INLINE_HEADER_CODE(Renderer)

    public: // methods
        Renderer();
        ~Renderer();

        void setup();
        // void update();
        void destroy();
        void draw();
        
        inline void setClientId(const string &id){ client_id = id; }
        inline const string &clientId(){ return client_id; }

        inline ofVec2f getScreenSize(){ return ofVec2f(screenWidth, screenHeight); }

    private: // callbacks
        
        void registerRealtimeEffectCallback(bool reg=true);
        void onEffectAdded(Effect &effect);

        void fillContextClientInfo(Context &context);
        void fillEffectSetting(Effect &effect, XmlItemSetting &fxsetting);
        void fillScreenSetting(XmlItemSetting &setting);

    private: // properties
        
        Player *player;
        string client_id;
        ofFbo *fbo, *fbo2, *fbo3;
        ofFbo defaultFbo, defaultFbo2, defaultFbo3;
        bool bCallbacksRegistered;
        float screenWidth, screenHeight;
        //Effect *overlayEffect;
        
        float lastFrameTime;
        XmlConfigs* effect_configs_instance;
        Context cached_context;
        bool bScreenConfigCached;
    };
}

#endif /* renderer_hpp */
