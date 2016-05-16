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
    } Context;

    
    enum EffectType{
        DEFAULT = 0,
        OFF = 1,
        COLOR = 2,
        VID = 3,
        TUNNEL = 4,
        SPOT = 5,
        VOICE = 6
    };
    
    static map<EffectType, string> EFFECT_NAMES = {
        {DEFAULT, "default"},
        {OFF, "off"},
        {COLOR, "color"},
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
        inline float getGlobalTime();
        inline float getGlobalDuration();
        inline float getGlobalProgress();

        Context *context;
        Effect *effect;
    };

    // === === === === === === === === ===

    class Off : public Effect{

    public: // methods
        Off();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

    // === === === === === === === === ===

    class Color : public Effect{

    public: // methods
        Color();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);

        float getGlobalDuration();
        float getIterations();

    public: // attributes
        ofColor color;
    };

    // === === === === === === === === ===

    class Vid : public Effect{
    public: // methods
        Vid();
        virtual void setup(Context &context);
        virtual void draw(Context &context);

    public: // attributes
        ofVideoPlayer *video_player;
    };

    // === === === === === === === === ===
    
    class Tunnel : public Effect{
    public: // methods
        Tunnel();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

    // === === === === === === === === ===
    
    class Spot : public Effect{
    public: // methods
        Spot();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

    // === === === === === === === === ===
    
    class Voice : public Effect{
    public: // methods
        Voice();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

}} // namespace of2030{ namespace effects {

#endif /* effect_hpp */
