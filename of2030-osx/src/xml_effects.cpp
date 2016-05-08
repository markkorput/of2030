#include "xml_effects.hpp"
#include "ofxXmlSettings.h"

using namespace of2030;

// local methods

void xmlLoadEffect(TiXmlElement &xml_el, XmlItemSetting &fx){

    const char *pstr = xml_el.Attribute("name");
    if(pstr)
        fx.name = pstr;
    
    fx.data.clear();
    for(TiXmlElement* child = xml_el.FirstChildElement(); child != NULL; child = child->NextSiblingElement()){
        fx.data[child->ValueStr()] = child->ToElement()->GetText();
        ofLogVerbose() << "[XmlEffect] got value: " << child->ValueStr() << "/" << child->ToElement()->GetText();
    }
}


// XmlEffects implementation


XmlEffects* XmlEffects::_instance = NULL;
XmlEffects* XmlEffects::_screens_instance = NULL;

XmlEffects* XmlEffects::instance(){
    if (!_instance){
        _instance = new XmlEffects();
    }
    return _instance;
}

XmlEffects* XmlEffects::screens(){
    if (!_screens_instance){
        _screens_instance = new XmlEffects();
        _screens_instance->path = "screens.xml";
        _screens_instance->rootNodeName = "screens";
        _screens_instance->itemNodeName = "screen";
    }
    return _screens_instance;
}


XmlEffects::XmlEffects() : path("effects.xml"), rootNodeName("effects"), itemNodeName("effect"){
}

void XmlEffects::destroy(){
    for(auto &fx_setting: settings)
        if(fx_setting)
            delete fx_setting;

    settings.clear();
}


void XmlEffects::load(){
    ofxXmlSettings xml;
    xml.loadFile(path);

    TiXmlDocument *doc = &xml.doc;
    TiXmlElement *el = doc->FirstChildElement("of2030");
    if(el){
        el = el->FirstChildElement(rootNodeName);
        if(el){

            XmlItemSetting *fx;
            int loaded_count = settings.size();
            int xml_count = 0;

            el = el->FirstChildElement(itemNodeName);
            while(el){

                // allocate new instance or use previsouly allocated?
                if(xml_count >= loaded_count){
                    // new instance
                    fx = new XmlItemSetting();
                    // add to list
                    settings.push_back(fx);
                    // increase our loaded count
                    loaded_count++;
                } else {
                    // grab existing
                    fx = settings[xml_count];
                }
                
                // populate our client instance
                xmlLoadEffect(*el, *fx);

                xml_count++;
                el = el->NextSiblingElement(itemNodeName);
            }

            // remove any too-many instances
            while(loaded_count > xml_count){
                fx = settings.back();
                delete fx;
                settings.pop_back();
                loaded_count--;
            }
        }
    }
}

XmlItemSetting* XmlEffects::getItem(string name){
    for(int i=settings.size()-1; i>=0; i--){
        XmlItemSetting* setting = settings[i];

        if(setting->name == name){
            return setting;
        }
    }

    return NULL;
}

void XmlEffects::setItemParam(string settingName, string paramName, string value){
    // find existing setting
    XmlItemSetting *pSetting = getItem(settingName);

    if(!pSetting){
        // create setting
        pSetting = new XmlItemSetting();
        pSetting->name = settingName;
        // add to our list
        settings.push_back(pSetting);
    }

    pSetting->data[paramName] = value;
}



