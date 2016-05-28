//
//  context.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-28.
//
//

#ifndef context_hpp
#define context_hpp

#include "ofMain.h"
#include "setting_types.h"

namespace of2030{
    class Context {
    public:
        float time;
        XmlItemSetting effect_setting;
        XmlItemSetting screen_setting;
        ofFbo* fbo;
        ofFbo* fbo2;
        ofFbo* fbo3;
    };
}

#endif /* context_hpp */
