#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    //make some control vertices
    cv.resize(10);
    float radius = 220;
    
        for (int i=0; i<cv.size(); i++) {
            cv[i].set( sin( TWO_PI * float(i)/float(cv.size())) * radius, 0, sin(TWO_PI * float(i)/float(cv.size())) * radius );
        }
    
    //setup our curve
    curve.setSubdivisions( 10 );
    curve.setControlVertices( cv );

    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    //move arounf our control vertices

        for (int i=0; i<cv.size(); i++) {
            cv[i].y = sin( ofGetElapsedTimef() + i ) * 100;
        }

    
    //update the curve's vertices
    curve.update();
    
    //find a point on curve 0-1
    
    pointOnCurve = curve.getPoint( sin(ofGetElapsedTimef())*.5 +.5);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    camera.begin();
    
//    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    
    //draw the control vertices
//    ofColor color = ofColor::fromHsb(
//                                     ofMap( mouseY, 0, ofGetHeight(), 10,350 ),
//                                     100,
//                                     100
//                                     );
    
    
    
    ofColor color2 = ofColor::fromHsb(
                                     ofMap( mouseX, 0, ofGetHeight()*2, 350,10 ),
                                     100,
                                     100
                                     );
    
    ofBackground(color2);
    
    for(int j=0; j<100; j++){


        ofSetColor(255);
        for (int i=0; i<cv.size(); i++) {
            ofDrawSphere(cv[i], 2);
        }

        
        //draw a point on curve
        ofSetColor(255);
        ofDrawSphere( pointOnCurve, 4);
        
    
        //draw the curve
        ofSetColor(255);
        //    ofxSimpleSpline(curve, 0.5);
        curve.draw();
        
        ofRotate(4, 1000, 1000, 1000);
    }
    
   
    
    camera.end();
    
    ofRotate(4, 1000, 1000, 1000);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
