//
//  osc_interface.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-04.
//
//

#ifndef osc_interface_hpp
#define osc_interface_hpp

#include "shared.h"

#include "ofxOsc.h"

namespace OscToolkit {
    class Interface {
        OSC_TOOLKIT_SINGLETON_INLINE_HEADER_CODE(Interface)

    public: // method: input
        void process(ofxOscMessage &message);

    public: // event: output
        ofEvent<ofxOscMessage> messageEvent; // triggered by every possible source (also recording playback)
    };
};

#endif /* osc_interface_hpp */
