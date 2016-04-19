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

namespace of2030{ namespace effects {

    enum EffectType{
        OFF = 0,
        COLOR = 1,
    };

    #define NO_TIME (-1.0f)

    class Effect{

    public:
        Effect() : m_startTime(NO_TIME), m_type(OFF) {}
        // ~Effect(){}

        bool hasTime(){ return m_startTime >= 0.0f; }

    public:
        float m_startTime, m_endTime;
        EffectType m_type;
    };

}} // namespace of2030{ namespace effects {

#endif /* effect_hpp */
