//
//  effect.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "effects.hpp"

using namespace of2030::effects;

int Effect::cidCounter = 0;

Effect::Effect() : startTime(NO_TIME), endTime(NO_TIME), type(EffectType::OFF) {
    // every effect instance gets a unique cid (client-side-id)
    cid = cidCounter;
    cidCounter++;
}

float Effect::getDuration(){
    if(hasDuration())
        return duration;

    if(hasStartTime() and hasEndTime())
        return endTime - startTime;

    return -1.0;
}

Color::Color(){
    type = EffectType::COLOR;
    color = ofColor::black;
}


Cursor::Cursor(){
    type = EffectType::CURSOR;
    duration = 3.0;
}