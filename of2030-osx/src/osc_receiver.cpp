//
//  osc_receiver.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "osc_receiver.hpp"
#include "ofxJSONElement.h"

using namespace of2030;

OscReceiver::OscReceiver() : m_interface(NULL), m_bConnected(false){
    default_setting.port = 2030;
    osc_setting = &default_setting;
}

void OscReceiver::configure(unsigned int port, Interface* interface){
    osc_setting->port = port;

    if(interface)
        m_interface = interface;

    if(m_bConnected){
        // reconnect
        connect();
    }
}

void OscReceiver::configure(OscSetting &_osc_setting){
    this->osc_setting = &_osc_setting;

    if(m_bConnected){
        // reconnect
        connect();
    }
}

void OscReceiver::setup(){
    if(m_interface == NULL){
        m_interface = Interface::instance();
    }

    if(!m_bConnected){
        connect();
    }
}

void OscReceiver::update(){
    int message_count = 0;

    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages() && message_count < MAX_MESSAGES_PER_CYCLE){
        // get the next message
        ofxOscMessage m;
        m_oscReceiver.getNextMessage(m);
        message_count++;

        string addr = m.getAddress();
        if(addr == osc_setting->addresses["shader"]){
            string shader_name = m.getArgAsString(0);
            if(shader_name == ""){
                ofLogWarning() << "[osc-in] got " << osc_setting->addresses["shader"] << " message without shader name param";
                continue;
            }

            effects::ShaderEffect* fx = new effects::ShaderEffect();
            fx->setShader(shader_name);
            ofNotifyEvent(m_interface->effectEvent, (*(effects::Effect*)fx), m_interface);
            continue;
        }

        if(addr == osc_setting->addresses["effect"]){
            //ofLogVerbose() << "Got /message OSC Message";
            if(processFxMessage(m))
                continue;
        }

        if(addr == osc_setting->addresses["control"]){
            //ofLogVerbose() << "Got /ctrl OSC Message";
            if(processCtrlMessage(m))
                continue;
        }

        if(addr == osc_setting->addresses["song"]){
            string name = m.getArgAsString(0);
            ofLogVerbose() << "[osc-in] song: " << name;
            ofNotifyEvent(m_interface->songEvent, name, m_interface);
            continue;
        }

        if(addr == osc_setting->addresses["clip"]){
            string name = m.getArgAsString(0);
            ofLogVerbose() << "[osc-in] clip: " << name;
            ofNotifyEvent(m_interface->clipEvent, name, m_interface);
            continue;
        }

        if(addr == "/effect"){
            //ofLogVerbose() << "Got /effect OSC Message";
            if(processJsonEffectMessage(m))
                continue;
        }

        ofLog() << "Unable to process OSC Message " << m.getAddress();
    }
}

void OscReceiver::destroy(){
    if(m_bConnected){
        disconnect();
    }
}

bool OscReceiver::connect(){
    // m_oscReceiver.enableReuse();

#ifdef __BOOL_OSC_SETUP__
    if(m_oscReceiver.setup(osc_setting->port)){
        m_bConnected = true;
        ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
        return true;
    }

    ofLogWarning() << "OscReceiver could not start listening to port: " << osc_setting->port;
    return false;
#else
    m_oscReceiver.setup(osc_setting->port);
    m_bConnected = true;
    ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
    return true;
#endif // __BOOL_OSC_SETUP__

}

void OscReceiver::disconnect(){
    m_bConnected = false;
}

bool OscReceiver::processJsonEffectMessage(ofxOscMessage &m){
    if(m.getNumArgs() < 1){
        ofLogWarning() << "/effect message didn't have any args";
        return false;
    }

    if(m.getArgType(0) != OFXOSC_TYPE_STRING){
        ofLogWarning() << "/effect didn't have string arg";
        return false;
    }

    effects::Effect* effect = createEffectFromJsonString(m.getArgAsString(0));
    if(!effect){
        ofLogWarning() << "Could not create effect instance from OSC /effect message with data: " << m.getArgAsString(0);
        return false;
    }

    // ofLog() << "[OscReceiver] Triggering interface's effectEvent";
    ofNotifyEvent(m_interface->effectEvent, *effect, m_interface);
    return true;
}

void OscReceiver::getMapFromJsonString(const std::string &str, map<string, string> &target){
    ofxJSONElement jsonEl;
    jsonEl.parse(str);

    // get all attributes as string (no support for nester structure for now)
    vector<string> attrs = jsonEl.getMemberNames();

    for(int i=attrs.size()-1; i>=0; i--){
        string name = attrs[i];
        target[name] = jsonEl[name].asString();
    }
}

effects::Effect* OscReceiver::createEffectFromJsonString(const std::string &json_string){
    effects::Effect* pEffect;
    std::string type, value;

    ofxJSONElement json;
    if(!json.parse(json_string))
        return NULL;

    if(json.isMember("type"))
        type = json["type"].asString();

    // create an instance of the appropriate effects class
    // beased on the model's type attribute
    // and populate type-specific attributes
    // the assign the pEffect pointer to the created instance
    if(type == "OFF"){

        effects::Off* effect = new effects::Off();
        pEffect = (effects::Effect*)effect;

    } else if(type == "COLOR"){

        effects::Color* effect = new effects::Color();
        if(json.isMember("r") && json.isMember("g") && json.isMember("b")){
            effect->color.set(ofColor(json["r"].asInt(),
                                      json["g"].asInt(),
                                      json["b"].asInt()));
        }
        pEffect = (effects::Effect*)effect;

    } else {

        ofLogWarning() << "[EffectCreator] got unknown effect model type: " << type;
        pEffect = new effects::Effect();
    }

    // process some more (optional) general effect attributes
    if(json.isMember("start"))
        pEffect->startTime = json["start"].asFloat();

    if(json.isMember("end"))
        pEffect->endTime = json["end"].asFloat();

    if(json.isMember("duration"))
        pEffect->duration = json["duration"].asFloat();

    return pEffect;
}

bool OscReceiver::processFxMessage(ofxOscMessage &m){
    if(m.getNumArgs() < 1){
        ofLogWarning() << "/fx message didn't have any args";
        return true;
    }

    if(m.getArgType(0) != OFXOSC_TYPE_STRING){
        ofLogWarning() << "/fx didn't have string arg";
        return true;
    }

    string messageType = m.getArgAsString(0);
    ofLogVerbose() << "[osc-in] /fx " << messageType;

    if(messageType == "cursor"){
        effects::Cursor* cursor_effect = new effects::Cursor();
        ofNotifyEvent(m_interface->effectEvent, (*(effects::Effect*)cursor_effect), m_interface);
        return true;
    }

    if(messageType == "vid"){
        effects::Vid* effect = new effects::Vid();
        ofNotifyEvent(m_interface->effectEvent, (*(effects::Effect*)effect), m_interface);
        return true;
    }

    ofLogWarning() << "[osc-in] unknown messageType: " << messageType;
    return false;
}

bool OscReceiver::processCtrlMessage(ofxOscMessage &m){
    if(m.getNumArgs() < 1){
        ofLogWarning() << "/ctrl message didn't have any args";
        return false;
    }

    if(m.getArgType(0) != OFXOSC_TYPE_STRING){
        ofLogWarning() << "/ctrl didn't have string arg";
        return false;
    }

    string messageType = m.getArgAsString(0);
    ofLogVerbose() << "[osc-in] /ctrl " << messageType;

    if(messageType == "reconfig_clients"){
        string config_path = "";
        if(m.getNumArgs() >= 2 and m.getArgType(1) == OFXOSC_TYPE_STRING)
        config_path = m.getArgAsString(1);
        ofLogVerbose() << "reconfig_clients";
        ofNotifyEvent(m_interface->reconfigClientsEvent, config_path, m_interface);
        return true;
    }

    if(messageType == "reconfig_settings"){
        string path="";
        ofLog() << "reconfig_settings";
        ofNotifyEvent(m_interface->reconfigSettingsEvent, path, m_interface);
        return true;
    }

    if(messageType == "reconfig_effects"){
        string config_path = "";
        if(m.getNumArgs() >= 2 and m.getArgType(1) == OFXOSC_TYPE_STRING)
        config_path = m.getArgAsString(1);
        ofLogVerbose() << "reconfig_effects";
        ofNotifyEvent(m_interface->reconfigEffectsEvent, config_path, m_interface);
        return true;
    }

    ofLogWarning() << "[osc-in] unknown messageType: " << messageType;
    return false;
}
