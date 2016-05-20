#include "xml_triggers.hpp"
#include "ofxXmlSettings.h"

#include <regex>

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
    if(!xml.pushTag("of2030") || !xml.pushTag("triggers"))
        // nope, abort
        return;

    string triggerName, effectName;

    for(int i=xml.getNumTags("trigger")-1; i >= 0; i--){
        triggerName = xml.getAttribute("trigger", "name", "", i);
        effectName = xml.getValue("trigger", "", i);
        trigger_effects[triggerName] = effectName;
    }
}

string XmlTriggers::getEffectName(const string &triggerName){
    // find and configured effect for this trigger
    std::map<string,string>::iterator it = trigger_effects.find(triggerName);
    // found it! return the configured effect to be triggered
    if(it != trigger_effects.end())
        return it->second;

    // matches on any digits at then end of the string
    std::regex expression("(\\d+)$");
    // return the trigger name itself without any trailing digits
    return std::regex_replace (triggerName, expression,"");
}
