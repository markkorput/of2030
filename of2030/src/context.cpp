//
//  context.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-28.
//
//

#include "context.hpp"

using namespace of2030;

void Context::precalc(){
    //    resolution = screen_setting.getValue("resolution", ofVec2f(768, 576));
    resolution.set(fbo2->getWidth(), fbo2->getHeight());
    
}