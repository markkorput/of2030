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
    } Context;

    
    enum EffectType{
        DEFAULT = 0,
        VID = 3,
        TUNNEL = 4,
        SPOT = 5,
        VOICE = 6
    };
    
    static map<EffectType, string> EFFECT_NAMES = {
        {DEFAULT, "default"},
        {VID, "vid"},
        {TUNNEL, "tunnel"},
        {SPOT, "spot"},
        {VOICE, "voice"}
    };

    #define NO_TIME (-1.0f)

    class Effect{

    public: // methods

        Effect();
        void reset();
        // ~Effect(){}

        virtual void setup(Context &context);
        virtual void draw(Context &context);

        inline bool hasStartTime() const { return startTime >= 0.0f; }
        inline bool hasEndTime() const { return endTime >= 0.0f; }
        inline bool hasDuration() const { return duration >= 0.0f; }

        float getDuration() const;
        

    protected: // methods
        
        void setType(EffectType effect_type);

    public: // properties

        // int cid;
        float startTime, endTime, duration;
        EffectType type;
        string name;
        string trigger;
        ofShader *shader;
        // static int cidCounter;
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
