//
//  osc_playback.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#ifndef osc_playback_hpp
#define osc_playback_hpp

#include "osc_interface.hpp"
#include "ascii_file_reader.hpp"

namespace OscToolkit {

    class Playback : public of2030::OscInterface {

    public:
        Playback(AsciiFileReader &_file){
            bLoop = false;
            file = &_file;
        }

        void start();
        // returns true if not ended yet
        bool update(float dt);
        inline AsciiFileReader* getFile() const { return file; }

    public: // events
        ofEvent<Playback> loopEvent;

    private: // attributes
        bool bLoop;
        float time;
        AsciiFileReader *file;
        AsciiFileLine *pending_line;
    };
}

#endif /* osc_playback_hpp */
