/*
 * ITCExtraSettings.h
 * IOS Printing SDK
 * 
 * Created by Teo Boon Keat on 4/12/14.
 * Copyright (c) 2014 Honeywell. All rights reserved.
 */ 

#import <Foundation/Foundation.h>
#import <ExternalAccessory/ExternalAccessory.h>

/**
 * This class defines extra settings that is required for the LinePrinter.
 * For iOS, LinePrinter requires an EAAcessory instance and a protocolString
 * for connection.
 *
 * Once your application has identified the correct EAAccessory, you
 * need to save it to a ExtraSettings singleton. This instance is then being
 * used by LinePrinter init method to open a connection to the printer.
 * @code
 * EAAcessory *acc;
 * NSString *protocol;
 *
 * acc = identifiedMFIAccessory;
 * protocol = identifiedProtocolString; // e.g. "com.honeywell.printer"
 * //getting the singleton
 * ExtraSettings *printerExtraSettings = [ExtraSettings retrieveExtraSettings];
 *
 *
 * //saving the settings...
 * [printerExtraSettings setProtocolString:protocol];
 * [printerExtraSettings setAccessory:acc];
 * @endcode
 * @brief Information required by SDK to make connection to printer.
 */
@interface ITCExtraSettings : NSObject

/**
 * An EAAccessory object.
 * This object is obtained
 */
@property (getter=getAccessory, nonatomic, strong) EAAccessory *accessory;

/**
 * A NSString. Eg. "com.honeywell.printer"
 */
@property (getter=getProtocolString, nonatomic, strong)
    NSString *protocolString;

/**
 * Provides a ExtraSettings singleton appropriate for use on the main thread.
 * if the context doesn't already exist it is created and bound to the
 * persistent store coordinator for the application, otherwise the existing
 * singleton context is returned.
 * @returns The shared ExtraSettings for the application
 */
+ (ITCExtraSettings *)retrieveExtraSettings;

@end
