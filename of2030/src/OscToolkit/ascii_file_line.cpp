//
//  ascii_file_line.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#include "ascii_file_line.hpp"


using namespace OscToolkit;

bool AsciiFileLine::parse(const string &ascii){
    string column, value;
    std::stringstream linestream(ascii);

    // read timestamp from line
    if(!std::getline(linestream,column,','))
        // try next line
        return false;

    timestamp = ofToFloat(column);
    
    // read message address
    if(!std::getline(linestream,column,','))
        return false;

    // we're gonna populate our last_line.message instance
    // with data from the current line
    message.clear();
    message.setAddress(column);

    while(std::getline(linestream,column,',') && std::getline(linestream,value,',')){
        if(column == "i"){
            message.addIntArg(ofToInt(value));
            continue;
        }
        
        if(column == "f"){
            message.addFloatArg(ofToFloat(value));
            continue;
        }
        
        if(column != "s"){
            ofLogWarning() << "AsciiFileLine only supports f (float), i (int) and s (string) args. Got a '" << column << "' arg, treating as string";
            ofLogWarning() << "full line: " << ascii;
        }

        message.addStringArg(value);
    }

    return true;
}
void AsciiFileLine::write(const ofxOscMessage &message, float timestamp, std::ofstream &stream){
    // write timestamp and address
    stream << timestamp << "," << message.getAddress();

    // write args (type and value)
    for(int i=0; i<message.getNumArgs(); i++){
        stream << ',' << (char)message.getArgType(i) << ',' << message.getArgAsString(i);
    }

    // finalise this line
    stream << '\n';
}