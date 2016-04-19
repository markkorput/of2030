//
//  interface.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "interface.hpp"

using namespace of2030;

Interface* Interface::singleton = NULL;

Interface* Interface::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new Interface();
    }
    return singleton;
}