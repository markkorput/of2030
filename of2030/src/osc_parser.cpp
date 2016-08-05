//
//  osc_parser.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#include "osc_parser.hpp"

using namespace of2030;

SINGLETON_INLINE_IMPLEMENTATION_CODE(OscParser)

OscParser::OscParser(){
    osc_interface = OscToolkit::Interface::instance();
    m_interface = Interface::instance();
}

void OscParser::setup(){
    if(osc_interface){
        registerCallbacks();
    }
}

void OscParser::destroy(){
    if(osc_interface){
        registerCallbacks(false);
    }
}

void OscParser::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(osc_interface->messageEvent, this, &OscParser::onMessage);
    } else {
        ofRemoveListener(osc_interface->messageEvent, this, &OscParser::onMessage);
    }
}

void OscParser::onMessage(ofxOscMessage &m){
    string addr,param,sub;
    
    addr = m.getAddress();
    if(m.getNumArgs() > 0)
        param = m.getArgAsString(0);
    else
        param = "";
    
    ofLogVerbose() << "[osc-in] " << addr << " with " << param;
    
    
    if(addr == OSC_TRIGGER){
        ofNotifyEvent(m_interface->triggerEvent, param, m_interface);
        return;
    }
    
    sub = OSC_TRIGGER;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->triggerEvent, param, m_interface);
        return;
    }
    
    
    sub = OSC_CONFIG_EFFECT;
    sub += "/";
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
    
    
    if(addr == OSC_PLAY){
        ofNotifyEvent(m_interface->playbackEvent, param, m_interface);
        return;
    }
    
    sub = OSC_PLAY;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->playbackEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_TRIGGERSTOP){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT){
            param = "";
        }
        
        ofNotifyEvent(m_interface->stopTriggerEvent, param, m_interface);
        return;
    }
    
    sub = OSC_TRIGGERSTOP;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->stopTriggerEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_PLAYSTOP){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT){
            param = "";
        }
        
        ofNotifyEvent(m_interface->stopPlaybackEvent, param, m_interface);
        return;
    }
    
    sub = OSC_PLAYSTOP;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->stopPlaybackEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_CONTROL){
        ofNotifyEvent(m_interface->controlEvent, param, m_interface);
        return;
    }
    
    sub = OSC_CONTROL;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->controlEvent, param, m_interface);
        return;
    }
    
    sub = OSC_CONFIG_SCREEN;
    sub += "/";
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
        
        EffectConfig cfg;
        
        // 2 params and screen config param-name end with pos?
        // then treat as two _x and _y params
        if(m.getNumArgs() == 2 && sub.substr(sub.size()-3) == "pos"){
            cfg.setting_name = sub.substr(0, pos);
            cfg.param_name = sub.substr(pos+1) + "_x";
            cfg.param_value = param;
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_y";
            cfg.param_value = m.getArgAsString(1);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        }
        
        // 2 params and screen config param-name end with pos?
        // then treat as two _x and _y params
        if(m.getNumArgs() == 3 && sub.substr(sub.size()-3) == "pos"){
            cfg.setting_name = sub.substr(0, pos);
            cfg.param_name = sub.substr(pos+1) + "_x";
            cfg.param_value = param;
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_y";
            cfg.param_value = m.getArgAsString(1);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
            cfg.param_name = sub.substr(pos+1) + "_z";
            cfg.param_value = m.getArgAsString(2);
            ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        }
        
        cfg.setting_name = sub.substr(0, pos);
        cfg.param_name = sub.substr(pos+1);
        cfg.param_value = param;
        
        ofNotifyEvent(m_interface->screenConfigEvent, cfg, m_interface);
        return;
    }
    
    if(addr == OSC_VIDLOAD){
        ofNotifyEvent(m_interface->loadVideoEvent, param, m_interface);
        return;
    }
    
    sub = OSC_VIDLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->loadVideoEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_VIDUNLOAD){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT){
            param = "";
        }
        
        ofNotifyEvent(m_interface->unloadVideoEvent, param, m_interface);
        return;
    }
    
    sub = OSC_VIDUNLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->unloadVideoEvent, param, m_interface);
        return;
    }
    
    
    if(addr == OSC_IMGLOAD){
        ofNotifyEvent(m_interface->loadImageEvent, param, m_interface);
        return;
    }
    
    sub = OSC_IMGLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->loadImageEvent, param, m_interface);
        return;
    }
    
    if(addr == OSC_IMGUNLOAD){
        ofNotifyEvent(m_interface->unloadImageEvent, param, m_interface);
        return;
    }
    
    sub = OSC_IMGUNLOAD;
    sub += "/";
    if(addr.substr(0, sub.size()) == sub){
        if(m.getNumArgs() == 1 and m.getArgType(0) == ofxOscArgType::OFXOSC_TYPE_FLOAT and m.getArgAsFloat(0) == 0.0f){
            // ignore this message; touch osc sends two message on for touch down, one for touch up, this is a touch up
            return;
        }
        
        param = addr.substr(sub.size());
        ofNotifyEvent(m_interface->unloadImageEvent, param, m_interface);
        return;
    }
    
    ofLog() << "Unable to process OSC Message " << m.getAddress();
}

