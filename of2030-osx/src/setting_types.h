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
        string effectAddress; // /effect
        string controlAddress; // /control
    } OscSetting;

    //
    // Effect
    //
    
    class EffectSetting{
    public: // attributes
        string name;
        string part;
        map<string, string> data;

    public: // methods
        void merge(EffectSetting &other){
            map<string, string>::iterator it = data.begin();
            while(it != data.end()){
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
                ofToInt(it->second);
            return defaultValue;
        }
        
        float getValue(string name, float defaultValue){
            std::map<string,string>::iterator it = data.find(name);
            if(it != data.end())
                ofToFloat(it->second);
            return defaultValue;
        }
    };
}

#endif /* client_settings_h */
