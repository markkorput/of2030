//
//  composition.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef composition_hpp
#define composition_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "effects.hpp"

namespace of2030{
  
    class Composition{
      
    public: // methods

        // Composition(){}
        // ~Composition(){}

        void add(effects::Effect *effect);
        const vector<effects::Effect*> &getEffects(){ return m_effects; }

    public: // events
        ofEvent<effects::Effect> newEffectEvent;

    private: // attributes

        vector<effects::Effect*> m_effects;
    };

};

#endif /* composition_hpp */
