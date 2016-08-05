//
//  ascii_file_writer.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#include "ascii_file_writer.hpp"
#include "ascii_file_line.hpp"

using namespace OscToolkit;

void AsciiFileWriter::destroy(){
    if(outfile.is_open())
        outfile.close();
}

void AsciiFileWriter::start(string path){
    if(outfile.is_open()){
        stop();
    }
    
    outfile.open(path);
    start_time = ofGetElapsedTimef();
    first_message_time = 0.0f;
}

void AsciiFileWriter::stop(){
    outfile.close();
}

void AsciiFileWriter::process(ofxOscMessage &message){
    float t = ofGetElapsedTimef();
    
    if(first_message_time < start_time){
        first_message_time = t;
        t = 0.0f;
    } else {
        t -= first_message_time;
    }

    AsciiFileLine::write(message, t, outfile);
    
    // call parent class' implementation
    Interface::process(message);
}