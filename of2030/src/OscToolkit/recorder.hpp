//
//  osc_recorder.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-22.
//
//

#ifndef osc_recorder_hpp
#define osc_recorder_hpp

#include "shared.h"
#include "interface.hpp"
#include "ascii_file_writer.hpp"

namespace OscToolkit{

    class Recorder{

    OSC_TOOLKIT_SINGLETON_INLINE_HEADER_CODE(Recorder)

    public:
        Recorder() : bRecording(false), interface(NULL){}
        ~Recorder();

        inline void toggle(){ if(bRecording) stop(); else start(); }
        void start();
        void stop();

        inline bool is_recording(){ return bRecording; }

        inline Interface* getInterface(){ return interface; }
        void setInterface(Interface* new_interface);

    private:
        void registerCallbacks(bool _register=true);

    private:
        Interface *interface;
        bool bRecording;
        AsciiFileWriter file_writer;
    };

} // namespace OscToolkit

#endif /* osc_recorder_hpp */
