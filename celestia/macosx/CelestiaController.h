//
//  CelestiaController.h
//  celestia
//
//  Created by Bob Ippolito on Tue May 28 2002.
//  Copyright (c) 2002 Chris Laurel. All rights reserved.
//
#define __AIFF__

#import <AppKit/AppKit.h>
#import "CelestiaAppCore.h"
#import "CelestiaSettings.h"
#import "FavoritesDrawerController.h"
#import "RenderPanelController.h"

@interface CelestiaController : NSWindowController 
{
    CelestiaSettings* settings;
    CelestiaAppCore* appCore;
    BOOL ready;
    BOOL isDirty;
    IBOutlet NSPanel *loadingPanel;
    IBOutlet NSProgressIndicator *loadingIndicator;
    IBOutlet NSOpenGLView *glView;
    IBOutlet NSWindow *mainWindow;
    IBOutlet NSWindow *gotoWindow;
    IBOutlet FavoritesDrawerController *favoritesDrawerController;
    IBOutlet RenderPanelController *renderPanelController;
    NSTimer* timer;
    volatile NSThread *computeThread;
    volatile BOOL computeThreadShouldTerminate;

    NSConditionLock* startupCondition;
    int keyCode, keyTime;
    NSString *pendingScript;
    NSString *pendingUrl;
}
-(BOOL)applicationShouldTerminate:(id)sender;
-(BOOL)windowShouldClose:(id)sender;
-(IBAction)back:(id)sender;
-(IBAction)forward:(id)sender;
-(IBAction)showInfoURL:(id)sender;
-(IBAction)openScript:(id)sender;
-(void)setDirty;
-(void)forceDisplay;
-(void)resize;
-(void)startInitialization;
-(void)finishInitialization;
-(void)display;
-(void)awakeFromNib;
-(void)keyPress:(int)code hold:(int)time;
-(void)setupResourceDirectory;
+(CelestiaController*) shared;
- (void) fatalError: (NSString *) msg;

-(void)addSurfaceMenu:(NSMenu*)contextMenu;
-(BOOL)validateMenuItem:(id)item;
-(IBAction)activateMenuItem:(id)item;
@end
