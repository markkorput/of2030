//
//  osc_ascii_file.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#include "osc_ascii_file.hpp"

OscAsciiFile::OscAsciiFile(){
    bLoop = false;
}

void OscAsciiFile::destroy(){
    if(infile.is_open())
        infile.close();

    if(outfile.is_open())
        outfile.close();
}

bool OscAsciiFile::load(string path){
    if(infile.is_open())
        infile.close();

    if(!ofFile::doesFileExist(path)){
        return false;
    }

    infile.open(path);
    readpath = path;
    return true;
}

OscAsciiLine* OscAsciiFile::next_line(){
    // recursion check is for empty files; without this check we could get
    // stuck in an endless while-loop
    int loopCount = 0;

    while(loopCount <= 1){
        OscAsciiLine* line = read_line();
        
        if(line)
            return line;

        if(!bLoop){
            infile.close();
            return NULL;
        }

        loopCount++;
    }

    ofLogWarning() << "OscAsciiFile::next_line loopCount > 1";
    return NULL;
}

OscAsciiLine* OscAsciiFile::read_line(){
//    if(!infile.is_open()){
//        ofLogWarning() << "[OscAsciiFile::next_line] infile not loaded";
//        return NULL;
//    }

    string line, column, value;

    // read line from file
    while(std::getline(infile, line)){
        std::stringstream linestream(line);

        // read timestamp from line
        if(!std::getline(linestream,column,','))
            // try next line
            continue;

        last_line.timestamp = ofToFloat(column);

        // read message address
        if(!std::getline(linestream,column,','))
            continue;

        // we're gonna populate our last_line.message instance
        // with data from the current line
        ofxOscMessage *msg = &last_line.message;
        msg->clear();
        msg->setAddress(column);

        while(std::getline(linestream,column,',') && std::getline(linestream,value,',')){
            if(column == "i"){
                msg->addIntArg(ofToInt(value));
                continue;
            }
            
            if(column == "f"){
                msg->addFloatArg(ofToFloat(value));
                continue;
            }
            
            if(column != "s"){
                ofLogWarning() << "OscAsciiFile only support f (float), i (int) and s (string) args, treating as string";
                ofLogWarning() << "got line: " << line;
            }
            
            msg->addStringArg(value);
        }

        // done, return pointer to populated OscAsciiLine struct
        return &last_line;
    }

    return NULL;
}

void OscAsciiFile::start_writing(string path){
    if(outfile.is_open()){
        stop_writing();
    }

    outfile.open(path);
    start_time = ofGetElapsedTimef();
    first_message_time = 0.0f;
}

void OscAsciiFile::stop_writing(){
    outfile.close();
}

void OscAsciiFile::write_line(ofxOscMessage &msg){
    float t = ofGetElapsedTimef();

    if(first_message_time < start_time){
        first_message_time = t;
        t = 0.0;
    } else {
        t -= first_message_time;
    }

    write_line(msg, t);
}

void OscAsciiFile::write_line(const ofxOscMessage &msg, float timestamp){
    string value;
    char typ;

    // write timestamp and message address
    outfile << timestamp << ',' << msg.getAddress();

    // write args (type and value)
    for(int i=0; i<msg.getNumArgs(); i++){
        typ = msg.getArgType(i);
        value = msg.getArgAsString(i);
        outfile << ',' << typ << ',' << value;
    }

    // finalise this line
    outfile << '\n';
    // ofLog() << "OscAsciiFile::write_line";
}
