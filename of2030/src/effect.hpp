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
        virtual void draw(Context &_context, float dt);
        // virtual void update(float dt);

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

        inline void setDuration(float duration){
            endTime = startTime + duration;
        }

        inline ofVideoPlayer* getVideoPlayer() const { return video_player; }
        inline ofVideoPlayer* getMaskVideoPlayer() const { return mask_video_player; }
        inline ofImage* getImage() const { return image; }
        inline ofImage* getMaskImage() const { return mask_image; }
        inline ofBlendMode getBlendMode() const { return blendMode; }
        inline bool getUnload() const { return bUnload; }
        inline bool unloadImages() const { return bUnload; }
        inline bool unloadVideos() const { return bUnload; }

    protected: // methods
        
        void drawContent();
        void drawMask(const string &coordsName);
        void drawPattern(const string &patternName);

        inline float getEffectTime(){ return context->time - startTime; }
        inline float getProgress(){ return getEffectTime() / getDuration(); }
        static inline ofVideoPlayer* getVideoPlayer(Context &contxt);
        inline ofImage* loadImage(Context &contxt);

    public: // properties

        // int cid;
        // string name;
        string trigger;
        
        // static int cidCounter;

    private: // attributes

        int layer;
        float startTime, endTime;
        bool bUnique, bUnload; // if there can be only one effect with this trigger value at any given moment

//        enum ofBlendMode{
//            OF_BLENDMODE_DISABLED = 0,
//            OF_BLENDMODE_ALPHA 	  = 1,
//            OF_BLENDMODE_ADD 	  = 2,
//            OF_BLENDMODE_SUBTRACT = 3,
//            OF_BLENDMODE_MULTIPLY = 4,
//            OF_BLENDMODE_SCREEN   = 5
//        };
        ofBlendMode blendMode;
        
        
        ofImage *image, *mask_image;
        ofVideoPlayer *video_player, *mask_video_player;
        bool bVidStarted;
        ofVec3f auto_pos, auto_rotation;
        float auto_alpha;
        ofShader *shader;
    
        Context *context;
        PreCalc *precalc;
    };

} // namespace of2030{

#endif /* effect_hpp */
