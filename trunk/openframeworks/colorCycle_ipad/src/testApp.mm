#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
	
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	
	lastTouch.id = -1;
	
	cc.setScreenSize( ofGetWidth(), ofGetHeight() );
	cc.setup();
	
	footer = NULL;
	footer = new FooterBar();
	if( footer != NULL )
		footer->setup();
	
	//-- splash screen fade out.
	
	splashScreen = new ofImage();
	splashScreen->loadImage( "Default.png" );
	drawSplashScreen();
	
	//--
	
	infoScreen.setup();
}

//--------------------------------------------------------------
void testApp::update()
{
	if( bTouchDown )
		++touchCount;
	
	if( footer->isShuffleSelected() )
		cc.shuffle();
	
	if( footer->isColorSelected() )
		cc.down( 0, 0, 0 );
	
	if( footer->isAddSelected() )
		cc.addCircle();
	
	if( footer->isRemoveSelected() )
		cc.removeCircle();
	
	if( footer->isInfoSelected() )
		infoScreen.show();
	
	float gx;
	gx = ofxAccelerometer.getForce().y;
	gx *= 2;									// increase the reaction to tilt.
	gx = MIN( 1.0, MAX( -1.0, gx ) );			// between -1 and 1.
	gx *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	float gy;
	gy = ofxAccelerometer.getForce().x;
	gy *= 2;									// increase the reaction to tilt.
	gy = MIN( 1.0, MAX( -1.0, gy ) );			// between -1 and 1.
	gy *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	cc.setGravity( gx, gy );
	
	cc.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	cc.draw();
	
	drawSplashScreen();
	
	infoScreen.draw();
	
	ofSetColor( 0, 0, 0 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(),  0 ), ofGetScreenWidth() - 30, 20 );
}

void testApp :: drawSplashScreen ()
{
	if( splashScreen == NULL )
		return;
	
	int t = 30;
	
	if( ofGetFrameNum() > t )
	{
		splashScreen->clear();
		delete splashScreen;
		splashScreen = NULL;
		return;
	}
	
	float a = 1 - Quad :: easeOut( ofGetFrameNum(), 0, 1.0, t );
	
	int cx = (int)( ofGetScreenWidth()  * 0.5 );
	int cy = (int)( ofGetScreenHeight() * 0.5 );

	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, a * 255 );
	
	glPushMatrix();
	glTranslatef( cx, cy, 0 );
	glRotatef( 90, 0, 0, 1 );
	glTranslatef( -cy, -cx, 0 );
	splashScreen->draw( 0, 0 );
	glPopMatrix();
	
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch)
{
	infoScreen.hide();
	
	if( lastTouch.id != touch.id )
	{
		lastTouch.id	= touch.id;
		lastTouch.x		= touch.x;
		lastTouch.y		= touch.y;
		
		bTouchDown		= true;
		touchCount		= 0;
	}
	
	cc.down( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch)
{
	cc.drag( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch)
{
	if( lastTouch.id == touch.id )
	{
		lastTouch.id = -1;
		
		cout << "touchCount " << touchCount << endl;
		
		if( touchCount > 10 )
		{
			float d = ofDist( lastTouch.x, lastTouch.y, touch.x, touch.y );
			
			if( d < 20 )
			{
				if( footer != NULL )
					footer->toggleShow();
			}
		}
	}
	
	cc.up( touch.x, touch.y, touch.id );
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch)
{
	if( footer != NULL )
		footer->toggleShow();
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}
