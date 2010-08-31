/*
 *  ColorCycle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorCycle.h"

ColorCycle :: ColorCycle ()
{
	setScreenSize( ofGetWidth(), ofGetHeight() );
	setFrameRate( ofGetFrameRate() );
	
	white.r = 255;
	white.g = 255;
	white.b = 255;
	
	black.r = 0;
	black.g = 0;
	black.b = 0;
	
	sounds = NULL;
}

ColorCycle :: ~ColorCycle ()
{
	//
}

void ColorCycle :: setScreenSize ( int w, int h )
{
	screen.set( w, h );
}

void ColorCycle :: setFrameRate	( int fr )
{
	frameRate = fr;
}

void ColorCycle :: setGravity ( float gx, float gy )
{
#ifdef USE_PHYSICS
	physics.setGravity( gx, gy );
#endif
}

void ColorCycle :: setSounds ( ColorSound* sounds )
{
	this->sounds = sounds;
}

///////////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////////

void ColorCycle :: setup ()
{
	rect.init( screen.screenWidth, screen.screenHeight );
	
	colorPicker0.setColorRadius( 1.0 );
	colorPicker1.setColorRadius( 1.0 );
	
	colorPicker0.setColorAngle( 3 / 6.0 );
	colorPicker1.setColorAngle( 5 / 6.0 );
	
//	colorPicker0.disable();
//	colorPicker1.disable();
	
	colorPicker0.enable();
	colorPicker1.enable();

	panel.setScreen( screen );
	panel.setColorPickers( &colorPicker0, &colorPicker1 );
	
	colorScale = 1.0;
	
	rndColorAngle0.ease	= 0.05;
	rndColorAngle1.ease	= 0.05;
	
	bColorSelectMode			= false;
	bColorSelectModeOnBgClick	= false;
	bColorSelectTimeout			= false;
	colorSelectTimeout			= 0;
	
	bInputDown				= false;
	inputLastID				= -2;
	
	triColorScale.value		= 1.0;
	triColorScale.target	= 1.0;
	triColorScale.ease		= 0.2;
	
#ifdef USE_PHYSICS
	physics.setSounds( sounds );
	physics.setScreen( screen );
	physics.setup();
#endif
	
	if( sounds != NULL )
		sounds->playRandomBackgroundSound();
}

///////////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////////

void ColorCycle :: update ()
{
	if( bInputDown )
	{
		float velx = inputPos1.x - inputPos2.x;
		float vely = inputPos1.y - inputPos2.y;
		
		inputVel.x += ( velx - inputVel.x ) * 0.1;
		inputVel.y += ( vely - inputVel.y ) * 0.1;
	}
	else
	{
		float ease = 0.98;
		inputVel.x *= ease;
		inputVel.y *= ease;
	}

	float angDrag;
	angDrag = inputVel.y / 5000.0;
	
	float angRnd0 = rndColorAngle0.value;
	rndColorAngle0.update();
	angRnd0 = rndColorAngle0.value - angRnd0;

	float angRnd1 = rndColorAngle1.value;
	rndColorAngle1.update();
	angRnd1 = rndColorAngle1.value - angRnd1;
	
	float ang0 = 0;
	ang0 += angDrag;
	ang0 += angRnd0;
//	ang0 += frameRate * 0.00001;

	float ang1 = 0;
	ang1 += angDrag;
	ang1 += angRnd1;
//	ang1 += frameRate * 0.00001;
	
	colorPicker0.setColorAngle( colorPicker0.getColorAngle() + ang0 );
	colorPicker1.setColorAngle( colorPicker1.getColorAngle() + ang1 );
	
	float scl = inputVel.x / 3000.0;
	
	colorPicker0.setColorScale( colorPicker0.getColorScale() + scl );
	colorPicker0.setColorScale( MAX( MIN( colorPicker0.getColorScale(), 1.0 ), 0.5 ) );
	colorPicker1.setColorScale( colorPicker1.getColorScale() + scl );
	colorPicker1.setColorScale( MAX( MIN( colorPicker1.getColorScale(), 1.0 ), 0.5 ) );
	
	colorPicker0.update();
	colorPicker1.update();
	
	upperColor = colorPicker0.getColor();
	lowerColor = colorPicker1.getColor();
	
	rect.setCornerColor( upperColor, 0 );
	rect.setCornerColor( upperColor, 1 );
	rect.setCornerColor( lowerColor, 2 );
	rect.setCornerColor( lowerColor, 3 );
	
	panel.update();
	
	updatePhysics();
	updateDelaunay();
	updateTriangles();
	
	inputCheck();
}

void ColorCycle :: updatePhysics ()
{
#ifdef USE_PHYSICS
	
	physics.update();
	
	for( int i=0; i<physics.circles.size(); i++ )
	{
		ofColor c;
		c = interpolateColor( upperColor, lowerColor, physics.circles[ i ]->posColor.y );
		physics.circles[ i ]->setColor( c );
	}
	
#endif
}

void ColorCycle :: updateDelaunay ()
{
	delaunay.reset();
	
#ifdef USE_PHYSICS
	
	for( int i=0; i<physics.circles.size(); i++ )
	{
		ofPoint& pos = physics.circles[ i ]->pos;
		delaunay.addPoint( pos.x, pos.y, pos.z );
	}
	
#else
	
	for( int i=0; i<CIRCLES_MAX; i++ )
	{
		delaunay.addPoint( ofRandom( 0, ofGetScreenWidth() ), ofRandom( 0, ofGetScreenHeight() ), 0 );
	}
	
#endif
	
	delaunay.triangulate();
}

void ColorCycle :: updateTriangles ()
{
#ifdef USE_COLOR_CIRCLE	
	
	triColorScale.update();
	
	//--
	
	for( int i=0; i<triangles.size(); i++ )
	{
		delete triangles[ i ];
	}
	triangles.clear();
	
	//--
	
	for( int i=0; i<delaunay.triangles.size(); i++ )
	{
		const ofxDelaunayTriangle& delTri = delaunay.triangles[ i ];
		
		ColorTriangle* triangle;
		triangle = new ColorTriangle();
		
		for( int j=0; j<3; j++ )
		{
			//-- copy triangle points.
			
			triangle->points[ j ].x = delTri.points[ j ].x;
			triangle->points[ j ].y = delTri.points[ j ].y;
			
			triangle->indexs[ j ] = delTri.pointIndex[ j ];
			
			//-- collisions.
			
			ColorCircle* circle;
			circle = getCircleAtPoint( delTri.points[ j ] );
			float collisionScale = 0;
			if( circle != NULL )
			{
				collisionScale = circle->collision;
				collisionScale = ofClamp( collisionScale, 0.25, 0.75 );
			}
			
			//-- work out triangle colour.
			
			ofxVec2f p1, p2, p3;
			p1.set( delTri.points[ ( j + 0 ) % 3 ] );
			p2.set( delTri.points[ ( j + 1 ) % 3 ] );
			p3.set( delTri.points[ ( j + 2 ) % 3 ] );
			
			p2 -= p1;
			p2.normalize();
			
			p3 -= p1;
			p3.normalize();
			
			p2 += p3;
			p2.normalize();
			p2 *= triColorScale.value;
//			p2 *= collisionScale;
			
//			p1 += p2 * ( screen.screenMin * 0.25 );
			p1 += p2 * ( screen.screenMin * collisionScale );
			
			float px, py;
			px = p1.x / (float)screen.screenWidth;
			py = p1.y / (float)screen.screenHeight;
			
			px = MIN( MAX( px, 0.0 ), 1.0 );
			py = MIN( MAX( py, 0.0 ), 1.0 );
			
			ofColor c;
			c = interpolateColor( upperColor, lowerColor, py );
//			c = interpolateColor( c, black, collisionScale );
			
			triangle->colors[ j ] = c;
		}
		
		//--

		triangle->init();
		triangles.push_back( triangle );
	}
	
#endif
}

bool ColorCycle :: checkTriangleHit ( float x, float y, int id )
{
#ifdef USE_PHYSICS
	
	ofPoint p;
	p.set( x, y, 0 );
	
	for( int i=0; i<triangles.size(); i++ )
	{
		ColorTriangle* triangle = triangles[ i ];
		bool hit = triangle->isPointInsideTriangle( p );
		
		if( hit )
		{
			ofPoint p;
			p = triangle->getNearestTrianglePoint( x, y );

			ColorCircle* circle;
			for( int j=0; j<physics.circles.size(); j++ )
			{
				circle = physics.circles[ j ];
				
				if( circle->pos.x == p.x && circle->pos.y == p.y )
				{
					circle->bSelected = true;
					break;
				}
			}
			
			physics.box2d->grabShapeDown2( x, y, id, circle->body );
			
			return true;
		}
	}
	
#endif
	
	return false;
}

ofColor ColorCycle :: interpolateColor ( const ofColor& c1, const ofColor& c2, float p )
{
	ofColor c;
	c.r = ( c2.r - c1.r ) * p + c1.r;
	c.g = ( c2.g - c1.g ) * p + c1.g;
	c.b = ( c2.b - c1.b ) * p + c1.b;
	
	return c;
}

#ifdef USE_COLOR_CIRCLE
ColorCircle* ColorCycle :: getCircleAtPoint ( ofPoint p1 )
{
#ifdef USE_PHYSICS
	
	for( int i=0; i<physics.circles.size(); i++ )
	{
		ColorCircle* circle = physics.circles[ i ];
		const ofPoint& p2	= circle->pos;
		
		if( p1.x == p2.x && p1.y == p2.y )
		{
			return circle;
		}
	}
	
#endif
	
	return NULL;
}
#endif
///////////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////////

void ColorCycle :: draw ()
{
	rect.draw();
	
	drawTriangles();
	
#ifdef USE_PHYSICS	
	physics.draw();
#endif
	
	panel.draw();
}

void ColorCycle :: drawTriangles ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		triangles[ i ]->drawFill();
	}
	
	for( int i=0; i<triangles.size(); i++ )
	{
		triangles[ i ]->drawStroke();
	}
}

///////////////////////////////////////////////////////
//	JOINTS.
///////////////////////////////////////////////////////

void ColorCycle :: addCircle ()
{
	bool success = false;
	
#ifdef USE_PHYSICS
	success = physics.addSingleCircle();
#endif
	
	if( success )
	{
		if( sounds != NULL )
		{
			sounds->playRandomPointAddSound();
			sounds->startCollisionSoundTimeout();
		}
	}
}

void ColorCycle :: removeCircle ()
{
	bool success = false;
	
#ifdef USE_PHYSICS
	success = physics.removeCircle();
#endif
	
	if( success )
	{
		if( sounds != NULL )
		{
			sounds->playRandomPointRemoveSound();
			sounds->startCollisionSoundTimeout();
		}
	}
}

void ColorCycle :: shuffle ()
{
#ifdef USE_PHYSICS
	
	if( !bColorSelectMode )
	{
		physics.resetJoints();
	}
	
#endif
	
//	rndColorAngle0.target += ofRandom( -0.2, 0.2 );
//	rndColorAngle1.target += ofRandom( -0.2, 0.2 );
	
	rndColorAngle0.target += ofRandom( 0.05, 0.3 );
	rndColorAngle1.target -= ofRandom( 0.05, 0.3 );

	if( sounds != NULL )
	{
		sounds->playRandomPointShuffleSound();
		sounds->startCollisionSoundTimeout();
	}
}

void ColorCycle :: colorSelectMode ()
{
	bColorSelectMode = true;
	
	inputLastID	= -2;
	bInputDown	= true;
	
	inputPos1.x = inputPos2.x = 0;
	inputPos1.y = inputPos2.y = 0;
	
	panel.show();
	
#ifdef USE_PHYSICS
	physics.hideCircles();
#endif	
	
	triColorScale.target = 0.2;
}

///////////////////////////////////////////////////////
//	INPUT.
///////////////////////////////////////////////////////

void ColorCycle :: down ( int x, int y, int id )
{
	if( bColorSelectMode )
	{
		if( inputLastID != id )		// still in color select mode, but different id.
		{
			inputLastID = id;
			bInputDown	= true;
			
			inputPos1.x = inputPos2.x = x;
			inputPos1.y = inputPos2.y = y;
			
			bColorSelectTimeout = false;
		}
	}
	else
	{
		if( checkTriangleHit( x, y, id ) )		// touch inside triangles.
		{
			// do nothing.
		}
		else									// touch on background.
		{
			if( bColorSelectModeOnBgClick )
			{
				bColorSelectMode = true;
				
				inputLastID	= id;
				bInputDown	= true;
				
				inputPos1.x = inputPos2.x = x;
				inputPos1.y = inputPos2.y = y;
				
				panel.show();
				
#ifdef USE_PHYSICS				
				physics.hideCircles();
#endif
				
				triColorScale.target = 0.2;
			}
		}
	}
	
	colorPicker0.touchDown( x, y, id );
	colorPicker1.touchDown( x, y, id );
	
//	physics.checkHit( x, y )	// don't forget this, could be handy later.
}

void ColorCycle :: drag ( int x, int y, int id )
{
	if( bColorSelectMode )
	{
		if( inputLastID == id )
		{
			inputPos2.x = inputPos1.x;	// copy old values.
			inputPos2.y = inputPos1.y;
	
			inputPos1.x = x;			// save new values.
			inputPos1.y = y;
		}
	}
	else
	{
#ifdef USE_PHYSICS
		physics.box2d->grabShapeDragged2( x, y, id );
#endif
	}
	
	colorPicker0.touchMoved( x, y, id );
	colorPicker1.touchMoved( x, y, id );
}

void ColorCycle :: up ( int x, int y, int id )
{
	if( bColorSelectMode )
	{
		if( inputLastID == id )
		{
			inputLastID	= -2;
			bInputDown	= false;
		}
		
		colorSelectTimeout	= ofGetElapsedTimeMillis() + 1000;
		bColorSelectTimeout	= true;
	}
	
	colorPicker0.touchUp( x, y, id );
	colorPicker1.touchUp( x, y, id );
	
#ifdef USE_PHYSICS
	physics.box2d->grabShapeUp2( x, y, id );
#endif
}

void ColorCycle :: inputCheck ()
{
	if( bColorSelectTimeout )
	{
		if( ofGetElapsedTimeMillis() > colorSelectTimeout )
		{
			bColorSelectTimeout	= false;
			bColorSelectMode	= false;
			
			panel.hide();
			
#ifdef USE_PHYSICS
			physics.showCircles();
#endif			
			
			triColorScale.target = 1.0;
		}
	}
}
