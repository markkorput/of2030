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

void ConfigFile::load(){
    JsonFileReader::load();
    client_ids = m_json["py2030"]["clients"].getMemberNames();
}

int ConfigFile::getClientCount(){
    if(!m_bLoaded) load();
    return client_ids.size();
}

string ConfigFile::getClientId(int idx){
    return client_ids[idx];
}
