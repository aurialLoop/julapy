/*
 *  Clock.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Clock.h"

Clock :: Clock ()
{
	box2d	= NULL;
	font	= NULL;
	
	clockMode = CLOCK_MODE_1;
	
	hrsOneTotal = 2;
	hrsTwoTotal = 9;
	minOneTotal = 5;
	minTwoTotal = 9;
	secOneTotal = 5;
	secTwoTotal = 9;
	
	hrsOneX = hrsOneM1X	= 150;
	hrsTwoX = hrsTwoM1X	= 220;
	minOneX = minOneM1X	= 590;
	minTwoX = minTwoM1X	= 660;
	secOneX = secOneM1X	= 1000;
	secTwoX = secTwoM1X	= 1070;
	
	hrsOneM2X = 100;
	hrsTwoM2X = 270;
	minOneM2X = 540;
	minTwoM2X = 710;
	secOneM2X = 950;
	secTwoM2X = 1120;
	
	setSize( ofGetWidth(), ofGetHeight() );
}

Clock :: ~Clock()
{
	//
}

///////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////

void Clock :: setup ()
{
	if( box2d == NULL )
		return;
	
	createCircles();
}

///////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////

void Clock :: setBox2d ( ofxBox2d *box2d )
{
	this->box2d = box2d;
}

void Clock :: setSize ( ofRectangle &size )
{
	setSize( size.width, size.height );
}

void Clock :: setSize ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
	screenMinLength	= MIN( w, h );
	screenMaxLength = MAX( w, h );
	screenTotal		= w * h;
}

void Clock :: setTimeFont ( ofTrueTypeFont *font )
{
	this->font = font;
}

void Clock :: createCircles ()
{
	float area	= 0.015;		// biggest area as start.
	float dec	= 0.5;		// decrease in area for the next batch of circles.
	
	createCircle( hrsOne, hrsOneTotal, areaToRadius( area ),		0x6b007e, 100 );
	createCircle( hrsTwo, hrsTwoTotal, areaToRadius( area *= dec ),	0xf51d2a, 270 );
	createCircle( minOne, minOneTotal, areaToRadius( area *= dec ),	0xf6009d, 540 );
	createCircle( minTwo, minTwoTotal, areaToRadius( area *= dec ),	0x8c162f, 710 );
	createCircle( secOne, secOneTotal, areaToRadius( area *= dec ),	0xc96dfd, 950 );
	createCircle( secTwo, secTwoTotal, areaToRadius( area *= dec ),	0xf7719a, 1120 );
}

void Clock  :: createCircle ( vector<ClockCircle*> &circlesVec, int numOfCircle, float radius, int color, int lx )
{
	for( int i=0; i<numOfCircle; i++ )
	{
		ClockCircle* circle;
		circle = new ClockCircle( radius, color );
		
		//-- define circles physics.
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		
		circle->setPhysics( mass, bounce, friction );
		circle->setup( box2d->getWorld(), ofRandom( 0, screenWidth ), ofRandom( 0, screenHeight ), radius, false );
		circle->setRotationFriction( 1.0 );
		circle->setDamping( 1.0 );
//		circle->enableGravity( false );
		
		//-- define line up point.
		
		int lineX;
		lineX = lx;
		lineX += ( i > 4 ) ? radius * 2.2 : 0;

		int j = i % 5;
		
		int lineY;
		lineY = screenHeight;
		lineY -= 100;
		lineY -= radius;
		lineY -= j * radius * 2;
		lineY -= j * ( radius * 0.2 );
		
		circle->lineUpPoint.set( lineX, lineY );
		
		//-- add to vectors.
		
		circlesVec.push_back( circle );
		circlesAll.push_back( circle );
	}
}

float Clock :: areaToRadius ( float area )
{
	float r;
	r = sqrt( area * screenTotal );
	r *= 0.5;
	
	return r;
}

///////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////

void Clock :: update ( int hrs, int min, int sec )
{
	hrsOneCount = hrs / 10;
	hrsTwoCount = hrs % 10;
	
	minOneCount = min / 10;
	minTwoCount = min % 10;
	
	secOneCount = sec / 10;
	secTwoCount = sec % 10;

	//--
	
	updateTimeX();
	
	if( clockMode == CLOCK_MODE_1 )
	{
		updateForcesM1();
		updateRayBlob();
	}
	
	if( clockMode == CLOCK_MODE_2 )
	{
		updateForcesM2();		
	}
	
	box2d->update();
}

void Clock :: updateTimeX ()
{
	float ease = 0.3;
	
	if( clockMode == CLOCK_MODE_1 )
	{
		hrsOneX += ( hrsOneM1X - hrsOneX ) * ease;
		hrsTwoX += ( hrsTwoM1X - hrsTwoX ) * ease;
		minOneX += ( minOneM1X - minOneX ) * ease;
		minTwoX += ( minTwoM1X - minTwoX ) * ease;
		secOneX += ( secOneM1X - secOneX ) * ease;
		secTwoX += ( secTwoM1X - secTwoX ) * ease;
	}
	else if ( clockMode == CLOCK_MODE_2 )
	{
		hrsOneX += ( hrsOneM2X - hrsOneX ) * ease;
		hrsTwoX += ( hrsTwoM2X - hrsTwoX ) * ease;
		minOneX += ( minOneM2X - minOneX ) * ease;
		minTwoX += ( minTwoM2X - minTwoX ) * ease;
		secOneX += ( secOneM2X - secOneX ) * ease;
		secTwoX += ( secTwoM2X - secTwoX ) * ease;
	}
}

///////////////////////////////////////////////
//	PREPARE CIRCLES.
///////////////////////////////////////////////

void Clock :: setForM1 ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		
//		circle.enableGravity( false );
	}
}

void Clock :: setForM2 ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		ClockCircle& circle = *circlesAll[ i ];
		
//		circle.enableGravity( true );
	}
}

///////////////////////////////////////////////
//	CLOCK MODE 1.
///////////////////////////////////////////////

void Clock :: updateForcesM1 ()
{
	updateForcesM1b( hrsOne, hrsOneCount );
	updateForcesM1b( hrsTwo, hrsTwoCount );
	updateForcesM1b( minOne, minOneCount );
	updateForcesM1b( minTwo, minTwoCount );
	updateForcesM1b( secOne, secOneCount );
	updateForcesM1b( secTwo, secTwoCount );
}

void Clock :: updateForcesM1b ( vector<ClockCircle*> &circlesVec, int count )
{
	for( int i=0; i<circlesVec.size(); i++ )
	{
		if( i < count )
		{
			pullToCenter( *circlesVec[ i ] );
		}
		else 
		{
			pushFromCenter( *circlesVec[ i ] );
		}
	}
}

void Clock :: pullToCenter ( ClockCircle& circle )
{
	ofxVec2f c;
	ofxVec2f p;
	ofxVec2f v;
	
	c.set( ofGetWidth() * 0.5, ofGetHeight() * 0.5 );
	p.set( circle.getPosition() );
	v = c - p;
	
	float d;
	d = v.length() / (float)screenMaxLength;
	
	float s;
	s = circle.getRadius() / 20.0;			// the bigger the circle, the stronger the pull towards center.
	
	ofxVec2f perp;							// spinning force.
	perp = v.getPerpendicular();
	perp *= circle.spinFrc;
	
	v.normalize();
	v *= d;
	v *= s;
	v *= 30;
	v += perp;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

void Clock :: pushFromCenter ( ClockCircle& circle )
{
	ofxVec2f c;
	ofxVec2f p;
	ofxVec2f v;
	
	c.set( ofGetWidth() * 0.5, ofGetHeight() * 0.5 );
	p.set( circle.getPosition() );
	v = c - p;
	
	float d;
	d = 1 - ( v.length() / ( screenMinLength * 0.4 ) );

	ofxVec2f perp;							// spinning force.
	perp = v.getPerpendicular();
	perp *= circle.spinFrc;
	perp *= circle.spinDir;
	
	v.normalize();
	v *= d;
	v *= -1;
	v *= 15;
	v += perp;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

///////////////////////////////////////////////
//	CLOCK MODE 2.
///////////////////////////////////////////////

void Clock :: updateForcesM2 ()
{
	updateForcesM2b( hrsOne, hrsOneCount );
	updateForcesM2b( hrsTwo, hrsTwoCount );
	updateForcesM2b( minOne, minOneCount );
	updateForcesM2b( minTwo, minTwoCount );
	updateForcesM2b( secOne, secOneCount );
	updateForcesM2b( secTwo, secTwoCount );
}

void Clock :: updateForcesM2b ( vector<ClockCircle*> &circlesVec, int count )
{
	for( int i=0; i<circlesVec.size(); i++ )
	{
		if( i < count )
		{
			lineUp( *circlesVec[ i ] );
		}
		else 
		{
			floatUp( *circlesVec[ i ] );
		}
	}
}

void Clock :: floatUp ( ClockCircle& circle )
{
	b2Vec2 up = b2Vec2( 0, -10 );
	
	circle.body->ApplyImpulse( up, circle.body->GetWorldCenter() );
}

void Clock :: lineUp ( ClockCircle& circle )
{
	ofxVec2f p1;
	ofxVec2f p2;
	ofxVec2f v;
	
	p1.set( circle.lineUpPoint );
	p2.set( circle.getPosition() );
	v = p1 - p2;
	
	float d;
	d = v.length() / (float)screenMaxLength;
	
	v.normalize();
	v *= d;
	v *= 30;
	
	circle.body->ApplyImpulse( b2Vec2( v.x, v.y ), circle.body->GetWorldCenter() );
}

//-- RAY BLOB.

void Clock :: updateRayBlob ()
{
	int h = RAY_BLOB_HI_RES;	// high res.
	int l = RAY_BLOB_LO_RES;	// low res - down sample.
	int s = h / l;				// sample.
	
	bool bVerbose = false;
	
	float blobPad	= 50;
	float blobEase	= 0.4;
	float blobDist	= screenMinLength * 0.3;
	
	ofxVec2f p1;
	ofxVec2f p2;
	ofxVec2f hitPoint;
	
	p2.set( screenWidth * 0.5, screenHeight * 0.5 );
	
	for( int i=0; i<h; i+=s )
	{
		if( bVerbose && ofGetFrameNum() == 0 )
			cout << "new loop" << endl;
		
		int count			= 0;
		float length		= 0;
		float lengthMax		= 0;
		float lengthTotal	= 0;
		
		for( int j=0; j<s; j++ )
		{
			//-------						// works out the index.
			
			int k = i + j;
			int m = k - (int)( s * 0.5 );
			
			if( m < 0 )
				m += h;
			
			if( m > h - 1 )
				break;
			
			//-------						// prints index for debugging.
			
			if( bVerbose && ofGetFrameNum() == 0 )
				cout << m << endl;
			
			//-------						// works out average length of samples.
			
			float p;
			p = m / (float)h;
			
			p1.set( 0, -1 );
			p1.rotate( p * TWO_PI * RAD_TO_DEG );
			p1 *= blobDist;
			p1 += p2;
			
			vector<ofPoint> hitPoints;
			box2d->raycast( p1, p2, 10, &hitPoints );
			
			if( hitPoints.size() > 0 )
			{
				hitPoint.set( hitPoints[ 0 ] );
				hitPoint -= p2;
				hitPoint += hitPoint.getNormalized() * blobPad;
			}
			else
			{
				hitPoint.set( 0, 0 );
			}

			length		= hitPoint.length();
			lengthMax	= ( length > lengthMax ) ? length : lengthMax;
			lengthTotal	+= length;
			count		+= 1;
		}
		
		//-------							// calc average.
		
		float lengthAvg;
		lengthAvg = length / (float)count;

		float p;
		p = i / (float)h;
		
		p1.set( 0, -1 );
		p1.rotate( p * TWO_PI * RAD_TO_DEG );
//		p1 *= lengthAvg;
		p1 *= lengthMax;
		p1 += p2;
		
		int n = i / s;
		ofPoint& rayPoint = rayBlob[ n ];
		
		rayPoint.x += ( p1.x - rayPoint.x ) * blobEase;
		rayPoint.y += ( p1.y - rayPoint.y ) * blobEase;
	}
}

//-- CLOCK MODE.

void Clock :: toggleClockMode ()
{
	if( clockMode == CLOCK_MODE_1 )
	{
		clockMode = CLOCK_MODE_2;
	}
	else if( clockMode == CLOCK_MODE_2 )
	{
		clockMode = CLOCK_MODE_1;
	}
}

///////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////

void Clock :: draw ()
{
	box2d->draw();

	if( clockMode == CLOCK_MODE_1 )
	{
		drawRayBlob();
//		drawRayCasts();
	}
	
	drawCircles();
	drawTime();
}

void Clock :: drawCircles ()
{
	for( int i=0; i<circlesAll.size(); i++ )
	{
		drawCircle( *circlesAll[ i ] );
	}
}

void Clock :: drawCircle ( ClockCircle &circle )
{
	ofFill();
	ofSetColor( circle.colorHex );
	
	circle.draw();
	
	ofEnableSmoothing();
	
	ofNoFill();
	ofSetColor( circle.colorHex );
	
	circle.draw();
	
	ofDisableSmoothing();
}

void Clock :: drawTime ()
{
	if( font == NULL )
		return;
	
	int blackH = 80;
	ofFill();
	ofEnableAlphaBlending();
	ofSetColor( 0, 0, 0, 120 );
	ofRect( 0, screenHeight - blackH, screenWidth, blackH );
	ofDisableAlphaBlending();
	
	ofSetColor( 0xFFFFFF );
	
	int fontY = screenHeight - 30;
	font->drawString( ofToString( hrsOneCount, 0 ), hrsOneX, fontY );
	font->drawString( ofToString( hrsTwoCount, 0 ), hrsTwoX, fontY );
	font->drawString( ofToString( minOneCount, 0 ), minOneX, fontY );
	font->drawString( ofToString( minTwoCount, 0 ), minTwoX, fontY );
	font->drawString( ofToString( secOneCount, 0 ), secOneX, fontY );
	font->drawString( ofToString( secTwoCount, 0 ), secTwoX, fontY );
}

void Clock :: drawRayCasts ()
{
	int t = RAY_BLOB_LO_RES;
	
	ofEnableAlphaBlending();

	for( int i=0; i<t; i++ )
	{
		float p;
		p = i / (float)t;
		
		if( p > 1.0 )	// just to test its being drawn correctly.
			return;
		
		ofxVec2f v;
		v.set( 0, -1 );
		v.rotate( p * TWO_PI * RAD_TO_DEG );
		v *= screenMinLength * 0.4;
		
		ofxVec2f cp;
		cp.set( screenWidth * 0.5, screenHeight * 0.5 );

		ofNoFill();
		ofSetColor( 255, 0, 0, 100 );
		ofLine( cp.x, cp.y, cp.x + v.x, cp.y + v.y );
		
		ofPoint& rayPoint = rayBlob[ i ];
		
		ofFill();
		ofSetColor( 0, 255, 0, 100 );
		ofCircle( rayPoint.x, rayPoint.y, 4 );
	}
	
	ofDisableAlphaBlending();
}

void Clock :: drawRayBlob ()
{
	int t = RAY_BLOB_LO_RES;

	ofNoFill();
	ofSetColor( 255, 255, 255, 100 );
	ofSetLineWidth( 2 );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	ofBeginShape();
	
	for( int i=0; i<t+3; i++ )
	{
		int j = i % t;
		
		ofPoint& rayPoint = rayBlob[ j ];
		
		ofCurveVertex( rayPoint.x, rayPoint.y );
	}
	
	ofEndShape( true );
	
	ofSetLineWidth( 1 );
	ofDisableSmoothing();
	ofDisableAlphaBlending();
}