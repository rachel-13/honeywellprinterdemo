/*
 * ITCLinePrinterException.h
 * IOS Printing SDK
 *
 * Created by Teo Boon Keat on 3/12/14.
 * Copyright (c) 2014 Honeywell. All rights reserved.
 */

#import <Foundation/Foundation.h>
/**
 * @file
 * @brief Contains some error codes application can check
 */

/**
 * Exception thrown SDK methods are called before
 * an ITCLinePrinter object has been initialized.
 */
extern int const ITC_EXCEPTION_NIL_LINE_PRINTER;

/**
 * Exception thrown when a trying to send data to printer
 * without an existing connection.
 */
extern int const ITC_EXCEPTION_NO_CONNECTION;

/**
 * Exception thrown when a connect method is being called
 * while there is an existing connection.
 */
extern int const ITC_EXCEPTION_ALREADY_CONNECTED;

/**
 * Exception thrown when the specified file is corrupted
 * or cannot be found.
 */
extern int const ITC_EXCEPTION_FILE_NOT_FOUND;

/**
 * Exception thrown when a matching printer id cannot be found
 * in the command and attributes json file
 */
extern int const ITC_EXCEPTION_INVALID_PRINTER_ID;

/**
 * Exception thrown if EAAccessory accessory property of ITCExtraSettings
 * is nil when calling connect method.
 */
extern int const ITC_EXCEPTION_NIL_ACCESSORY;

/**
 * Exception thrown if NSString* protocolString property of ITCExtraSettings
 * is nil when calling connect method.
 */
extern int const ITC_EXCEPTION_NIL_PROTOCOL_STRING;

/**
 * Unexpected exception. Use for debugging purposes during development
 */
extern int const ITC_EXCEPTION_UNEXPECTED;

/**
 * Exception thrown by init method if the attribute file(json) has a wrong
 * format or corrupted. If the command and attribute file parameter is nil,
 * this exception will be thrown as well.
 */
extern int const ITC_EXCEPTION_INVALID_ATTRIBUTE_FILE;

/**
 * Exception thrown if Info.plist file does not contain a correct protocol
 * string inside "Supported External Accessory Protocol"
 */
extern int const ITC_EXCEPTION_SUPPORTED_ACCESSORY_NOT_SPECIFIED;

/**
 * LinePrinterException is thrown when LinePrinter encounters errors.
 * @brief Exception Class that is thrown when LinePrinter encounters errors.
 */
@interface ITCLinePrinterException : NSException

/** \cond */
@property int errorCode;
/** \endcond */

- (id)initWithError:(NSString *)name
             reason:(NSString *)errorMsg
          errorCode:(int)code;

/** \cond */
+ (int)formatError:(int)aErrorCode facility:(int)aFacilityCode;
/** \endcond */

/**
 * Gets the error code associated with this exception.
 * @return An integer error code.
 *
 * @code
 * //Example usage
 * @try {
 *      [lp connect];
 * }
 * @catch (LinePrinterException *exception) {
 *      //display or log error
 *      NSLog(@"ERROR:%@, errorcode:%tx", [exception reason], [exception
 *getErrorCode]);
 * }
 * @finally {
 *
 * }
 * @endcode
 */
- (int)getErrorCode;

@end
