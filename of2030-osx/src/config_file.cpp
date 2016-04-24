//
//  config_file.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-24.
//
//

#include "config_file.hpp"
#include <stdio.h>
#include <string.h>



using namespace of2030;

ConfigFile::ConfigFile() : m_bLoaded(false){
}

void ConfigFile::setPath(string path){
    m_path = path;

    if(m_bLoaded){
        // reload from new path
        load();
    }
}

void ConfigFile::load(){
    ofFile file(ofToDataPath(m_path), ofFile::Mode::ReadOnly, false);
    ofBuffer buf = file.readToBuffer();;
    ofLogVerbose() << "[ConfigFile] Read file content: " << buf;
    m_json.parse(buf);
    m_bLoaded = true;
}

string ConfigFile::getValue(std::string valuePath){
    if(!m_bLoaded)
        load();

    Json::Value jsonTmp = m_json;

    // convert from const char * to char *
    char * dup = strdup(valuePath.c_str());
    char * pch = strtok(dup, ".");

    while(pch != NULL)
    {
        // ofLog() << "looking for " << pch;
        jsonTmp = jsonTmp[pch];
        // ofLog() << jsonTmp.asString();
        pch = strtok(NULL, ".");
    }

    free(dup);

    return jsonTmp.asString();
}