//
//  ascii_file_reader.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-05.
//
//

#include "ascii_file_reader.hpp"

using namespace OscToolkit;

AsciiFileReader::AsciiFileReader(){
    bLoop = false;
}

void AsciiFileReader::destroy(){
    if(infile.is_open())
        infile.close();
}

bool AsciiFileReader::load(string path){
    if(infile.is_open())
        infile.close();
    
    if(!ofFile::doesFileExist(path)){
        return false;
    }

    infile.open(path);
    readpath = path;
    return true;
}

AsciiFileLine* AsciiFileReader::next_line(){
    // recursion check is for empty files; without this check we could get
    // stuck in an endless while-loop
    int loopCount = 0;
    
    while(loopCount <= 1){
        AsciiFileLine* line = read_line();

        if(line)
            return line;
        
        if(!bLoop){
            infile.close();
            return NULL;
        }
        
        loopCount++;
    }
    
    ofLogWarning() << "AsciiFileReader::next_line loopCount > 1";
    return NULL;
}

AsciiFileLine* AsciiFileReader::read_line(){
    //    if(!infile.is_open()){
    //        ofLogWarning() << "[AsciiFileReader::next_line] infile not loaded";
    //        return NULL;
    //    }

    string line;

    // read line from file
    while(std::getline(infile, line)){
        if(last_line.parse(line)){
            return &last_line;
        }
    }

    return NULL;
}
