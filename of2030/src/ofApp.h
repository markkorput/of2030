#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void exit(ofEventArgs &args);
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    private: // callbacks
        void onControl(string &type);
        void onPlayback(string &type);
        void onStopPlayback(string &type);
        void onLoadVideo(string &name);
        void onUnloadVideo(string &name);
        void onVideoPlayerUnload(ofVideoPlayer &player);

    private: // attributes
        float next_log_alive_time;
};
