//
//  osc_parser.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#ifndef osc_parser_hpp
#define osc_parser_hpp

#include "interface.hpp"
#include "OscToolkit/interface.hpp"
#include "shared2030.h"

namespace of2030 {
    
    class OscParser {
        SINGLETON_INLINE_HEADER_CODE(OscParser)

    public: // methods
        OscParser();
        ~OscParser(){ destroy(); }

        void setup();
        void destroy();

    private: // callbacks
        void registerCallbacks(bool _register=true);
        void onMessage(ofxOscMessage &m);

    private: // attributes
        Interface* m_interface;
        OscToolkit::Interface* osc_interface;
    };
}

#endif /* osc_parser_hpp */
