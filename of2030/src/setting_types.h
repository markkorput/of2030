//
//  client_setting.h
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-06.
//
//

#ifndef client_setting_h
#define client_setting_h

#include "ofMain.h"

namespace of2030{

    //
    // Osc
    //
    typedef struct {
        int port;
        map<string, string> addresses;
    } OscSetting;

    //
    // EffectConfig
    //
    typedef struct {
        string setting_name;
        string param_name;
        string param_value;
    } EffectConfig;

    //
    // Effect
    //
    
    class XmlItemSetting{
    public: // attributes
        string name;
        map<string, string> data;

    public: // methods
        void merge(XmlItemSetting &other){
            for(map<string, string>::iterator it = other.data.begin(); it != other.data.end(); ++it){
                this->data[it->first] = it->second;
            }
        }

        bool hasValue(string name){
            std::map<string,string>::iterator it = data.find(name);
            if(it == data.end())
                return false;
            return true;
        }

        string getValue(string name, string defaultValue=""){
            std::map<string,string>::iterator it = data.find(name);
            if(it == data.end())
                return defaultValue;
            return it->second;
        }

        int getValue(string name, int defaultValue){
            std::map<string,string>::iterator it = data.find(name);
            if(it != data.end())
                return defaultValue;
            return ofToInt(it->second);
        }
        
        float getValue(string name, float defaultValue){
            std::map<string,string>::iterator it = data.find(name);
            if(it == data.end())
                return defaultValue;
            return ofToFloat(it->second);
        }

        ofVec2f getValue(string name, const ofVec2f &defaultValue){
            return ofVec2f(getValue(name+"_x", defaultValue.x),
                           getValue(name+"_y", defaultValue.x));
        }

        ofVec3f getValue(string name, const ofVec3f &defaultValue){
            return ofVec3f(getValue(name+"_x", defaultValue.x),
                           getValue(name+"_y", defaultValue.y),
                           getValue(name+"_z", defaultValue.z));
        }
    };
}

#endif /* client_settings_h */
