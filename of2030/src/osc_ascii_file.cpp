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

void OscAsciiFile::load(string path){
    if(infile.is_open())
        infile.close();

    infile.open(path);
    readpath = path;
}

OscAsciiLine* OscAsciiFile::next_line(int recursion_count){
    if(recursion_count > 1){
        ofLogWarning() << "[OscAsciiFile::next_line] recursion count > 1";
        return NULL;
    }

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
    
    // loop
    if(bLoop){
        reset();
        OscAsciiLine* line = next_line(recursion_count+1);
        if(line){
            return line;
        }
        // else continue closing file
    }

    // done
    infile.close();
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
