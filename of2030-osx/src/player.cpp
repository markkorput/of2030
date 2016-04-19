//
//  player.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-04-19.
//
//

#include "player.hpp"

using namespace of2030;

void Player::update(){
    if(m_bPlaying){
        setPlaybackTime(ofGetElapsedTimef() - m_startTime);
    }
}

void Player::update(float dt){
    if(m_bPlaying){
        setPlaybackTime(m_time + dt);
    }
}

void Player::start(){
    m_startTime = ofGetElapsedTimef();
    m_time = 0.0f;
    m_bPlaying = true;
}

void Player::stop(){
    m_bPlaying = false;
}

void Player::setPlaybackTime(float time){
    // TODO; activate all effects that start between m_time and time
    // TODO; de-activate all active effects that end between m_time and time
    m_time = time;    
}

