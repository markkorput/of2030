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

        ofVec2f tunnel_size; // real world tunnel size in meters
        ofVec2f pano_size;
    } Context;



    class PreCalc {

    public: // methods

        PreCalc(){ reset(); }
        inline void reset(){ context = NULL; bIsSpot = false; }
        void load(Context &_context);

//        inline float panoWorldToScreen(float p);
//        inline ofRectangle panoTunnelDrawRect(){ return panoDrawRect().getIntersection(tunnelDrawRect()); }
        // void readTunnelScreenCoords(ofVec2f *target);

//    protected:
//        
//         ofRectangle panoDrawRect();
//         ofRectangle tunnelDrawRect();

    public: // properties

        bool bIsSpot;
        ofVec2f resolution;
        ofVec2f /*worldToScreenVec2f,*/ scrDrawSize;
        ofColor color;
        ofVec3f translate, rotate, scale, effect_translate, effect_scale, effect_rotate;

    private: // attributes

        Context* context;
//        bool isTunnel, isPano;
    };

    // inline function implementations

//    float PreCalc::panoWorldToScreen(float p){
//        return ofMap(p - floor(p),
//                     scrPanoStart,
//                     scrPanoEnd,
//                     0.0,
//                     resolution.x);
//    }
}

#endif /* context_hpp */
