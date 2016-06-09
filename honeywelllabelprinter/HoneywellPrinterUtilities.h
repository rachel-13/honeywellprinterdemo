//
//  HoneywellPrinterUtilities.h
//  honeywelllabeprinter
//
//  Created by pohyee on 27/05/2016.
//  Copyright © 2016 ritebozz. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#pragma mark framework common constants/enums

#define HONEYWELLPRT_KEY_ITEM_PRICE         @"itemPrice"
#define HONEYWELLPRT_KEY_ITEM_DESC          @"itemDescription"
#define HONEYWELLPRT_KEY_BARCODETYPE_CODE   @"barcodeTypeCode"
#define HONEYWELLPRT_KEY_BARCODE_INPUT      @"barcodeInput"

typedef NS_ENUM (NSInteger,LabelTemplateType) {
    STANDARD_PRICE_LABEL = 0,
    FOOD_INFO_LABEL,
    OTHER_LABEL
};

#pragma mark class interface begin

@interface HoneywellPrinterUtilities : NSObject<NSStreamDelegate>

-(void)initNetworkCommunication:(NSString *)host port:(int)port;
-(void)closeNetworkConnection;
-(void)printDataOnDefaultSizeLabel:(NSMutableDictionary *)dataToPrint;
-(void)printDataOn50x30mmLabel:(NSMutableDictionary *)dataToPrint templateType:(LabelTemplateType)type;
@end
