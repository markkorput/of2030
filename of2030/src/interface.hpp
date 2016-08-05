//
//  interface.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef interface_hpp
#define interface_hpp

#include "ofMain.h"
#include "shared2030.h"
#include "setting_types.h"

namespace of2030 {
    
    class Interface {
        SINGLETON_INLINE_HEADER_CODE(Interface)

    public: // VJ-events
        ofEvent<string> triggerEvent;
        ofEvent<string> stopTriggerEvent;

        ofEvent<EffectConfig> effectConfigEvent;
        ofEvent<EffectConfig> screenConfigEvent;
        
        ofEvent<string> controlEvent;

        ofEvent<string> playbackEvent;
        ofEvent<string> stopPlaybackEvent;

        ofEvent<string> loadVideoEvent;
        ofEvent<string> unloadVideoEvent;

        ofEvent<string> loadImageEvent;
        ofEvent<string> unloadImageEvent;
    };
};

#endif /* interface_hpp */
