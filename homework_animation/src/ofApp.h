#pragma once

#include "ofMain.h"
#include "ofxSimpleSpline.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        vector<ofVec3f> cv;
    
        ofEasyCam camera;
    
    //*********************************
    //ofxSimpleSpline is created by myself?
    //*********************************
    
        ofxSimpleSpline curve;
    
        ofVec3f pointOnCurve;
		
};


