//
//  HoneywellPrinterUtilities.m
//  honeywelllabeprinter
//
//  Created by pohyee on 27/05/2016.
//  Copyright © 2016 ritebozz. All rights reserved.
//

/*
 
 Note:
 This framework is specifically designed for Honeywell PC42t Desktop Label Printer.
 All commands sent to the printer via TCP socket connection is uses the 
 Intermec Direct Protocol v.8.60—Programmer’s Reference Manual
 http://www.intermec.com/public-files/manuals/1-960597-06%20PRM%20DP%208.60.pdf
 
 */

#import "HoneywellPrinterUtilities.h"

@interface HoneywellPrinterUtilities()
{
    NSInputStream *inputStream;
    NSOutputStream *outputStream;
    NSString * printerHost;
    NSString * imageFileName;
}
@end

@implementation HoneywellPrinterUtilities


#pragma mark settings functions

- (void)initNetworkCommunication:(NSString *)host port:(int)port {
    
    imageFileName = @"1bitleaf";
    printerHost = host;
    
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


- (void)closeNetworkConnection {
    
    [inputStream close];
    [outputStream close];
    
    [inputStream setDelegate:nil];
    [outputStream setDelegate:nil];
    
    [inputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

#pragma mark general functions

-(void)printDataOnDefaultSizeLabel:(NSMutableDictionary *)dataToPrint
{
    [self sendSettingCommands];
    
    NSMutableString * printerCommandString = [self generateStandardPriceTemplate35x25mm:dataToPrint];
    
    NSData *data = [[NSData alloc] initWithData:[printerCommandString dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
    
}

-(void)printDataOn50x30mmLabel:(NSMutableDictionary*)dataToPrint templateType:(LabelTemplateType)type
{
    [self sendSettingCommands];
    
    NSMutableString * printerCommandString = [[NSMutableString alloc]init];
    
    switch (type) {
        case STANDARD_PRICE_LABEL:
            printerCommandString = [self generateStandardPriceTemplate50x30mm:dataToPrint];
            break;
            
        case FOOD_INFO_LABEL:
            printerCommandString = [self generateFoodLabelTemplate50x30mm:dataToPrint];
            break;
            
        default:
            NSLog(@"Unrecognized Label Type");
            break;
    }
    
    NSData *data = [[NSData alloc] initWithData:[printerCommandString dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
}

#pragma mark command template generator

-(NSMutableString*)generateStandardPriceTemplate50x30mm:(NSDictionary*)dataToPrint
{
    /* based on 5.0cm x 3.0cm label print area */
    /* width 400 height 241 */
    
    NSString * barcodeTypeCode = [[dataToPrint objectForKey:HONEYWELLPRT_KEY_BARCODETYPE_CODE] uppercaseString];
    NSString * barCodeInput = [dataToPrint objectForKey:HONEYWELLPRT_KEY_BARCODE_INPUT];
    NSString * itemDescription = [dataToPrint objectForKey:HONEYWELLPRT_KEY_ITEM_DESC];
    NSString * itemPrice = [dataToPrint objectForKey:HONEYWELLPRT_KEY_ITEM_PRICE];
    
    NSLog(@"barcodetype: %@", barcodeTypeCode);
    
    NSMutableString * printerCommandString = [[NSMutableString alloc]init];
    
    // image
    [printerCommandString appendString:@"PP 0,190: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:@"MAG 1,4: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PM \"%@\": ",[imageFileName uppercaseString]]];
    [printerCommandString appendString:@"MAG 1,1: "];
    
    // barcode
    [printerCommandString appendString:@"BF ON: "];
    [printerCommandString appendString:@"BF \"Andale Mono\",1: "];
    [printerCommandString appendString:@"PP 200,75: "];
    [printerCommandString appendString:@"AN 2: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"BARSET \"%@\": ", barcodeTypeCode]];
    [printerCommandString appendString:@"BARHEIGHT 70: "];
    [printerCommandString appendString:@"BARMAG 3: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PB \"%@\": ", barCodeInput]];
    
    // multiline text item description
    /* PX box_height, box_width, box_border_thickness, info */
    [printerCommandString appendString:@"FT \"Andale Mono\",6: "];
    [printerCommandString appendString:@"PP 5,35: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PX 40,400,0,\"%@\": ", itemDescription]];
    
    // single line text price
    [printerCommandString appendString:@"FT \"Swiss 721 Bold Condensed BT\",9: "];
    [printerCommandString appendString:@"PP 260,0: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PT \"%@\": ", itemPrice]];
    [printerCommandString appendString:@"PF \r\n"];

    return printerCommandString;
}

-(NSMutableString*)generateFoodLabelTemplate50x30mm:(NSMutableDictionary*)dataToPrint
{
    /* based on 5.0cm x 3.0cm label print area */
    /* width 400 height 241 */
    
    NSString * barcodeTypeCode = [[dataToPrint objectForKey:HONEYWELLPRT_KEY_BARCODETYPE_CODE] uppercaseString];
    NSString * barCodeInput = [dataToPrint objectForKey:HONEYWELLPRT_KEY_BARCODE_INPUT];
    NSString * itemDescription = [dataToPrint objectForKey:HONEYWELLPRT_KEY_ITEM_DESC];
    NSString * itemPrice = [dataToPrint objectForKey:HONEYWELLPRT_KEY_ITEM_PRICE];
    
    NSMutableString * printerCommandString = [[NSMutableString alloc]init];
    
    // multiline text item description
    /* PX box_height, box_width, box_border_thickness, info */
    [printerCommandString appendString:@"FT \"Swiss 721 Bold BT\",8: "];
    [printerCommandString appendString:@"PP 200,200: "];
    [printerCommandString appendString:@"AN 2: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PX 30,400,0,\"%@\": ", itemDescription]];
    
    // divider line below item description
    /* PL line_width, line_thickness */
    [printerCommandString appendString:@"PP 0,170: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:@"PL 400,2: "];
    
    // barcode
    [printerCommandString appendString:@"BF ON: "];
    [printerCommandString appendString:@"BF \"Andale Mono\",1: "];
    [printerCommandString appendString:@"PP 17,35: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:@"DIR 1: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"BARSET \"%@\": ", barcodeTypeCode]];
    [printerCommandString appendString:@"BARHEIGHT 50: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PB \"%@\": ", barCodeInput]];

    // multiline text company detail
    [printerCommandString appendString:@"FT \"Andale Mono\",6: "];
    [printerCommandString appendString:@"PP 200,0: "];
    [printerCommandString appendString:@"AN 2: "];
    [printerCommandString appendString:@"DIR 1: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PX 25,400,0,\"%@\": ", @"Ritebos Sdn Bhd, Genius Income"]];
    
    // divider line top company details
    [printerCommandString appendString:@"PP 0,25: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:@"PL 400,2: "];
    
    [printerCommandString appendString:@"PF \r\n"];
    return printerCommandString;
}

-(NSMutableString*)generateStandardPriceTemplate35x25mm:(NSMutableDictionary*)dataToPrint
{
    /* based on 3.5cm x 2.5cm label print area */
    /* width 280 height 201 */
    
    NSMutableString * printerCommandString = [[NSMutableString alloc]init];
    
    NSString * barcodeTypeCode = [[dataToPrint objectForKey:@"barcode"] uppercaseString];
    NSString * input = [dataToPrint objectForKey:@"input"];
    
    NSLog(@"barcodetype: %@", barcodeTypeCode);
    
    // image
    [printerCommandString appendString:@"PP 0,150: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:@"MAG 1,4: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PM \"%@\": ",[imageFileName uppercaseString]]];
    [printerCommandString appendString:@"MAG 1,1: "];
    
    // barcode
    [printerCommandString appendString:@"BF ON: "];
    [printerCommandString appendString:@"BF \"Andale Mono\",1: "];
    [printerCommandString appendString:@"PP 140,50: "];
    [printerCommandString appendString:@"AN 2: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"BARSET \"%@\": ", barcodeTypeCode]];
    [printerCommandString appendString:@"BARHEIGHT 70: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PB \"%@\": ", input]];
    
    // multiline text item description
    /* PX box_height, box_width, box_border_thickness, info */
    [printerCommandString appendString:@"FT \"Andale Mono\",6: "];
    [printerCommandString appendString:@"PP 5,15: "];
    [printerCommandString appendString:@"AN 1: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PX 40,170,0,\"%@\": ", @"A&W Rootbeer Orange-Berry Flavour 250ml"]];
    
    // single line text price
    [printerCommandString appendString:@"FT \"Swiss 721 Bold Condensed BT\",9: "];
    [printerCommandString appendString:@"PP 160,20: "];
    [printerCommandString appendString:[NSString stringWithFormat:@"PT \"%@\": ", @"RM28000.50"]];
    [printerCommandString appendString:@"PF \r\n"];
    
    return printerCommandString;
}

#pragma mark upload image

-(void)uploadImage
{
    NSString * filepath = [[NSBundle mainBundle] pathForResource:imageFileName ofType:nil];
    NSLog(@"filepath:%@",filepath);
    
    NSData *imageData = [[NSData alloc]initWithContentsOfFile:filepath];
    
    NSString * urlString = [NSString stringWithFormat:@"http://%@/manage/upload.lp?type=image",printerHost];
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:urlString]];
    
    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
    [request setHTTPShouldHandleCookies:NO];
    [request setTimeoutInterval:120];
    [request setHTTPMethod:@"POST"];
    
    NSString *boundary = @"------WebKitFormBoundary";
    
    // set Content-Type in HTTP header
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@", boundary];
    [request addValue:contentType forHTTPHeaderField:@"Content-Type"];
    
    // post body
    NSMutableData *body = [NSMutableData data];
    
    // add image data
    if (imageData) {
        [body appendData:[[NSString stringWithFormat:@"\r\n--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"; filename=\"%@\"\r\n", @"file", imageFileName] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[@"Content-Type: application/octet-stream\r\n\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[NSData dataWithData:imageData]];
        [body appendData:[[NSString stringWithFormat:@"\r\n--%@\r\n", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    }
    
    NSString * bodyString = [[NSString alloc]initWithData:body encoding:NSASCIIStringEncoding];
    NSLog(@"bodyString:\n%@",bodyString);
    
    // setting the body of the post to the request
    [request setHTTPBody:body];
    
    // set the content-length
    NSString *postLength = [NSString stringWithFormat:@"%lu", (unsigned long)[body length]];
    [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler: ^(NSData *data, NSURLResponse *response, NSError *error) {
        
        if(data.length > 0)
        {
            //success
            NSString * responseString = [[NSString alloc]initWithData:data encoding:NSASCIIStringEncoding];
            NSLog(@"responseString: \n%@",responseString);
            
            NSString * redirectUri = [self getRedirectURIFromHTMLString:responseString];
            [self onUploadImageComplete:redirectUri];
        }
    }];
    
    [task resume];
    
}

- (void)onUploadImageComplete:(NSString*)redirectURI
{
    NSString * urlString = [NSString stringWithFormat:@"http://%@/manage/%@",printerHost,redirectURI];
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:urlString]];
    
    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
    [request setHTTPShouldHandleCookies:NO];
    [request setTimeoutInterval:120];
    [request setHTTPMethod:@"GET"];
    
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler: ^(NSData *data, NSURLResponse *response, NSError *error) {
        if(data.length > 0)
        {
            //success
            NSString * responseString = [[NSString alloc]initWithData:data encoding:NSASCIIStringEncoding];
            NSLog(@"responseString: \n%@",responseString);
        }
    }];
    
    [task resume];
}

-(NSString *)getRedirectURIFromHTMLString:(NSString*)htmlString
{
    NSRange rangePrefix = [htmlString rangeOfString:@"url="];
    if (rangePrefix.location == NSNotFound) {
        NSLog(@"Search string was not found");
        return @"";
    }
    
    NSRange rangeSuffix = [htmlString rangeOfString:@"</head>"];
    if(rangeSuffix.location == NSNotFound){
        NSLog(@"Search string was not found");
        return @"";
    }
    
    NSRange uriStringRange = NSMakeRange(rangePrefix.location + rangePrefix.length,
                                         rangeSuffix.location - (rangePrefix.location + rangePrefix.length) - 5);
    
    NSString *redirectUri = [htmlString substringWithRange:uriStringRange];
    NSLog(@"redirectUri: %@", redirectUri);
    
    return redirectUri;
}

#pragma mark stream delegates

- (void)stream:(NSStream *)theStream handleEvent:(NSStreamEvent)streamEvent {
    
    switch (streamEvent) {
            
        case NSStreamEventOpenCompleted:
            
            if(theStream == inputStream){
                NSLog(@"Input stream opened");
            } else {
                NSLog(@"Output stream opened");
                [self uploadImage];
            }
            break;
            
        case NSStreamEventHasBytesAvailable:
            
            if (theStream == inputStream) {
                
                uint8_t buffer[1024];
                long len;
                
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
            NSLog(@"Stream event end occured");
            break;
            
        default:
            break;
    }
    
}

@end
