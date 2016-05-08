//
//  xml_screens.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-08.
//
//

#include "xml_screens.hpp"

using namespace of2030;

const string XmlScreens::path = "screens";
const string XmlScreens::rootNodeName = "screens";
const string XmlScreens::itemNodeName = "screen";

XmlScreens* XmlScreens::singleton = NULL;

XmlScreens* XmlScreens::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new XmlScreens();
    }
    return singleton;
}