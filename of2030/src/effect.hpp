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

    #define NO_TIME (-1.0f)

    class Effect{

    public: // methods

        Effect();
        void reset();
        // ~Effect(){ destroy(); }

        virtual void setup(Context &_context);
        virtual void draw(Context &_context);
        virtual void update(float dt);

        inline void truncate(){
            startTime = 0.0f;
            endTime = 0.0f;
        }

        inline bool hasStartTime() const { return startTime >= 0.0f; }
        inline bool hasEndTime() const { return endTime >= 0.0f; }

        inline float getStartTime() const { return startTime; }
        inline float getEndTime() const { return endTime; }
         inline float getDuration() const { return endTime-startTime; }
        inline int getLayer() const { return layer; }
        inline bool getUnique() const { return bUnique; }

        inline ofVideoPlayer* getVideoPlayer() const { return video_player; }
        inline void setDuration(float duration){
            endTime = startTime + duration;
        }

    protected: // methods
        
        void drawContent();
        void drawMask(const string &coordsName);
        void drawPattern(const string &patternName);

        inline float getEffectTime(){ return context->time - startTime; }
        inline float getProgress(){ return getEffectTime() / getDuration(); }

    public: // properties

        // int cid;
        // string name;
        string trigger;
        
        // static int cidCounter;

    private: // attributes

        int layer;
        float startTime, endTime;
        bool bUnique; // if there can be only one effect with this trigger value at any given moment

        ofVideoPlayer *video_player, *mask_video_player;
        ofVec3f auto_pos, auto_velocity;
        ofShader *shader;
    
        Context *context;
        PreCalc *precalc;
    };

} // namespace of2030{

#endif /* effect_hpp */
