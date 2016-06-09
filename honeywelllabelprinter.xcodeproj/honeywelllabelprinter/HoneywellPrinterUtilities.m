//
//  HoneywellPrinterUtilities.m
//  honeywelllabeprinter
//
//  Created by pohyee on 27/05/2016.
//  Copyright © 2016 ritebozz. All rights reserved.
//

#import "HoneywellPrinterUtilities.h"

@interface HoneywellPrinterUtilities()
{
    NSInputStream *inputStream;
    NSOutputStream *outputStream;
}
@end

@implementation HoneywellPrinterUtilities

- (void)initNetworkCommunication:(NSString *)host port:(int)port {
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (__bridge CFStringRef)host, port, &readStream, &writeStream);
    inputStream = (__bridge NSInputStream *)readStream;
    outputStream = (__bridge NSOutputStream *)writeStream;
    
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    [inputStream open];
    [outputStream open];
}

-(void)sendSettingCommands
{
    NSMutableString * responseString = [[NSMutableString alloc]init];
    [responseString appendString:@"SYSVAR(18)=-1 \r\n"];
    
    NSData *data = [[NSData alloc] initWithData:[responseString dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
}

-(void)uploadImage
{
    
}


- (void)closeNetworkConnection {
    
    [inputStream close];
    [outputStream close];
    
    [inputStream setDelegate:nil];
    [outputStream setDelegate:nil];
    
    [inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void)printData:(NSDictionary *)dataToPrint{
    
    [self sendSettingCommands];
    
    NSString * barcodeTypeCode = [[dataToPrint objectForKey:@"barcode"] uppercaseString];
    NSString * input = [dataToPrint objectForKey:@"input"];
    
    NSLog(@"barcodetype: %@", barcodeTypeCode);
    
    NSMutableString * mutableResponse = [[NSMutableString alloc]init];
    // image
    [mutableResponse appendString:@"PP 0,150: "];
    [mutableResponse appendString:@"AN 1: "];
    [mutableResponse appendString:@"MAG 1,4: "];
    [mutableResponse appendString:@"PM \"SKETCH\": "];
    [mutableResponse appendString:@"MAG 1,1: "];
    
    // barcode
//    [mutableResponse appendFormat:@"BF OFF,1: "];
    [mutableResponse appendString:@"BF ON: "];
    [mutableResponse appendString:@"BF \"Andale Mono\",1: "];
    [mutableResponse appendString:@"PP 140,50: "];
    [mutableResponse appendString:@"AN 2: "];
    [mutableResponse appendString:[NSString stringWithFormat:@"BARSET \"%@\": ", barcodeTypeCode]];
    [mutableResponse appendString:@"BARHEIGHT 70: "];
    [mutableResponse appendString:[NSString stringWithFormat:@"PB \"%@\": ", input]];
    
    // multiline text
    [mutableResponse appendString:@"FT \"Andale Mono\",6: "];
    [mutableResponse appendString:@"PP 5,15: "];
    [mutableResponse appendString:@"AN 1: "];
    [mutableResponse appendString:[NSString stringWithFormat:@"PX 40,170,0,\"%@\": ", @"A&W Rootbeer Orange-Berry Flavour 250ml"]];
    
    // single line price
    [mutableResponse appendString:@"FT \"Swiss 721 Bold Condensed BT\",9: "];
    [mutableResponse appendString:@"PP 160,20: "];
    [mutableResponse appendString:[NSString stringWithFormat:@"PT \"%@\": ", @"RM28000.50"]];
    [mutableResponse appendString:@"PF \r\n"];
    
    NSData *data = [[NSData alloc] initWithData:[mutableResponse dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
    
}



- (void)stream:(NSStream *)theStream handleEvent:(NSStreamEvent)streamEvent {
    
    switch (streamEvent) {
            
        case NSStreamEventOpenCompleted:
            NSLog(@"Stream opened");
            break;
            
        case NSStreamEventHasBytesAvailable:
//            NSLog(@"Has Bytes Available");
            if (theStream == inputStream) {
                
                uint8_t buffer[1024];
                int len;
                
                while ([inputStream hasBytesAvailable]) {
                    len = [inputStream read:buffer maxLength:sizeof(buffer)];
                    if (len > 0) {
                        
                        NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
                        
                        if (nil != output) {
                            NSLog(@"server said: %@", output);
                        }
                    }
                }
            }
            break;
            
        case NSStreamEventErrorOccurred:
            NSLog(@"Can not connect to the host!");
            break;
            
        case NSStreamEventEndEncountered:
            break;
            
        default:
//            NSLog(@"Unknown event");
            break;
    }
    
}

@end
