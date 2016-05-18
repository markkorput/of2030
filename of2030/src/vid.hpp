//
//  vid.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-18.
//
//

#ifndef vid_hpp
#define vid_hpp

#include "effect.hpp"

namespace of2030 { namespace effects {
    
class Vid : public Effect{
public: // methods
    Vid();
    virtual void setup(Context &context);
    virtual void draw(Context &context);
    
public: // attributes
    ofVideoPlayer *video_player;
    ofMesh mesh;
    ofTexture vidtex;
};

}}

#endif /* vid_hpp */
