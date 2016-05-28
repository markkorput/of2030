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
#include "context.hpp"

namespace of2030{

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

        inline EffectType getType() const { return type; }
        inline float getStartTime() const { return startTime; }
        inline float getEndTime() const { return endTime; }
//        inline float getDuration() const { return duration; }
        float resolveDuration() const;
        
        // draw coords
        ofRectangle getDrawRect(Context &context);
        ofRectangle panoDrawRect(Context &context);
        ofRectangle tunnelDrawRect(Context &context);
        ofRectangle panoTunnelDrawRect(Context &context);

        inline ofVec2f getScreenWorldSize(Context &context){ return context.screen_setting.getValue("world_size", ofVec2f(2.67f, 2.0f)); }

        inline ofVec2f getWorldToScreenVector(Context &context){ return context.resolution / getScreenWorldSize(context); }
        inline float panoWorldToScreenPos(Context &context, float p){
            return ofMap(p - floor(p),
                         context.screen_setting.getValue("pano_start", 0.0f),
                         context.screen_setting.getValue("pano_end", 1.0f),
                         0.0,
                         context.resolution.x);
        }
        
        inline ofVideoPlayer* getVideoPlayer() const { return video_player; }

    protected: // methods
        
        void setType(EffectType effect_type);
        void drawContent(Context &context);
        void drawMask(Context &context, const string &coordsName);
        void drawVideo(Context &context, ofVec2f &drawSize);
        void drawPattern(Context &context, const string &patternName, ofVec2f &drawSize);

        inline float getGlobalTime(Context &context){ return context.time - startTime; }
        inline float getGlobalDuration(){ return endTime - startTime; }
        inline float getGlobalProgress(Context &context){ return getGlobalTime(context) / getGlobalDuration(); }

    public: // properties

        // int cid;
        string name;
        string trigger;
        
        // static int cidCounter;


    private: // attributes
        float startTime, endTime, duration;
        EffectType type;

        ofVideoPlayer* video_player;
        float pano_pos, pano_velocity;
        ofShader *shader;
    };

} // namespace of2030{

#endif /* effect_hpp */
