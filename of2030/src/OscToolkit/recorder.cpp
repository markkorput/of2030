//
//  osc_recorder.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-22.
//
//

#include "recorder.hpp"
#include <time.h>

using namespace OscToolkit;


OSC_TOOLKIT_SINGLETON_INLINE_IMPLEMENTATION_CODE(Recorder)

Recorder::~Recorder(){
    if(bRecording){
        stop();
    }
}

void Recorder::start(){
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
    file_writer.start(ofToDataPath("osc/"+filename));
    registerCallbacks();
    bRecording=true;
}

void Recorder::stop(){
    file_writer.stop();
    registerCallbacks(false);
    bRecording=false;
    ofLog() << "recording stopped";
}

void Recorder::setInterface(Interface* new_interface){
    bool restart = bRecording;
    
    if(restart){
        stop();
    }
    
    interface = new_interface;
    
    if(restart){
        start();
    }
}

void Recorder::registerCallbacks(bool _register){
    if(!interface){
        interface = Interface::instance();
    }

    if(_register){
        ofAddListener(interface->messageEvent, &file_writer, &AsciiFileWriter::process);
    } else {
        ofRemoveListener(interface->messageEvent, &file_writer, &AsciiFileWriter::process);
    }
}
