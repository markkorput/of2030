#include "xml_triggers.hpp"
#include "ofxXmlSettings.h"

using namespace of2030;

// XmlTriggers implementation

XmlTriggers* XmlTriggers::singleton = NULL;

XmlTriggers* XmlTriggers::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new XmlTriggers();
    }
    return singleton;
}

void XmlTriggers::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);

    trigger_effects.clear();

    // any trigger data in xml?
    if(!xml.pushTag("of2030:triggers"))
        // nope, abort
        return;

    string triggerName, effectName;
    
    for(int i=xml.getNumTags("trigger")-1; i >= 0; i--){
        triggerName = xml.getAttribute("trigger", "name", "", i);
        effectName = xml.getValue("trigger", "", i);
        trigger_effects[triggerName] = effectName;
    }
}
