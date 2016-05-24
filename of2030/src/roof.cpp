//
//  roof.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-24.
//
//

#include "roof.hpp"

using namespace of2030::effects;

Roof::Roof(){
    coordsName = "roof_coord";
}

void Roof::setup(Context &context){
    MapEffect::setup(context);
}

void Roof::draw(Context &context){
    MapEffect::draw(context);
}
