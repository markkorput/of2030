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
        CURSOR = 2
    };

    #define NO_TIME (-1.0f)



    class Effect{

    public: // methods

        Effect();
        // ~Effect(){}

        bool hasStartTime(){ return startTime >= 0.0f; }
        bool hasEndTime(){ return endTime >= 0.0f; }
        bool hasDuration(){ return duration >= 0.0f; }
        float getDuration();

    public: // properties

        int cid;
        float startTime, endTime, duration;
        EffectType type;

        static int cidCounter;
    };



    class Off : public Effect{

    public: // methods

        Off(){ type = EffectType::OFF; }
    };



    class Color : public Effect{

    public: // methods
        Color();

    public: // attributes
        ofColor color;
    };


    class Cursor : public Effect{
   
    public: // methods
        Cursor();
        
    };

}} // namespace of2030{ namespace effects {

#endif /* effect_hpp */
