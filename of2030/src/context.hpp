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

    typedef struct {
        float time;
        XmlItemSetting effect_setting;
        XmlItemSetting screen_setting;
        // ofFbo* fbo;
        ofFbo* fbo2;
        ofFbo* fbo3;
    } Context;



    class PreCalc {

    public: // methods

        PreCalc(Context &_context);

        inline float panoWorldToScreen(float p);
        ofRectangle panoDrawRect();
        ofRectangle tunnelDrawRect();
        inline ofRectangle panoTunnelDrawRect(){ return panoDrawRect().getIntersection(tunnelDrawRect()); }

    public: // properties

        ofVec2f resolution;
        ofVec2f scrWorldSize, worldToScreenVec2f, scrDrawSize;
        ofColor color;
        float scrPanoStart, scrPanoEnd, fxPanoStart, fxPanoEnd;
        
    private: // attributes
        
        Context* context;

        
    };

    // inline function implementations

    float PreCalc::panoWorldToScreen(float p){
        return ofMap(p - floor(p),
                     scrPanoStart,
                     scrPanoEnd,
                     0.0,
                     resolution.x);
    }
}

#endif /* context_hpp */
