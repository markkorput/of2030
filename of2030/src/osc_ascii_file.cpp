//
//  osc_ascii_file.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-05-21.
//
//

#include "osc_ascii_file.hpp"

OscAsciiFile::OscAsciiFile() : file(NULL){
    // last_line.message.setRemoteEndpoint("127.0.0.1", 2031);
}

void OscAsciiFile::destroy(){
    if(file)
        delete file;
}

void OscAsciiFile::load(string path){
    if(file)
        delete file;

    file = new std::ifstream(path);
}

OscAsciiLine* OscAsciiFile::next_line(){
    if(!file){
        ofLogWarning() << "[OscAsciiFile::next_line] file == NULL";
        return NULL;
    }

    string line, column, value;

    // read line from file
    while(std::getline(*file, line)){
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
                ofLogWarning() << "OscAsciiFile only support f (float), i (int) and s (string) args, treating as string -- was:", column;
            }
            
            msg->addStringArg(value);
        }

        // done, return pointer to populated OscAsciiLine struct
        return &last_line;
    }

    return NULL;
}
