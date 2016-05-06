#include "xml_effects.hpp"
#include "ofxXmlSettings.h"

using namespace of2030;

// local methods

void xmlLoadEffect(TiXmlElement &xml_el, EffectSetting &fx){

    const char *pstr = xml_el.Attribute("name");
    if(pstr)
        fx.name = pstr;
    pstr = xml_el.Attribute("part");
    if(pstr)
        fx.part = pstr;
    
    fx.data.clear();
    for(TiXmlElement* child = xml_el.FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
        fx.data[child->ValueStr()] = child->ToElement()->GetText();
        ofLogVerbose() << "[XmlEffect] got value: " << child->ValueStr() << "/" << child->ToElement()->GetText();
    }
}


// XmlEffects implementation

XmlEffects* XmlEffects::singleton = NULL;

XmlEffects* XmlEffects::instance(){
    if (!singleton){   // Only allow one instance of class to be generated.
        singleton = new XmlEffects();
    }
    return singleton;
}


void XmlEffects::destroy(){
    for(auto &fx_setting: effect_settings)
        if(fx_setting)
            delete fx_setting;

    effect_settings.clear();
}


void XmlEffects::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);

    TiXmlDocument *doc = &xml.doc;
    TiXmlElement *el = doc->FirstChildElement("of2030");
    if(el){
        el = el->FirstChildElement("effects");
        if(el){

            EffectSetting *fx;
            int loaded_count = effect_settings.size();
            int xml_count = 0;
            
            el = el->FirstChildElement("effect");
            while(el){

                // allocate new instance or use previsouly allocated?
                if(xml_count >= loaded_count){
                    // new instance
                    fx = new EffectSetting();
                    // add to list
                    effect_settings.push_back(fx);
                    // increase our loaded count
                    loaded_count++;
                } else {
                    // grab existing
                    fx = effect_settings[xml_count];
                }
                
                // populate our client instance
                xmlLoadEffect(*el, *fx);

                xml_count++;
                el = el->NextSiblingElement("effect");
            }

            // remove any too-many instances
            while(loaded_count > xml_count){
                fx = effect_settings.back();
                delete fx;
                effect_settings.pop_back();
                loaded_count--;
            }
        }
    }
}

EffectSetting* XmlEffects::getEffectSetting(string name, string part){
    for(int i=effect_settings.size()-1; i>=0; i--){
        EffectSetting* setting = effect_settings[i];

        if(setting->name == name && setting->part == part){
            return setting;
        }
    }

    return NULL;
}