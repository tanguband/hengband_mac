/**
 *\file AppDelegate.h
 *\brief Declare the application delegate used by the OS X front end.
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#import <Cocoa/Cocoa.h>
#import "SoundAndMusic.h"

@interface AngbandAppDelegate : NSObject <NSApplicationDelegate,
        SoundAndMusicChanges> {
    NSMenu *_graphicsMenu;
    NSMenu *_commandMenu;
    NSDictionary *_commandMenuTagMap;
}
@property (nonatomic, retain) IBOutlet NSMenu *graphicsMenu;
@property (strong, nonatomic, retain) IBOutlet NSMenu *commandMenu;
@property (strong, nonatomic, retain) NSDictionary *commandMenuTagMap;
@property (strong, nonatomic) SoundAndMusicPanelController
    *soundAndMusicPanelController;
- (IBAction)newGame:(id)sender;
- (IBAction)editFont:(id)sender;
- (IBAction)openGame:(id)sender;
- (IBAction)saveGame:(id)sender;
- (IBAction)setRefreshRate:(NSMenuItem *)sender;
- (IBAction)toggleWideTiles:(NSMenuItem *)sender;
- (IBAction)showSoundAndMusicPanel:(NSMenuItem *)sender;
- (void)setGraphicsMode:(NSMenuItem *)sender;
- (void)selectWindow:(id)sender;
- (void)beginGame;

@end
