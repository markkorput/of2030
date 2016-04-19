//
//  effect.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef effect_hpp
#define effect_hpp

#include <stdio.h>
#include "CMSModel.h"

namespace of2030{

    enum EffectType{
        OFF = 0,
        COLOR = 1,
    };

    #define NO_TIME (-1.0f)

    class Effect : public CMS::Model{

    public:
        Effect() : m_time(NO_TIME), m_type(OFF) {}
        // Effect(float time, EffectType type) : m_time(time), m_type(type) {}
        // ~Effect(){}
        
        bool hasTime(){ return m_time >= 0.0f; }

    public:
        float m_time;
        EffectType m_type;
    };
}

#endif /* effect_hpp */
