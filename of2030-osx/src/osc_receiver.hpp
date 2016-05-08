//
//  osc_receiver.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef osc_receiver_hpp
#define osc_receiver_hpp

#include "ofxOsc.h"
#include "interface.hpp"
#include "effects.hpp"

namespace of2030 {
    
    class OscReceiver {

    public:
        OscReceiver();
        ~OscReceiver(){ destroy(); }
        void configure(unsigned int port=2030, Interface* interface=NULL);
        void configure(OscSetting &_osc_setting);
        void setup();
        void update();
        void destroy();

    private: // methods
        bool connect();
        void disconnect();

//        bool processJsonEffectMessage(ofxOscMessage &m);
//        void getMapFromJsonString(const std::string &str, map<string, string> &map);
//        effects::Effect* createEffectFromJsonString(const std::string &json_string);

        bool processCtrlMessage(ofxOscMessage &m);
        
    private: // attributes
        Interface* m_interface;
        ofxOscReceiver m_oscReceiver;
        OscSetting *osc_setting;
        OscSetting default_setting;
        bool m_bConnected;
        const static int MAX_MESSAGES_PER_CYCLE = 10;
    };
}

#endif /* osc_receiver_hpp */
