/*
 *  EventPanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 22/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "EventDataItem.h"

class EventPanel : public ofxSprite
{

public:
	
	EventPanel()
	{
		thumbImage = NULL;
	}
	
	~EventPanel() {};
	
	//==================================================

	EventDataItem	data;
	
	ofImage*	thumbImage;
	
	ofImage*	bg;
	ofImage*	iconRain;
	ofImage*	iconTemp;
	ofImage*	play;
	ofImage*	shadow;
	
	ofPoint		bgPoint;
	ofPoint		iconPoint;
	ofPoint		playPoint;
	ofPoint		shadowPoint;
	ofRectangle	whiteBox;
	ofPoint		titlePoint;
	
	ofTrueTypeFont	title;
	
	//==================================================
	
	void populate ( const EventDataItem& data )
	{
		this->data = data;
	}
	
	void setup ()
	{
//		x = data.lon;
//		y = data.lat;

		x = (int)ofRandom( ofGetWidth()  * 0.3, ofGetWidth()  * 0.7 );
		y = (int)ofRandom( ofGetHeight() * 0.1, ofGetHeight() * 0.5 );
		
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_bg" );
		iconRain	= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_icon_rain" );
		iconTemp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_icon_temp" );
		play		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_play" );
		shadow		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_shadow" );
		
		thumbImage	= (ofImage*)ofxAssets :: getInstance()->getAssetByFileName( data.thumb );
		
		bgPoint.set( -119, -216, 0 );
		iconPoint.set( -81, -50, 0 );
		playPoint.set( -67, -180, 0 );
		shadowPoint.set( -81, -3, 0 );
		titlePoint.set( -45, -46, 0 );
		
		whiteBox.x		= -80;
		whiteBox.y		= -175;
		whiteBox.width	= 155;
		whiteBox.height	= 112;
		
		title.loadFont( "assets/LIBRARY/fonts/Rockwell.ttf", 9 );
	}
	
	void update ()
	{
		//
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		ofEnableAlphaBlending();
		
		bg->draw( x + bgPoint.x, y + bgPoint.y );
		iconRain->draw( x + iconPoint.x, y + iconPoint.y );
		shadow->draw( x + shadowPoint.x, y + shadowPoint.y );
		
		ofFill();
		ofSetColor( 0xFFFFFF );
		ofRect( x + whiteBox.x, y + whiteBox.y, whiteBox.width, whiteBox.height );
		
		if( thumbImage != NULL )
		{
			thumbImage->draw( x + whiteBox.x, y + whiteBox.y );
		}
		
		play->draw( x + playPoint.x, y + playPoint.y );

		title.drawString( data.title, x + titlePoint.x, y + titlePoint.y );
		
		ofFill();
		ofSetColor( 0xFF0000 );
		ofRect( x - 1, y - 1, 3, 3 );
		
		ofDisableAlphaBlending();
	}
	
};