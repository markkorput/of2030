//
//  effect.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#ifndef effect_hpp
#define effect_hpp

//#include <stdio.h>
#include "ofMain.h"
#include "setting_types.h"

namespace of2030{ namespace effects {

    
    typedef struct {
        float time;
        XmlItemSetting effect_setting;
        XmlItemSetting screen_setting;
        ofFbo* fbo;
        ofFbo* fbo2;
        ofFbo* fbo3;
    } Context;

    
    enum EffectType{
        DEFAULT = 0,
        VID = 3,
        TUNNEL = 4,
        SPOT = 5,
        VOICE = 6,
        POS = 7,
        ROOF = 8
    };
    
    static map<EffectType, string> EFFECT_NAMES = {
        {DEFAULT, "default"},
        {VID, "vid"},
        {TUNNEL, "tunnel"},
        {SPOT, "spot"},
        {VOICE, "voice"},
        {POS, "pos"},
        {ROOF, "roof"}
    };

    #define NO_TIME (-1.0f)

    class Effect{

    public: // methods

        Effect();
        void reset();
        // ~Effect(){ destroy(); }

        virtual void setup(Context &context);
        virtual void draw(Context &context);
        virtual void update(float dt);

        inline bool hasStartTime() const { return startTime >= 0.0f; }
        inline bool hasEndTime() const { return endTime >= 0.0f; }
        inline bool hasDuration() const { return duration >= 0.0f; }

        
        // draw coords
        ofRectangle getDrawRect(Context &context);
        ofRectangle panoDrawRect(Context &context);
        ofRectangle tunnelDrawRect(Context &context);
        ofRectangle panoTunnelDrawRect(Context &context);
        inline ofVec2f getResolution(Context &context){ return ofVec2f(context.fbo->getWidth(), context.fbo->getHeight()); }
        inline ofVec2f getScreenWorldSize(Context &context){ return ofVec2f(context.screen_setting.getValue("world_width", 2.67f),
                                                                            context.screen_setting.getValue("world_height", 2.0f)); }
        inline ofVec2f getWorldToScreenVector(Context &context){ return getResolution(context) / getScreenWorldSize(context); }
        inline float panoWorldToScreenPos(Context &context, float p){
            return ofMap(p - floor(p),
                         context.screen_setting.getValue("pano_start", 0.0f),
                         context.screen_setting.getValue("pano_end", 1.0f),
                         0.0,
                         getResolution(context).x);
        }
        float getDuration() const;


    protected: // methods
        
        void setType(EffectType effect_type);
        void drawContent(Context &context);
        void drawMask(Context &context, const string &coordsName, const ofVec2f &resolution);
        void drawVideo(Context &context, ofVec2f &drawSize);
        void drawPattern(Context &context, const string &patternName, ofVec2f &drawSize);

    public: // properties

        // int cid;
        float startTime, endTime, duration;
        EffectType type;
        string name;
        string trigger;
        ofShader *shader;
        // static int cidCounter;
        float pano_pos, pano_velocity;

    private: // attributes
        
        ofVideoPlayer* video_player;
    };

    // === === === === === === === === ===

    class EffectLogic{
    public:
        EffectLogic(Effect *_effect, Context *_context) : effect(_effect), context(_context){}
        inline float getGlobalTime(){ return context->time - effect->startTime; }
        inline float getGlobalDuration(){ return effect->endTime - effect->startTime; }
        inline float getGlobalProgress(){ return getGlobalTime() / getGlobalDuration(); }

        Context *context;
        Effect *effect;
    };

    // === === === === === === === === ===
    
    class Tunnel : public Effect{
    public: // methods
        Tunnel();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

}} // namespace of2030{ namespace effects {

#endif /* effect_hpp */
