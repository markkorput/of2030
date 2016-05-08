//
//  interface.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef interface_hpp
#define interface_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "setting_types.h"

namespace of2030 {
    
    class Interface {

    public:
        static Interface* instance();
    
    public: // events
        ofEvent<string> triggerEvent;
        ofEvent<string> effectEvent;
        ofEvent<string> shaderEffectEvent;
        ofEvent<EffectConfig> effectConfigEvent;
        ofEvent<string> reconfigSettingsEvent;
        ofEvent<string> reconfigClientsEvent;
        ofEvent<string> reconfigEffectsEvent;
        ofEvent<string> songEvent;
        ofEvent<string> clipEvent;

    private:
        static Interface* singleton;

    };
};

#endif /* interface_hpp */
