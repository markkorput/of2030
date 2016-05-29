//
//  osc_receiver.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef osc_receiver_hpp
#define osc_receiver_hpp

#include "shared2030.h"
#include "ofxOsc.h"
#include "interface.hpp"
#include "effects.hpp"

namespace of2030 {
    
    class OscReceiver {
        SINGLETON_INLINE_HEADER_CODE(OscReceiver)

    public: // methods
        OscReceiver();
        ~OscReceiver(){ destroy(); }

        void configure(OscSetting &_osc_setting);
        void setup();
        void update();
        void destroy();

        inline bool isConnected(){ return bConnected; }
        void processMessage(ofxOscMessage &m);

    public: //events
        ofEvent<ofxOscMessage> newMessageEvent;

    private: // methods
        bool connect();
        void disconnect();
        
    private: // attributes
        Interface* m_interface;
        ofxOscReceiver m_oscReceiver;
        OscSetting *osc_setting;
        OscSetting default_setting;
        bool bConnected;
        const static int MAX_MESSAGES_PER_CYCLE = 10;
    };
}

#endif /* osc_receiver_hpp */
