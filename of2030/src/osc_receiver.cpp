//
//  osc_receiver.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "ofMain.h"
#include "osc_receiver.hpp"
#include "setting_types.h"

using namespace of2030;

SINGLETON_CLASS_IMPLEMENTATION_CODE(OscReceiver)

OscReceiver::OscReceiver() : m_interface(NULL), bConnected(false){
    default_setting.port = 2030;
    osc_setting = &default_setting;
}

void OscReceiver::configure(OscSetting &_osc_setting){
    this->osc_setting = &_osc_setting;

    if(bConnected){
        if(_osc_setting.port == 0){
            disconnect();
        } else { // reconnect
            connect();
        }
    }
}

void OscReceiver::setup(){
    if(m_interface == NULL){
        m_interface = Interface::instance();
    }

    if(!bConnected && osc_setting->port != 0){
        connect();
    }
}

void OscReceiver::update(){
    ofxOscMessage m;
    int message_count = 0;

    // check for waiting messages
    while(m_oscReceiver.hasWaitingMessages() && message_count < MAX_MESSAGES_PER_CYCLE){
        // get the next message
        m_oscReceiver.getNextMessage(m);
        ofNotifyEvent(newMessageEvent, m, this);
        message_count++;
        processMessage(m);
    }
}

void OscReceiver::destroy(){
    if(bConnected){
        disconnect();
    }
}

void OscReceiver::processMessage(ofxOscMessage &m){
    string addr,param,sub;

    addr = m.getAddress();
    if(m.getNumArgs() > 0)
        param = m.getArgAsString(0);
    else
        param = "";
    
    ofLogVerbose() << "[osc-in] " << addr << " with " << param;
    
    sub = osc_setting->addresses["effect_config"] + "/";
    if(addr.substr(0, sub.size()) == sub){
        sub = addr.substr(sub.size());
        std::size_t pos = sub.find("/");
        
        if (pos==std::string::npos){
            ofLogError() << "could not get effect-path and param name from osc address";
            return;
        }
        
        if(m.getNumArgs() < 1){
            ofLogError() << "param value missing from OSC message";
            return;
        }
        
        EffectConfig cfg;
        cfg.setting_name = sub.substr(0, pos);
        cfg.param_name = sub.substr(pos+1);
        cfg.param_value = param;
        
        ofNotifyEvent(m_interface->effectConfigEvent, cfg, m_interface);
        return;
    }
    
    sub = osc_setting->addresses["screen_config"] + "/";
    if(addr.substr(0, sub.size()) == sub){
        sub = addr.substr(sub.size());
        std::size_t pos = sub.find("/");
        
        if (pos==std::string::npos){
            ofLogError() << "could not get screen-name and param name from osc address";
            return;
        }
        
        if(m.getNumArgs() < 1){
            ofLogError() << "param value missing from OSC message";
            return;
        }
        
        // 2 params and screen config param-name end with pos?
        // then treat as two _x and _y params
        if(m.getNumArgs() == 2 && sub.substr(sub.size()-3) == "pos"){
            EffectConfig cfg;
            cfg.setting_name = sub.substr(0, pos);
            cfg.param_name = sub.substr(pos+1) + "_x";
            cfg.param_value = param;
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_y";
            cfg.param_value = m.getArgAsString(1);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        }
        
        EffectConfig cfg;
        cfg.setting_name = sub.substr(0, pos);
        cfg.param_name = sub.substr(pos+1);
        cfg.param_value = param;
        
        ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        return;
    }
    
    if(addr == osc_setting->addresses["song"]){
        ofLogVerbose() << "[osc-in] song: " << param;
        ofNotifyEvent(m_interface->songEvent, param, m_interface);
        return;
    }
    
    if(addr == osc_setting->addresses["clip"]){
        ofLogVerbose() << "[osc-in] clip: " << param;
        ofNotifyEvent(m_interface->clipEvent, param, m_interface);
        return;
    }
    
    if(addr == osc_setting->addresses["trigger"]){
        ofNotifyEvent(m_interface->triggerEvent, param, m_interface);
        return;
    }
    
    sub = osc_setting->addresses["trigger"] + "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->triggerEvent, param, m_interface);
        return;
    }
    
    if(addr == osc_setting->addresses["stop"]){
        ofNotifyEvent(m_interface->stopTriggerEvent, param, m_interface);
        return;
    }
    
    sub = osc_setting->addresses["stop"] + "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->stopTriggerEvent, param, m_interface);
        return;
    }
    
    if(addr == osc_setting->addresses["effect"]){
        ofNotifyEvent(m_interface->effectEvent, param, m_interface);
        return;
    }
    
    sub = osc_setting->addresses["effect"] + "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->effectEvent, param, m_interface);
        return;
    }
    
    if(addr == osc_setting->addresses["control"]){
        ofNotifyEvent(m_interface->controlEvent, param, m_interface);
        return;
    }
    
    sub = osc_setting->addresses["control"] + "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->controlEvent, param, m_interface);
        return;
    }
    
    ofLog() << "Unable to process OSC Message " << m.getAddress();
}

bool OscReceiver::connect(){
    // m_oscReceiver.enableReuse();

#ifdef __BOOL_OSC_SETUP__
    if(m_oscReceiver.setup(osc_setting->port)){
        bConnected = true;
        ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
        return true;
    }

    ofLogWarning() << "OscReceiver could not start listening to port: " << osc_setting->port;
    return false;
#else
    m_oscReceiver.setup(osc_setting->port);
    bConnected = true;
    ofLog() << "of2030::OscReceiver listening to port: " << osc_setting->port;
    return true;
#endif // __BOOL_OSC_SETUP__

}

void OscReceiver::disconnect(){
    // (private method) m_oscReceiver.shutdown();
    bConnected = false;
}

