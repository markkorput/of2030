//
//  osc_recorder.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-22.
//
//

#include "osc_recorder.hpp"

#ifdef __OSC_RECORDER_ENABLED__

#include "osc_receiver.hpp"
#include "ofxOsc.h"
#include <time.h>

using namespace of2030;


SINGLETON_INLINE_IMPLEMENTATION_CODE(OscRecorder)

OscRecorder::~OscRecorder(){
    if(bRecording){
        stop_recording();
    }
}

void OscRecorder::start_recording(){
    char str[24];
    string filename;
    std::time_t t = std::time(NULL);

    if (std::strftime(str, sizeof(str), "%Y-%m-%d_%H%M%S", std::localtime(&t))) {
        filename = str;
        filename += ".csv";
    } else {
        filename = "_latest.csv";
    }
    
    ofLog() << "[OscRecorder] start recording " << filename;
    file.start_writing(ofToDataPath("osc/"+filename));
    registerCallbacks();
    bRecording=true;
}

void OscRecorder::stop_recording(){
    file.stop_writing();
    registerCallbacks(false);
    bRecording=false;
    ofLog() << "recording stopped";
}

void OscRecorder::registerCallbacks(bool _register){
    if(!osc_interface){
        osc_interface = OscInterface::instance();
    }

    if(_register){
        ofAddListener(osc_interface->messageEvent, &file, &OscAsciiFile::write_line);
    } else {
        ofRemoveListener(osc_interface->messageEvent, &file, &OscAsciiFile::write_line);
    }
}

#endif // __OSC_RECORDER_ENABLED__