/*
 *  NawlzFishBowl.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#include "MSACore.h"
#include "MSAFluid.h"

#include "ofBaseApp.h"
#include "ofTexture.h"

#include "NawlzParticle.h"
#include "NawlzParticle01.h"

using namespace MSA;

//////////////////////////////////////////////
//	.
//////////////////////////////////////////////

class NawlzFishBowl : public ofBaseApp
{
	
public:
	
	 NawlzFishBowl();
	~NawlzFishBowl();
	
	void setup	();
	void update	();
	void draw	();
	
	void setROI						( const ofRectangle& roi );
	void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createParticleTexture		( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createBowlTexture			( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	bool	bDrawBackground;
	bool	bDrawParticles;
	bool	bDrawBowl;
	
	bool	fluidEnableRGB;
	float	fluidFadeSpeed;
	float	fluidDeltaT;
	float	fluidVisc;
	float	fluidColorDiffusion;
	int		fluidSolverIterations;
	bool	fluidEnableVorticityConfinement;
	bool	fluidWrapX;
	bool	fluidWrapY;
	bool	fluidInputVelocityMult;
	
private:
	
	void initFluid	();
	
	void createFluidTexture			();
	void createParticles			();
	
	void updateParticles			();
	bool isPointInsideROI			( float x, float y );
	bool isPointInsideBowl			( float x, float y );
	
	void drawParticles	();
	
	void addToFluid		( Vec2f pos, Vec2f vel, bool addColor, bool addForce );
	void drawVectors	( float x, float y, float renderWidth, float renderHeight );
	void drawFluid		();
	
	int			mouseX;
	int			mouseY;
	
	ofRectangle		roi;
	
	ofTexture*		backgroundTexture;
	ofTexture*		particleTexture;
	ofTexture*		bowlTexture;
	unsigned char*	bowlPixels;
	ofTexture*		fluidTexture;
	unsigned char*	fluidPixels;
	
	ofPoint			backgroundTextureXY;
	ofPoint			bowlTextureXY;
	
	FluidSolver		fluidSolver;
	int				fluidCellsX;
	bool			bResizeFluid;
	Vec2f			pMouse;
	
	vector<NawlzParticle01>	particles;
	
	bool			bOverRoi;
};