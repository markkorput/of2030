#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void exit(ofEventArgs &args);
    
		void keyPressed(int key);
        void dragEvent(ofDragInfo dragInfo);
//		void keyReleased(int key);
//		void mouseMoved(int x, int y);
//		void mouseDragged(int x, int y, int button);
//		void mousePressed(int x, int y, int button);
//		void mouseReleased(int x, int y, int button);
//		void mouseEntered(int x, int y);
//		void mouseExited(int x, int y);
//		void windowResized(int w, int h);
//		void gotMessage(ofMessage msg);
    
    private: // callbacks
        void onControl(string &type);
        void onPlayback(string &type);
        void onStopPlayback(string &type);
    
        void onLoadVideo(string &name);
        void onUnloadVideo(string &name);
        void onVideoPlayerUnload(ofVideoPlayer &player);
    
        void onLoadImage(string &name);
        void onUnloadImage(string &name);
        void onImageUnload(ofImage &image);

    private: // attributes
        float next_log_alive_time, log_alive_interval;
        float last_update_time;
};
