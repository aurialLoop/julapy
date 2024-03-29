/*
 *  ofxiPhoneApp.h
 *  MSA Labs Vol. 1
 *
 *  Created by Mehmet Akten on 14/07/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
//#include "ofEvents.h"
#include "ofxiPhoneAlerts.h"
//#include "ofxMultiTouch.h"


//class ofxiPhoneApp : public ofSimpleApp, public ofxiPhoneAlertsListener, public ofxMultiTouchListener {
class ofxiPhoneApp : public ofSimpleApp, public ofxiPhoneAlertsListener {
	
public:
	void setup() {};
	void update() {};
	void draw() {};
	void exit() {};

	virtual void touchDown(int x, int y, int id) {};
	virtual void touchMoved(int x, int y, int id) {};
	virtual void touchUp(int x, int y, int id) {};
	virtual void touchDoubleTap(int x, int y, int id) {};
	
	void lostFocus() {}
	void gotFocus() {}
	void gotMemoryWarning() {}

//	virtual void touchDown(ofTouchEventArgs & touch) {
//		touchDown(touch.x, touch.y, touch.id);
//	};
//	virtual void touchMoved(ofTouchEventArgs & touch) {
//		touchMoved(touch.x, touch.y, touch.id);
//	};
//	virtual void touchUp(ofTouchEventArgs & touch) {
//		touchUp(touch.x, touch.y, touch.id);
//	};
//	virtual void touchDoubleTap(ofTouchEventArgs & touch) {
//		touchDoubleTap(touch.x, touch.y, touch.id);
//	};

};

