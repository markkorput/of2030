//
//  osc_sender.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-06-11.
//
//

#ifndef osc_sender_hpp
#define osc_sender_hpp

#include "shared.h"
#include "interface.hpp"

namespace OscToolkit{

    class Sender : public Interface{
        
        OSC_TOOLKIT_SINGLETON_INLINE_HEADER_CODE(Sender)

    public: // methods

        Sender() : bEnabled(false){}
        // ~OscSender(){}
        void setup(string host, int port);
        // void destroy(){}
        
        inline bool isEnabled() const { return bEnabled; }
        void process(ofxOscMessage &message);

    public: // properties

        ofxOscSender sender;

    private: // attributes

        bool bEnabled;
        
    };
}

#endif // osc_sender_hpp
