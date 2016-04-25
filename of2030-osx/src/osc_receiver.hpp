//
//  osc_receiver.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef osc_receiver_hpp
#define osc_receiver_hpp

//#include <stdio.h>
#include "ofxOsc.h"

#include "interface.hpp"
#include "effects.hpp"

namespace of2030 {
    
    class OscReceiver {
        const static int MAX_MESSAGES_PER_CYCLE = 10;

    public:
        OscReceiver() : m_port(2030), m_interface(NULL), m_bConnected(false){}
        ~OscReceiver(){ destroy(); }
        void configure(unsigned int port=2030, Interface* interface=NULL);
        void setup();
        void update();
        void destroy();

        unsigned int getPort(){ return m_port; };

    private: // methods
        bool connect();
        void disconnect();

        void processChangeMessage(ofxOscMessage &m);
        void processEffectMessage(ofxOscMessage &m);
        void processMessageMessage(ofxOscMessage &m);

        void getMapFromJsonString(const std::string &str, map<string, string> &map);
        effects::Effect* createEffectFromJsonString(const std::string &json_string);
        
    private: // attributes
        Interface* m_interface;
        ofxOscReceiver m_oscReceiver;
        bool m_bConnected;
        unsigned int m_port;
    };
}

#endif /* osc_receiver_hpp */
