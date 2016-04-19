//
//  effect.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef effect_hpp
#define effect_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "CMSModel.h"

namespace of2030{ namespace effects {

    enum EffectType{
        OFF = 0,
        COLOR = 1,
    };

    #define NO_TIME (-1.0f)



    class Effect{

    public: // methods

        Effect() : startTime(NO_TIME), endTime(NO_TIME), type(OFF) {}
        // ~Effect(){}

        bool hasStartTime(){ return startTime >= 0.0f; }
        bool hasEndTime(){ return endTime >= 0.0f; }

    public: // properties

        float startTime, endTime;
        EffectType type;
    };



    class Off : public Effect{

    public: // methods

        Off(){ type = EffectType::OFF; }
    };



    class Color : public Effect{

    public: // methods
        Color() : color(ofColor::black){ type = EffectType::COLOR; }

    public: // attributes
        ofColor color;
    };

}} // namespace of2030{ namespace effects {

#endif /* effect_hpp */
