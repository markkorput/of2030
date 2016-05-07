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
        int client_id, client_index, client_count;
        ClientSetting *client_setting;
        EffectSetting effect_setting;
        ofFbo* fbo;
    } Context;

    enum EffectType{
        OFF = 0,
        COLOR = 1,
        VID = 2,
        SHADER = 3,
        CURSOR = 4
    };
    
    static map<EffectType, string> EFFECT_NAMES = {
        {OFF, "off"},
        {COLOR, "color"},
        {CURSOR, "cursor"},
        {VID, "vid"},
        {SHADER, "shader"}
    };

    #define NO_TIME (-1.0f)

    class Effect{

    public: // methods

        Effect();
        // ~Effect(){}

        virtual void setup(Context &context);
        virtual void draw(Context &context);

        bool hasStartTime(){ return startTime >= 0.0f; }
        bool hasEndTime(){ return endTime >= 0.0f; }
        bool hasDuration(){ return duration >= 0.0f; }

        float getDuration();
        

    protected: // methods
        
        void setType(EffectType effect_type);

    public: // properties

        int cid;
        float startTime, endTime, duration;
        EffectType type;
        string name;

        static int cidCounter;
    };

    class EffectLogic{
    public:
        EffectLogic(Effect *_effect, Context *_context) : effect(_effect), context(_context){}
        inline float getGlobalTime();
        inline float getGlobalDuration();
        inline float getGlobalProgress();

        Context *context;
        Effect *effect;
    };


    class Off : public Effect{

    public: // methods
        Off();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };



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

    class Cursor : public Effect{

    public: // methods
        Cursor();
        // virtual void setup(Context &context);
        virtual void draw(Context &context);
    };

    class CursorLogic : public EffectLogic{
    public:
        CursorLogic(Effect *_effect, Context *_context) : EffectLogic(_effect, _context){}
        inline float getIterations();
        inline float getIterationDuration();
        inline int getCurrentIteration();
        inline float getIterationTime();
        inline float getIterationProgress();
        inline float getLocalProgress();
    };


    class ShaderEffect : public Effect{
    public:
        ShaderEffect();
        virtual void setup(Context &context);
        virtual void draw(Context &context);
        void setShader(string _name);
    public:
        string shaderName;
        ofShader *shader;
    };

    class Vid : public Effect{
    public: // methods
        Vid();
        virtual void setup(Context &context);
        virtual void draw(Context &context);

    public: // attributes
        ofVideoPlayer *video_player;
    };

}} // namespace of2030{ namespace effects {

#endif /* effect_hpp */
