//
//  osc_recorder.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-22.
//
//

#include "shared2030.h"

#ifdef __OSC_RECORDER_ENABLED__ // to speed op raspi compile

#ifndef osc_recorder_hpp
#define osc_recorder_hpp

#include "osc_ascii_file.hpp"

namespace of2030{
    class OscRecorder{

    SINGLETON_INLINE_HEADER_CODE(OscRecorder)

    public:
        OscRecorder() : bRecording(false){}
        ~OscRecorder();

        inline void toggle_record(){ if(bRecording) stop_recording(); else start_recording(); }
        void start_recording();
        void stop_recording();

        inline bool is_recording(){ return bRecording; }

    private:
        void registerCallbacks(bool _register=true);

    private:
        bool bRecording;
        OscAsciiFile file;
    };
}

#endif /* osc_recorder_hpp */

#endif