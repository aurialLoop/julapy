#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
//	frameRate = 25;
	frameRate = 60;
	
	ofSetVerticalSync( true );
	ofSetFrameRate( frameRate );
	
	screenGrab.setup( "movies/" );
//	screenGrab.setPause( false );
	
	bDebug		= false;
	bFullScreen	= false;
	bUseCamera	= true;
	
	initColors();
	initCamera();
	initLogo();
	initCv();
	initContours();
	initBox2d();
	initOpticalField();
}

void testApp :: initColors ()
{
	int colorSize;
	
//	int colorArray[] =			// image 1
//	{
//		0xFEFEFD,0xF9F634,0xEBDD45,0xCFDB9C,0xF8DECE,0xEB98AF,0xF7F5C8,0xD40D24,0xB1A09B,0xE54B85,
//		0xD4D2CF,0x433035,0x8FC9EE,0x9C9474,0xB72220,0x9EC18F,0x92A52D,0xCB9A2F,0x9D6C75,0x95C930,
//		0x79A6B4,0x8EB9D2,0xD2BD42,0xCD4474,0x4E8C3E,0xE3E8EB,0xBB5F27,0x9A314D,0xCEBBB5,0xF9D2DC,
//		0xC9CB5C,0x6A5571
//	};
//	colorSize = 32;
	
//	int colorArray[] =			// image 2
//	{
//		0xE9589F,0x15105E,0x571661,0x981E63,0x16238E,0x5F2A8E,0xA749A5,0x972E8D,0x3DA1F9,0x1A5EE4,
//		0x56052B,0x6148AE,0x6362DF,0xB29DFA,0x9D62D9,0x100020,0xE46ACF,0x1C47B2,0x8032F0,0xD9467F,
//		0xC32E6A,0x8A1036,0x2A0535,0x132DC7,0x30000B,0xC53893,0xB03C7A,0x5A35C8,0x963CC8,0xC2253F,
//		0x653879,0x193778
//	};
//	colorSize = 32;
	
	int colorArray[] =			// image 3
	{
		0xC5490E,0xE8EAD6,0xDFA161,0xAED0B3,0xD8DCB5,0x96B69C,0xA89B63,0xE4B78B,0xE48E19,0xF6D9B4,
		0x6A9F92,0x645A23,0x6F8D5F,0xF0CF66,0xD8CF99,0xBFDDC9,0xF2D294,0x180B07,0x284725,0x6A1A06,
		0xFDF3C4,0x67714A,0x145351,0xF5D013,0xA9822E,0x1E7483,0xF2A4F,0xD2EFC2,0xFAF08B,0xE2756B,
		0xB9C174,0xB5C61C
	};
	colorSize = 32;				
	
//	int colorArray[] =			// image 4
//	{
//		0xFFD44C,0xDC6517,0xFFB73A,0x5C2908,0xFF941F,0xFFAB25,0x50100,0xFFF3A9,0x270B03,0xF88022,
//		0x873F0C,0x3F1905,0xF48D3F,0xFF9D36,0xFFD30F,0xC48613,0xE4AA1A,0xF1AC41,0xFF9100,0xEB8300,
//		0xC98E3D,0xFFBE5E,0xFFAF00,0xE3B76A,0x422613,0xD7AB00,0x835E2A,0xE49864,0xFFA164,0x2A1011,
//		0xCEC500,0xBEF950
//	};
//	colorSize = 32;

//	int colorArray[] =			// image 7
//	{
//		0xF4FAF2,0xFD7E5,0xC6597,0xCE2C43,0x6B5D3,0x242855,0x5ADEE6,0x290B6,0xA2E6EB,0xEFE7AA,
//		0xD04F90,0x54ABDF,0xE89A5E,0xF1E75B,0xF1EC19,0xEF9E98,0xF29318,0xA9D9A2,0x97A9E2,0xA59BA8,
//		0x5BABAE,0xB1F467,0xA99B66,0x69E7AD,0x6256A0,0x6D9B55,0x96EC2,0x1F3A89,0xE7C0CA,0xAA8A2F,
//		0x597FD3,0xB2D62F
//	};
//	colorSize = 32;
	
//	int colorArray[] =			// custom.
//	{
//		0xFF00FF,0x00FFFF,0xFFFF00
//	};
//	colorSize = 3;

//	int colorArray[] =			// custom.
//	{
//		0xFF0000,0x0000FF,0xFF00FF
//	};
//	colorSize = 3;
	
	colors.resize( colorSize );
	copy( colorArray, colorArray + colorSize, colors.begin() );
}

void testApp :: initCamera ()
{
	cameraRect.width	= 640;
	cameraRect.height	= 480;
	
	cameraRect.width	*= 0.5;
	cameraRect.height	*= 0.5;
	
	if( bUseCamera )
	{
		camera.setVerbose( true );
		camera.initGrabber( cameraRect.width, cameraRect.height );
	}
	else
	{
		video.loadMovie( "input_video_320x240.mov" );
		
		if( !screenGrab.isRecording() )
			video.play();
	}

}

void testApp :: initLogo ()
{
	logo.loadImage( "holler_logo_med.png" );
	logo.setImageType( OF_IMAGE_GRAYSCALE );
	
	logoPixles = logo.getPixels();
	
	int logoPixelsTotal;
	logoPixelsTotal = logo.width * logo.height;
	
	float pixelThreshold;
	pixelThreshold = 0.5;
	
	logoBWPixles = new unsigned char[ logoPixelsTotal ];
	
	for( int i=0; i<logoPixelsTotal; i++ )		// clean up image.
	{
		if( logoPixles[ i ] > (int)( pixelThreshold * 255 ) )
		{
			logoBWPixles[ i ] = 255;
		}
		else 
		{
			logoBWPixles[ i ] = 0;
		}
	}
	
	logoBW.allocate( logo.width, logo.height );
	logoBW.setFromPixels( logoBWPixles, logo.width, logo.height );
	
	logo.loadImage( "holler_logo_med_alpha.png" );
	
	logoCropRect.width	= (int)( logo.width * ( cameraRect.height / (float)cameraRect.width ) );
	logoCropRect.height	= (int)( logo.height );
	logoCropRect.x		= (int)( ( ofGetWidth()  - logoCropRect.width  ) * 0.5 );
	logoCropRect.y		= (int)( ( ofGetHeight() - logoCropRect.height ) * 0.5 );
	
	ofxCvGrayscaleImage logoCopy;
	logoCopy.allocate( logoBW.width, logoBW.height );
	logoCopy = logoBW;
	logoCopy.setROI( logoCropRect );
	
	ofxCvGrayscaleImage logoCrop;
	logoCrop.allocate( logoCropRect.width, logoCropRect.height );
	logoCrop.setFromPixels( logoCopy.getRoiPixels(), logoCropRect.width, logoCropRect.height );
	
	logoSmall.allocate( cameraRect.width, cameraRect.height );
	logoSmall.scaleIntoMe( logoCrop );
	
	logoSmallIntersect.allocate( cameraRect.width, cameraRect.height );
	
	logoCopy.clear();
	logoCrop.clear();
}

void testApp :: initCv ()
{
	cameraColorImage.allocate( cameraRect.width, cameraRect.height );
	cameraGrayImage.allocate( cameraRect.width, cameraRect.height );
	cameraGrayPrevImage.allocate( cameraRect.width, cameraRect.height );
	cameraGrayDiffImage.allocate( cameraRect.width, cameraRect.height );
}

void testApp :: initContours ()
{
	//
}

void testApp :: initBox2d ()
{
	box2d.init();
//	box2d.setGravity( 0, 40 );
//	box2d.setFPS( 40 );
	box2d.setGravity( 0, 10 );
	box2d.setFPS( 30 );
	
	int thick;
	thick = 1;
	
	float wallMass		= 1.0;
	float wallBounce	= 0.5;
	float wallFriction	= 0.3;
	
	float fw;
	fw = 0.5;
	
	int sw, sh;
	sw = ofGetWidth() * 0.5;
	sh = ofGetHeight();
	
	int t = 10;
	
	floor.setPhysics( 0, 0, 0 );			// fixed.
	floor.addVertex( -sw * 0.5, t );
	floor.addVertex(  0, -t );
	floor.addVertex(  sw * 0.5, t );
	floor.createShape( box2d.getWorld(), ofGetWidth() * 0.5, ofGetHeight() + t );
	
//	ofxBox2dRect floor;
//	floor.setPhysics( wallMass, wallBounce, wallFriction );
//	floor.setup
//	(
//		box2d.getWorld(),
//		ofGetWidth() * ( 1.0 - fw ) * 0.5,
//		ofGetHeight(),
//		ofGetWidth() * fw,
//		thick,
//		true
//	);
	
//	ceil.setPhysics( wallMass, wallBounce, wallFriction );
//	ceil.setup( box2d.getWorld(), 0, 0, ofGetWidth(), thick, true );
	
//	left.setPhysics( wallMass, wallBounce, wallFriction );
//	left.setup( box2d.getWorld(), 0, 0, thick, ofGetHeight(), true );
//	
//	right.setPhysics( wallMass, wallBounce, wallFriction );
//	right.setup( box2d.getWorld(), ofGetWidth(), 0, thick, ofGetHeight(), true );
}

void testApp :: initOpticalField ()
{
	opticalFieldRect.width	= 160;
	opticalFieldRect.height	= 120;
	
	opticalField.init( cameraRect, opticalFieldRect );
	opticalField.setImageType( GL_LUMINANCE );
	opticalField.setMirror( false, false );
}


//--------------------------------------------------------------
void testApp::update()
{
	updateCamera();
	
	if( cameraNewFrame )
	{
		if( bUseCamera )
		{
			updateCv( camera.getPixels() );
		}
		else
		{
			updateCv( video.getPixels() );
		}
	
		opticalField.update( cameraGrayImage.getPixels() );
		
		if( ofGetFrameNum() % 3 == 0 )
		{
			int blobs;
			blobs = updateContours();
			
			if( blobs > 0 )
			{
				clearShapes();
				parseShapes();
		
				updateTriangles();
				addTrianglesToBox2d();
			}
		}
	}
	
	updateTriangleShapes();
	
	box2d.update();
}

void testApp :: updateCamera ()
{
	if( bUseCamera )
	{
		camera.grabFrame();
		cameraNewFrame = camera.isFrameNew();
	}
	else 
	{
		if( screenGrab.isRecording() )
		{
			float t;
			t = ofGetFrameNum() / (float)frameRate;
			
			float p;
			p = t / video.getDuration();
			
			video.setPosition( p );
		}
		
		cameraNewFrame = true;
	}
}

void testApp :: updateCv ( unsigned char *pixels )
{
	cameraColorImage.setFromPixels( pixels, cameraRect.width, cameraRect.height );
	cameraColorImage.mirror( false, true );
	
	cameraGrayPrevImage	= cameraGrayImage;
	cameraGrayImage		= cameraColorImage;
	
	cameraGrayDiffImage.absDiff( cameraGrayImage, cameraGrayPrevImage );
	cameraGrayDiffImage.threshold( 30 );
	
	logoSmallIntersect  = cameraGrayDiffImage;
	logoSmallIntersect &= logoSmall;
}

int testApp :: updateContours ()
{
	int maxArea;
	maxArea = logoSmallIntersect.width * logoSmallIntersect.height;
	
	float cfMinArea;
	cfMinArea = 0.003;
	
	int maxContoursToFind;
	maxContoursToFind = 20;
	
	int runningBlobs;
	
	runningBlobs = contourFinder.findContours
	(
		logoSmallIntersect,				// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	return runningBlobs;
}

void testApp :: parseShapes ()
{
	int t;
	t = contourFinder.nBlobs;
	
	if( t == 0 )
		return;
	
	for( int i=0; i<t; i++ )				// initialise shapes.
	{
		Shape s = Shape();
		
		s.parent	= -1;
		s.child		= -1;
		s.noPolys	= 1;
		
		shapes.push_back( s );
	}
	
	for( int i=0; i<t; i++ )				// check if shapes are embedded - left to right.
	{
		ofRectangle r1;
		r1.x		= contourFinder.blobs[ i ].boundingRect.x;
		r1.y		= contourFinder.blobs[ i ].boundingRect.y;
		r1.width	= contourFinder.blobs[ i ].boundingRect.width;
		r1.height	= contourFinder.blobs[ i ].boundingRect.height;
		
		Shape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i + j ) % t;
			
			ofRectangle r2;
			r2.x		= contourFinder.blobs[ k ].boundingRect.x;
			r2.y		= contourFinder.blobs[ k ].boundingRect.y;
			r2.width	= contourFinder.blobs[ k ].boundingRect.width;
			r2.height	= contourFinder.blobs[ k ].boundingRect.height;
			
			Shape& s2 = shapes.at( k );
			
			bool embedded = false;
			embedded = checkEmbeddedRectangles( r1, r2 );
			
			if( embedded )
			{
				s1.child	= k;
				s2.parent	= i;
			}
		}
	}
	
	for( int i=t-1; i>=0; i-- )				// check if shapes are embedded - right to left.
	{
		ofRectangle r1;
		r1.x		= contourFinder.blobs[ i ].boundingRect.x;
		r1.y		= contourFinder.blobs[ i ].boundingRect.y;
		r1.width	= contourFinder.blobs[ i ].boundingRect.width;
		r1.height	= contourFinder.blobs[ i ].boundingRect.height;
		
		Shape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i - j );
			
			if( k < 0 )
				k = t + k;
			
			ofRectangle r2;
			r2.x		= contourFinder.blobs[ k ].boundingRect.x;
			r2.y		= contourFinder.blobs[ k ].boundingRect.y;
			r2.width	= contourFinder.blobs[ k ].boundingRect.width;
			r2.height	= contourFinder.blobs[ k ].boundingRect.height;
			
			Shape& s2 = shapes.at( k );
			
			bool embedded;
			embedded = checkEmbeddedRectangles( r1, r2 );
			
			if( embedded )
			{
				s1.child	= k;
				s2.parent	= i;
			}
		}
	}
	
	for( int i=0; i<t; i++ )			// copy child data into parent.
	{
		Shape& s1 = shapes.at( i );
		
		bool hasChild;
		hasChild = ( s1.child > -1 );
		
		if( hasChild )
		{
			s1.noPolys += 1;
		}
		
		copyPolygonData( contourFinder.blobs[ i ].pts, s1.polyPoints );
		
		if( hasChild )
		{
			copyPolygonData( contourFinder.blobs[ s1.child ].pts, s1.polyPoints );
		}
	}
	
	for( int i=0; i<t; i++ )						// remove child contours with a parent, as they have already been copied to parent.
	{
		Shape& s1 = shapes.at( i );
		
		if( s1.parent != -1 )						// has parent. remove it.
		{
			shapes.erase( shapes.begin() + i );		// remove from array, step back counter after removed.
			
			--i;
			--t;
		}
	}
}

void testApp :: clearShapes ()
{
	for( int i=0; i<shapes.size(); i++ )
	{
		Shape& s = shapes.at( i );
		s.polyPoints.clear();
	}
	
	shapes.clear();
}

bool testApp :: checkEmbeddedRectangles( const ofRectangle& r1, const ofRectangle& r2 )
{
	bool p1, p2, p3, p4;
	
	p1 = ( ( r2.x >= r1.x ) && ( r2.y >= r1.y ) );
	p2 = ( ( r2.x + r2.width <= r1.x + r1.width ) && ( r2.y >= r1.y ) );
	p3 = ( ( r2.x + r2.width <= r1.x + r1.width ) && ( r2.y + r2.height <= r1.y + r1.height ) );
	p4 = ( ( r2.x >= r1.x ) && ( r2.y + r2.height <= r1.y + r1.height ) );
	
	return ( p1 && p2 && p3 && p4 );
}

void testApp :: copyPolygonData ( const vector<ofPoint>& p1, vector<ofPoint>& p2 )
{
	for( int i=0; i<p1.size(); i++ )
	{
		p2.push_back( ofPoint() );
		
		ofPoint& p = p2.back();
		p.x = p1.at( i ).x;
		p.y = p1.at( i ).y;
	}
}

void testApp :: copyPolygonData ( const vector<ofxPoint2f>& p1, vector<ofPoint>& p2 )
{
	for( int i=0; i<p1.size(); i++ )
	{
		p2.push_back( ofPoint() );
		
		ofPoint& p = p2.back();
		p.x = p1.at( i ).x;
		p.y = p1.at( i ).y;
	}
}

void testApp :: updateTriangles ()
{
	triangle.clear();
	
	for( int i=0; i<shapes.size(); i++ )
	{
		int resolution;
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.03 );
		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.08 );
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.2 );
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints.size() * 0.3 );
		
		triangle.triangulate( shapes.at( i ).polyPoints, resolution );
	}
	
	int t;
	t = triangles.size();
	
	for( int i=0; i<t; i++ )
	{
		ofxBox2dPolygonCustom& tri = triangles[ i ];
		
		ofPoint p = tri.getPosition();
		
		if			// check if off screen.
		(
			p.x < -100					||
			p.x > ofGetWidth()  + 100	||
			p.y > ofGetHeight() + 100
		)
		{
			tri.destroyShape();
			
			triangles.erase( triangles.begin() + i );		// remove from array, step back counter after removed.
			
			--i;
			--t;
		}
	}
}

void testApp :: addTrianglesToBox2d ()
{
	for( int i=0; i<triangle.getTriangles().size(); i++ )
	{
		ofxTriangleData& triData = triangle.getTriangles().at( i );
		
		float triScale;
//		triScale = 1.0;
		triScale = logoCropRect.width / (float)cameraRect.width;
		
		ofPoint tp;
		tp = triangle.getTriangleCenter( triData );
		
		ofPoint ta, tb, tc;
		ta = triData.a - tp;
		tb = triData.b - tp;
		tc = triData.c - tp;
		
		ofPoint tsa, tsb, tsc, tsp;
		tsa	= ta * triScale;
		tsb	= tb * triScale;
		tsc	= tc * triScale;
		tsp	= tp * triScale;

		ofPoint tspn;		// triangle scaled point + normalised.
		tspn.x = tsp.x / (float)logoCropRect.width;
		tspn.y = tsp.y / (float)logoCropRect.height;
		
		tsa.x += logoCropRect.x * 0.5;
		tsa.y += logoCropRect.y * 0.5;
		tsb.x += logoCropRect.x * 0.5;
		tsb.y += logoCropRect.y * 0.5;
		tsc.x += logoCropRect.x * 0.5;
		tsc.y += logoCropRect.y * 0.5;
		tsp.x += logoCropRect.x * 0.5;
		tsp.y += logoCropRect.y * 0.5;
		
		ofxBox2dPolygonCustom tri;
		
//		tri.setPhysics( 0, 0, 0 );			// fixed.
		tri.setPhysics
		(
			1.0,							// mass.
			ofRandom( 0.5, 0.8 ),			// bouce.
			ofRandom( 0.1, 0.7 )			// friction.
		);
		
		int color;
		color = colors[ (int)( colors.size() * ofRandom( 0.0, 1.0 ) ) ];
		
		tri.addVertex( tsa );
		tri.addVertex( tsb );
		tri.addVertex( tsc );
		tri.createShape( box2d.getWorld(), tsp.x, tsp.y );
		tri.color = color;
		
		ofPoint opVel;
		opticalField.getVelAtNorm( tspn.x, tspn.y, &opVel.x, &opVel.y );
		opVel *= 2;
		
		tri.setVelocity( opVel.x, opVel.y );
		
//		float velMax;
//		velMax = 1;
//		
//		ofxVec2f vel;
//		vel.x	= ofRandom( -velMax, velMax );
//		vel.y	= ofRandom( -velMax, velMax );
//		tri.setVelocity( vel.x, vel.y );
		
		triangles.push_back( tri );
		
		//--triangle shape.
		
		TriangleShape ts;
		ts.vertex[ 0 ]	= tsa + tsp;
		ts.vertex[ 1 ]	= tsb + tsp;
		ts.vertex[ 2 ]	= tsc + tsp;
		ts.colorFill	= color;
		ts.colorBorder	= color;
//		ts.colorFill	= 0xFFFFFF;
//		ts.colorBorder	= 0x000000;
		ts.count		= 0;
		ts.countKill	= 90;
		ts.fade			= 1.0;
		ts.alpha		= 255;
		
		triangleShapes.push_back( ts );
	}
}

void testApp :: updateTriangleShapes ()
{
	int i = 0;
	int t = triangleShapes.size();
	
	for( i; i<t; i++ )
	{
		TriangleShape& ts = triangleShapes[ i ];
		
		if( ++ts.count == ts.countKill )
		{
			triangleShapes.erase( triangleShapes.begin() + i );
			
			--i;
			--t;
		}
		else
		{
			ts.fade		= 1 - ( ts.count / (float)ts.countKill );
			ts.alpha	= (int)( 255 * ts.fade );
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );

	ofSetColor( 0xFFFFFF );

	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, 40 );
	cameraGrayDiffImage.draw( logoCropRect.x, logoCropRect.y, logoCropRect.width, logoCropRect.height );
	ofDisableAlphaBlending();
	ofSetColor( 0xFFFFFF );
	
	ofEnableAlphaBlending();
	logo.draw( 0, 0 );
	ofDisableAlphaBlending();
	
	if( bDebug )
	{
		drawDebug();
	}

	drawTriangleShapes();
	drawTriangles();
	
//	floor.draw();
//	box2d.draw();
	
	screenGrab.save();
}

void testApp :: drawDebug ()
{
	ofRectangle smlRect;
	smlRect.width	= 160;
	smlRect.height	= 120;
	
	int pad;
	pad = 10;
	
	glPushMatrix();
	glTranslatef( ofGetWidth() - smlRect.width - pad, pad, 0 );
	
	drawBorder( smlRect );
	cameraColorImage.draw( 0, 0, smlRect.width, smlRect.height );
	
	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	cameraGrayDiffImage.draw( 0, 0, smlRect.width, smlRect.height );
	
	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	logoSmall.draw( 0, 0, smlRect.width, smlRect.height );
	
	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	logoSmallIntersect.draw( 0, 0, smlRect.width, smlRect.height );
	
	glTranslatef( 0, smlRect.height + pad, 0 );
	
	drawBorder( smlRect );
	opticalField.drawOpticalFlow( 10 );
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef( logoCropRect.x, logoCropRect.y, 0 );
	
//	contourFinder.draw();
//	drawShapes();
	
	glPopMatrix();
	
}

void testApp :: drawBorder ( const ofRectangle& rect )
{
	int b = 2;
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	ofRect( -b, -b, rect.width + b * 2, rect.height + b * 2 );
}

void testApp :: drawShapes ()
{
	ofEnableSmoothing();
	
	for( int i=0; i<shapes.size(); i++ )
	{
		ofFill();
		ofSetColor( 0xFFFFFF );
		
		drawShape( shapes[ i ] );
		
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		
		drawShape( shapes[ i ] );
	}
	
	ofDisableSmoothing();
}

void testApp :: drawShape ( Shape& shape )
{
	ofBeginShape();
	
	for( int i=0; i<shape.polyPoints.size(); i++ )
	{
		ofPoint& p1 = shape.polyPoints.at( i );
		
		ofVertex( p1.x, p1.y );
	}
	
	ofEndShape( true );
}

void testApp :: drawTriangles ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		triangles[ i ].draw();
	}
}

void testApp :: drawTriangleShapes ()
{
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	for( int i=0; i<triangleShapes.size(); i++ )
	{
		TriangleShape& ts = triangleShapes[ i ];
		
		ofxColor cf = ofxColor( ts.colorFill );
		ofxColor cb = ofxColor( ts.colorBorder );
		
		int cfr = cf.red   + (int)( ( 255 - cf.red   ) * ( 1 - ts.fade ) );
		int cfg = cf.green + (int)( ( 255 - cf.green ) * ( 1 - ts.fade ) );
		int cfb = cf.blue  + (int)( ( 255 - cf.blue  ) * ( 1 - ts.fade ) );

		int cbr = cb.red   + (int)( ( 255 - cb.red   ) * ( 1 - ts.fade ) );
		int cbg = cb.green + (int)( ( 255 - cb.green ) * ( 1 - ts.fade ) );
		int cbb = cb.blue  + (int)( ( 255 - cb.blue  ) * ( 1 - ts.fade ) );
		
		ofFill();
		ofSetColor( cfr, cfg, cfb, ts.alpha );
		ofBeginShape();
		ofVertex( ts.vertex[ 0 ].x, ts.vertex[ 0 ].y );
		ofVertex( ts.vertex[ 1 ].x, ts.vertex[ 1 ].y );
		ofVertex( ts.vertex[ 2 ].x, ts.vertex[ 2 ].y );
		ofEndShape(true);
		
		ofNoFill();
		ofSetColor( cbr, cbg, cbb, ts.alpha );
		ofBeginShape();
		ofVertex( ts.vertex[ 0 ].x, ts.vertex[ 0 ].y );
		ofVertex( ts.vertex[ 1 ].x, ts.vertex[ 1 ].y );
		ofVertex( ts.vertex[ 2 ].x, ts.vertex[ 2 ].y );
		ofVertex( ts.vertex[ 0 ].x, ts.vertex[ 0 ].y );
		ofEndShape(true);
		ofEndShape();
	}
	
	ofDisableSmoothing();
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'f' )
	{
		bFullScreen = !bFullScreen;
		
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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

