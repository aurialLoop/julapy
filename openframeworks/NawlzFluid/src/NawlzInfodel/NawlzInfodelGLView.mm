//
//  NawlzSandGLView.m
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "NawlzInfodelGLView.h"
#import "Defines.h"
#import "FilePath.h"

#define USE_DEPTH_BUFFER 0

@implementation NawlzInfodelGLView

@synthesize context, backingWidth, backingHeight, animationTimer;

+ (Class) layerClass 
{
	return [CAEAGLLayer class];
}

#pragma mark -
#pragma mark Initialization and Shutdown

//DEBUG_LINE(BOOL glViewExists = NO);

- (id)initWithFrame:(CGRect)aRect
{
	//ASSERT(!glViewExists);
	//DEBUG_LINE(glViewExists = YES);
	
	if ((self = [super initWithFrame:aRect])) {
        // Get the layer
		self.multipleTouchEnabled = YES;
		self.opaque = NO;
		
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = NO;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		
		
		if (!context || ![EAGLContext setCurrentContext:context]) {
			[self release];
			return nil;
		}
		
		animationFrameInterval = 1;
		lastTime = 0.0f;
		
		[self createFramebuffer];
		[self initOpenGL];
		[self clearScreen];	
		
		[self initNawlzInfodel ];
	}
    return self;
}

/////////////////////////////////////////////////////
//	NAWLZ FLUID.
/////////////////////////////////////////////////////

- (void) initNawlzInfodel
{
	nawlzInfodel = new NawlzInfodel();
	
//	[ self createBackgroundTexture ];
    [ self createHarleyOneTexture ];
    [ self createHarleyTwoTexture ];
	[ self createParticleTexture ];
	
	nawlzInfodel->setup();
}

/////////////////////////////////////////////////////
//	TEXTURES.
/////////////////////////////////////////////////////

- (void) createBackgroundTexture
{
	NSString*	imagePath;
	UIImage*	image;
	
	GLubyte* pixels;
	int imageWidth;
	int imageHeight;
	
	imagePath	= [ FilePath pathForAsset : @"infodel_bg_clean.png" ];
	image		= [ [ UIImage alloc ] initWithContentsOfFile : imagePath ];
	
	CGContextRef spriteContext;
	CGImageRef	cgImage = image.CGImage;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	imageWidth	= CGImageGetWidth(cgImage);
	imageHeight	= CGImageGetHeight(cgImage);
	
	pixels			= (GLubyte *) malloc( imageWidth * imageHeight * bytesPerPixel);
	spriteContext	= CGBitmapContextCreate(pixels, imageWidth, imageHeight, CGImageGetBitsPerComponent(cgImage), imageWidth * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)imageWidth, (CGFloat)imageHeight), cgImage);
	CGContextRelease(spriteContext);
	
	nawlzInfodel->createBackgroundTexture( pixels, imageWidth, imageHeight, GL_RGBA );
	
	free( pixels );
	[ image release ];
}

- (void) createHarleyOneTexture
{
	NSString*	imagePath;
	UIImage*	image;
	
	GLubyte* pixels;
	int imageWidth;
	int imageHeight;
	
	imagePath	= [ FilePath pathForAsset : @"infodel_harley_0.png" ];
	image		= [ [ UIImage alloc ] initWithContentsOfFile : imagePath ];
	
	CGContextRef spriteContext;
	CGImageRef	cgImage = image.CGImage;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	imageWidth	= CGImageGetWidth(cgImage);
	imageHeight	= CGImageGetHeight(cgImage);
	
	pixels			= (GLubyte *) malloc( imageWidth * imageHeight * bytesPerPixel);
	spriteContext	= CGBitmapContextCreate(pixels, imageWidth, imageHeight, CGImageGetBitsPerComponent(cgImage), imageWidth * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)imageWidth, (CGFloat)imageHeight), cgImage);
	CGContextRelease(spriteContext);
	
	nawlzInfodel->createHarleyOneTexture( pixels, imageWidth, imageHeight, GL_RGBA, 366, 311 );
	
	free( pixels );
	[ image release ];
}

- (void) createHarleyTwoTexture
{
	NSString*	imagePath;
	UIImage*	image;
	
	GLubyte* pixels;
	int imageWidth;
	int imageHeight;
	
	imagePath	= [ FilePath pathForAsset : @"infodel_harley_1.png" ];
	image		= [ [ UIImage alloc ] initWithContentsOfFile : imagePath ];
	
	CGContextRef spriteContext;
	CGImageRef	cgImage = image.CGImage;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	imageWidth	= CGImageGetWidth(cgImage);
	imageHeight	= CGImageGetHeight(cgImage);
	
	pixels			= (GLubyte *) malloc( imageWidth * imageHeight * bytesPerPixel);
	spriteContext	= CGBitmapContextCreate(pixels, imageWidth, imageHeight, CGImageGetBitsPerComponent(cgImage), imageWidth * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)imageWidth, (CGFloat)imageHeight), cgImage);
	CGContextRelease(spriteContext);
	
	nawlzInfodel->createHarleyTwoTexture( pixels, imageWidth, imageHeight, GL_RGBA, 438, 366 );
	
	free( pixels );
	[ image release ];
}

- (void) createParticleTexture
{
	NSString*	imagePath;
	UIImage*	image;
	
	GLubyte* pixels;
	int imageWidth;
	int imageHeight;
	
	imagePath	= [ FilePath pathForAsset : @"infodel_particle.png" ];
	image		= [ [ UIImage alloc ] initWithContentsOfFile : imagePath ];
	
	CGContextRef spriteContext;
	CGImageRef	cgImage = image.CGImage;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	imageWidth	= CGImageGetWidth(cgImage);
	imageHeight	= CGImageGetHeight(cgImage);
	
	pixels			= (GLubyte *) malloc( imageWidth * imageHeight * bytesPerPixel);
	spriteContext	= CGBitmapContextCreate(pixels, imageWidth, imageHeight, CGImageGetBitsPerComponent(cgImage), imageWidth * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)imageWidth, (CGFloat)imageHeight), cgImage);
	CGContextRelease(spriteContext);
	
	nawlzInfodel->createParticleTexture( pixels, imageWidth, imageHeight, GL_RGBA );
	
	free( pixels );
	[ image release ];
}

/////////////////////////////////////////////////////
//	DESTROY.
/////////////////////////////////////////////////////

- (void)dealloc 
{
	//ASSERT(glViewExists);
	//DEBUG_LINE(glViewExists = NO);
	self.animationTimer = nil;
	
	if( nawlzInfodel )
	{
		delete nawlzInfodel;
		nawlzInfodel = NULL;
	}
	
	[self destroyFramebuffer];
	
	if ([EAGLContext currentContext] == context) 
	{
		[EAGLContext setCurrentContext:nil];
	}
	
	[context release];	
	
	[super dealloc];
}

/////////////////////////////////////////////////////
//	INIT YEAH.
/////////////////////////////////////////////////////

- (void)initOpenGL {
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
	glOrthof(0, backingWidth, 0, backingHeight, -1, 1);
	// Switch to GL_MODELVIEW so we can now draw our objects
	glMatrixMode(GL_MODELVIEW);
	
	// Set the viewport
    glViewport(0, 0, backingWidth, backingHeight);
	
	glDisable(GL_DEPTH_TEST);
    
    // Enable the OpenGL states we are going to be using when rendering
    glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
}

/////////////////////////////////////////////////////
//	ANIMATION TIMER.
/////////////////////////////////////////////////////

/*- (void)startAnimation {
 animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(render) userInfo:nil repeats:TRUE];
 }*/
- (void)startAnimation {
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 30.0) * animationFrameInterval) target:self selector:@selector(render) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
    self.animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    [animationTimer invalidate];
    animationTimer = newTimer;
	
	if (newTimer)
	{
		[[NSRunLoop currentRunLoop] addTimer: animationTimer forMode: NSRunLoopCommonModes];
	}
}

/////////////////////////////////////////////////////
//	RENDER.
/////////////////////////////////////////////////////

#pragma mark -
#pragma mark Drawing

- (void)render {
	
	//DLog("Render GL");
	//NLog(@"%@", self.superview);
	
	if (!self.superview)
	{
		DLog("Failed to delete the gl view after it has been removed from the stage. Feel bad about yourself.");
		[self stopAnimation];
		
		[context release];	
		
		context = nil;
		return;
	}
	
	CFTimeInterval		time;
	float				delta;
	
	// Apple advises to use CACurrentMediaTime() as CFAbsoluteTimeGetCurrent() is synced with the mobile
	// network time and so could change causing hiccups.
	time = CACurrentMediaTime();
	//time = CFAbsoluteTimeGetCurrent();
	delta = (time - lastTime);
	
	
	[self beginRender];
	
	if( nawlzInfodel )
	{
		nawlzInfodel->update();
		
		glPushMatrix();					// not sure why the GL view is upside down and back to front.
		glTranslatef( 512, 384, 0 );	// anyhoo, this fixes it.
		glRotatef( 180, 0, 0, 1 );		// conduct might have a more elegant solution for this.
		glScalef( -1, 1, 1 );
		glTranslatef( -512, -384, 0 );
		
		nawlzInfodel->draw();
		
		glPopMatrix();
	}
	
	[self endRender];
	
	lastTime = time;
	
}
- (void)clearScreen
{
	[EAGLContext setCurrentContext:context];
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
	
}

- (void)beginRender
{
	[EAGLContext setCurrentContext:context];
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glViewport(0, 0, backingWidth, backingHeight);
	glClear(GL_COLOR_BUFFER_BIT);
}

- (void)endRender
{
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

/////////////////////////////////////////////////////
//	TOUCH HANDLERS.
/////////////////////////////////////////////////////

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
	if( !nawlzInfodel )
		return;
    
    nawlzInfodel->toggleHarley();
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{
	if( !nawlzInfodel )
		return;
	
	for( int i = 0; i < (int)[touches count]; ++i )
	{
		UITouch* touch				= [ [ touches allObjects ] objectAtIndex : i ];
		CGPoint currentLocation		= [ touch locationInView : self ];
		CGPoint previousLocation	= [ touch previousLocationInView : self ];
		
		nawlzInfodel->mouseMoved( currentLocation.x, currentLocation.y );
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
	//
}

/////////////////////////////////////////////////////
//	FRAME BUFFER.
/////////////////////////////////////////////////////

#pragma mark -
#pragma mark OpenGL Framebuffer helper methods

- (BOOL)createFramebuffer 
{	
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) 
	{
		return NO;
	}
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	return YES;
}

- (void)destroyFramebuffer 
{
	
	glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
	
	if(depthRenderbuffer) {
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
}

@end