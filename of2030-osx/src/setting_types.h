//
//  client_setting.h
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-06.
//
//

#ifndef client_setting_h
#define client_setting_h

namespace of2030{
    
    //
    // Client
    //

    typedef struct{
        int id;
        ofVec3f screenpos, screenrot;
        ofVec2f screensize;
        
        float pano_start, pano_end;
        ofVec3f pano_rot;
    } ClientSetting;

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
    
    class EffectSetting{
    public: // attributes
        string name;
        map<string, string> data;

    public: // methods
        void merge(EffectSetting &other){
            for(map<string, string>::iterator it = other.data.begin(); it != other.data.end(); ++it){
                this->data[it->first] = it->second;
            }
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
    };
}

#endif /* client_settings_h */
