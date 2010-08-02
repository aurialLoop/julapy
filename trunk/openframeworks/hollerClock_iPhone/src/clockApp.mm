#include "clockApp.h"

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void clockApp :: setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
	
	//-------------------
	
	deviceType = DEVICE_IPAD;
	deviceType = DEVICE_IPHONE;

	ofBackground( 0, 0, 0 );
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	font.loadFont( "fonts/cooperBlack.ttf", 40 );
	
	secTwoSound.loadSound( ofToDataPath( "sound/boop_1.caf" ) );
	secOneSound.loadSound( ofToDataPath( "sound/boop_2.caf" ) );
	
	initClock();
}

void clockApp :: initClock ()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
	box2d.setIterations( 20, 10 );
	box2d.getWorld()->SetContactListener( &contactListener );
	
	contactListener.addReceiver( &clock );

	int w = ( deviceType == DEVICE_IPAD ) ? 1024 : 480;
	int h = ( deviceType == DEVICE_IPAD ) ? 768  : 320;
	
	float a1 = w * h;
	float a2 = 1280 * 720;
	float fs = a1 / (float)a2;
	
	clock.setBox2d( &box2d );
	clock.setSize( w, h );
	clock.setForceScale( fs );
//	clock.setTimeFont( &font );
	clock.setSound( &secTwoSound, &secOneSound );
	clock.setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void clockApp :: update()
{
	int hrs;
	int min;
	int sec;
	
	if( false )
	{
		int frameNum = ofGetFrameNum();
		
		int s = 00 + frameNum / frameRate;		// start time is add at front.
		int m = 48 + s / 60;					// start time is add at front.
		int h = 12 + m / 60;					// start time is add at front.
		
		hrs = h % 24;
		min = m % 60;
		sec = s % 60;
	}
	else
	{
		hrs = ofGetHours();
		min = ofGetMinutes();
		sec = ofGetSeconds();
	}
	
	//-- orientation.
	
	float ax = ofxAccelerometer.getForce().x;
	float ot = 0.6;								// orientation threshold.
	
	if( ax > ot )
	{
		if( ofxiPhoneGetOrientation() != OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT )
		{
			ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
		}
	}
	else if( ax < -ot )
	{
		if( ofxiPhoneGetOrientation() != OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT )
		{
			ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
		}
	}
	
	//-- gravity slant.
	
	float gx;
	gx = ofxAccelerometer.getForce().y;
	gx *= 2;									// increase the reaction to tilt.
	gx = MIN( 1.0, MAX( -1.0, gx ) );			// between -1 and 1.
	gx *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	clock.setGravity( gx, 0 );
	
	//-- update.
	
	clock.update( hrs, min, sec );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void clockApp :: draw()
{
	int bg = 0;
	ofBackground( bg, bg, bg );
	
	clock.draw();
	
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, ofGetHeight() - 15 );
}

void clockApp :: exit()
{

}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void clockApp::touchDown(ofTouchEventArgs &touch){

}

void clockApp::touchMoved(ofTouchEventArgs &touch){

}

void clockApp::touchUp(ofTouchEventArgs &touch){

}

void clockApp::touchDoubleTap( ofTouchEventArgs &touch )
{
	clock.toggleClockMode();
}

void clockApp::lostFocus(){

}

void clockApp::gotFocus(){

}

void clockApp::gotMemoryWarning(){

}

void clockApp::deviceOrientationChanged(int newOrientation)
{
	//
}
