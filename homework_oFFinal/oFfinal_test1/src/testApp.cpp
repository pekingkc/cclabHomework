#include "testApp.h"

//--------------------------------------------------------------
//----------------------  Params -------------------------------
//--------------------------------------------------------------
Params param;        //Definition of global variable

void Params::setup() {
    eCenter = ofPoint( ofGetWidth() / 2, ofGetHeight() / 2 );
    eRad = 50;
    velRad = 200;
    lifeTime = 1.0;
    
    rotate = 90;
}

//--------------------------------------------------------------
//----------------------  Particle  ----------------------------
//--------------------------------------------------------------
Particle::Particle() {
    live = false;
}

//--------------------------------------------------------------
ofPoint randomPointInCircle( float maxRad ){
    ofPoint pnt;
    float rad = ofRandom( 0, maxRad );
    float angle = ofRandom( 0, PI*4 );
    
    //make the main visual in full screen
    pnt.x = sin( angle+PI/4 ) * rad * 10 ;
    pnt.y = cos( angle+PI/4 ) * rad * 10;
    return pnt;
    ofRotate(angle);
}

//--------------------------------------------------------------
void Particle::setup() {
    pos = param.eCenter + randomPointInCircle( param.eRad );
    vel = randomPointInCircle( param.velRad );
    time = 0;
    lifeTime = param.lifeTime;
    live = true;
    
}

//--------------------------------------------------------------
void Particle::update( float dt ){
    if ( live ) {
        //Rotate vel
        vel.rotate( 0, 0, param.rotate * dt );
        
        //Update pos
        pos += vel * dt;    //Euler method
        
        //Update time and check if particle should die
        time += dt;
        if ( time >= lifeTime ) {
            live = false;   //Particle is now considered as died
        }
    }
}

//--------------------------------------------------------------
//use arguement to get input in
void Particle::draw(float x){
    if ( live ) {
        //Compute size
        float size = x;
        
        //Compute color
        ofColor color = ofColor::red;
        float hue = ofMap( time, 0, lifeTime, 100, 255 );
        color.setHue( hue );
        ofSetColor( color );
        
        //use the value from soundInput to draw
        ofCircle( pos, size*1.5 );  //Draw particle
    }
}

//--------------------------------------------------------------
//----------------------  testApp  -----------------------------
//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate( 60 );	//Set screen frame rate
    
    //Allocate drawing buffer
    int w = ofGetWidth();
    int h = ofGetHeight();
    fbo.allocate( w, h, GL_RGB32F_ARB );
    
    //Fill buffer with white color
    fbo.begin();
    ofBackground(255, 255, 255);
    fbo.end();
    
    //Set up parameters
    param.setup();		//Global parameters
    history = 0.995;
    bornRate = 1000;
    
    bornCount = 0;
    time0 = ofGetElapsedTimef();

    //setup the audioInput
    //******************************************************************************
    //******************************************************************************
    soundStream.listDevices();
    int bufferSize = 256;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    //******************************************************************************
    //******************************************************************************
}

//--------------------------------------------------------------
void testApp::update(){
    //Compute dt
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - time0, 0, 0.1 );
    time0 = time;
    
    //Delete inactive particles
    int i=0;
    while (i < p.size()) {
        if ( !p[i].live ) {
            p.erase( p.begin() + i );
        }
        else {
            i++;
        }
    }
    
    //Born new particles
    bornCount += dt * bornRate;      //Update bornCount value
    if ( bornCount >= 1 ) {          //It's time to born particle(s)
        int bornN = int( bornCount );//How many born
        bornCount -= bornN;          //Correct bornCount value
        for (int i=0; i<bornN; i++) {
            Particle newP;
            newP.setup();            //Start a new particle
            p.push_back( newP );     //Add this particle to array
        }
    }
    
    //Update the particles
    for (int i=0; i<p.size(); i++) {
        p[i].update( dt );
    }
    
    //******************************************************************************
    //******************************************************************************
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    //******************************************************************************
    //******************************************************************************
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground( 255, 255, 255 );  //Set white background
    
    //1. Drawing to buffer
    fbo.begin();
    
    //Draw semi-transparent white rectangle
    //to slightly clearing a buffer (depends on history value)
    
    ofEnableAlphaBlending();         //Enable transparency
    
    float alpha = (1-history) * 255;
    ofSetColor( 255, 255, 255, alpha );
    ofFill();
    ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
    
    ofDisableAlphaBlending();        //Disable transparency
    
    //Draw the particles
    ofFill();
    for (int i=0; i<p.size(); i++) {
        //give the input in, and use it to change the "size"
        p[i].draw(scaledVol);
        
    }
    
    fbo.end();
    
    //2. Draw buffer on the screen
    ofSetColor( 255, 255, 255 );
    fbo.draw( 0, 0 );
    ofRotate(scaledVol*10);
    
    //******************************************************************************
    //******************************************************************************
//    ofPushMatrix();
//        ofTranslate(565, 170, 0);
//        ofSetColor(245, 58, 135);
//        ofFill();
//        ofCircle(200, 200, scaledVol * 250.0f);
//        ofPopMatrix();
//    ofPopStyle();
    //******************************************************************************
    //******************************************************************************
}



void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}

//void Particle::audioIn(float * input, int bufferSize, int nChannels){
//    
//    float curVol = 0.0;
//    
//    // samples are "interleaved"
//    int numCounted = 0;
//    
//    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
//    for (int i = 0; i < bufferSize; i++){
//        left[i]		= input[i*2]*0.5;
//        right[i]	= input[i*2+1]*0.5;
//        
//        curVol += left[i] * left[i];
//        curVol += right[i] * right[i];
//        numCounted+=2;
//    }
//    
//    //this is how we get the mean of rms :)
//    curVol /= (float)numCounted;
//    
//    // this is how we get the root of rms :)
//    curVol = sqrt( curVol );
//    
//    smoothedVol *= 0.93;
//    smoothedVol += 0.07 * curVol;
//    
//    bufferCounter++;
//    
//}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}