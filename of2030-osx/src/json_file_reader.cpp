//
//  json_file_reader.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-25.
//
//

#include "json_file_reader.hpp"

using namespace of2030;

JsonFileReader::JsonFileReader() : m_bLoaded(false){
}

void JsonFileReader::setPath(string path){
    m_path = path;
    
    if(m_bLoaded){
        // reload from new path
        load();
    }
}

void JsonFileReader::load(){
    ofFile file(ofToDataPath(m_path), ofFile::Mode::ReadOnly, false);
    ofBuffer buf = file.readToBuffer();;
    // ofLogVerbose() << "[JsonFileReader] Read file content: " << buf;
    m_json.parse(buf);
    m_bLoaded = true;
}

string JsonFileReader::getValue(std::string valuePath){
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