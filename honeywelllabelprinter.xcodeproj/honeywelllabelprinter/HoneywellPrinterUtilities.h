//
//  HoneywellPrinterUtilities.h
//  honeywelllabeprinter
//
//  Created by pohyee on 27/05/2016.
//  Copyright © 2016 ritebozz. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HoneywellPrinterUtilities : NSObject<NSStreamDelegate>

-(void)initNetworkCommunication:(NSString *)host port:(int)port;
-(void)closeNetworkConnection;
-(void)printData:(NSMutableDictionary *)dataToPrint;

@end
