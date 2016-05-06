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
#include "effects.hpp"

namespace of2030 {
    
    class Interface {

    public:
        static Interface* instance();
    
    public: // events
        ofEvent<effects::Effect> effectEvent;
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
