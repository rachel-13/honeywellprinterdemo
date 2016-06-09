/*
 * ITCLinePrinter.h
 * IOS Printing SDK
 *
 * Created by sgkrbe on 9/7/14.
 * Copyright (c) 2014 Honeywell. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <ExternalAccessory/ExternalAccessory.h>
#import "ITCPrintProgressListener.h"
#import "ITCExtraSettings.h"

extern NSInteger const SYMBOLOGY_CODE39;
extern NSInteger const SYMBOLOGY_CODE128;
extern NSInteger const DEGREE_0;
extern NSInteger const DEGREE_90;
extern NSInteger const DEGREE_180;
extern NSInteger const DEGREE_270;

/**
 *
 * This class represents an Intermec mobile receipt printer and provides methods
 * to communicate with the printer.
 * <p>
 * <H3>Overview of LinePrinter Features</H3>
 * <UL>
 *   <LI> Communication between iOS and printer via Bluetooth. </LI>
 *   <LI> Sends data string or byte array to the printer for report printing or
 *        custom printer commands. </LI>
 *   <LI> Change text font styles. Font styles supported by the
 *        ITCLinePrinter class include bold, compressed, double high, double
 *wide. </LI>
 *   <LI> Print bitmap or PNG graphics.  </LI>
 *   <LI> Print CODE39 and CODE128 barcodes.  </LI>
 *   <LI> Provide the print job sending progress via the
 *        {@link ITCPrintProgressEvent}.  These events inform the iOS
 *application of
 *        when the report is started and when the printer is done sending report
 *over
 *        to printer. </LI>
 * </UL>
 * <p>
 * @brief Class representing, and providing methods to communicate to,
 * an Intermec PR2 or PR3 printer.
 *
 */
@interface ITCLinePrinter : NSObject
/** \cond */
@property (nonatomic, strong) NSMutableArray *progressListeners;
/** \endcond */

/**
 * Inits a ITCLinePrinter object with the specified settings. The
 * <code>aCmdAttribFilePath</code> parameter may contain an absolute
 * file path or a string. The file or string contents must conform to
 * the commands and attributes format specified in the API Guide.
 * <p>
 * The <code>aPrinterID</code> parameter specifies an identifier which
 * is used to load the printer settings from the commands and attributes
 * specified in the <code>aCmdAttribFilePath</code> parameter.
 * <p>
 * The <code>extraSettings</code> parameter is a required parameter on the
 * iOS platform and may be optional on other platforms.
 * The extraSettings instance should be obtained as a singleton using the
 * <code>retrieveExtraSettings</code> method.
 * @code
 * ITCExtraSettings *extraSettings = [ITCExtraSettings retrieveExtraSettings];
 * @endcode
 * The <code>extraSettings</code> singleton should have an EAAcessory
 * object and protocolString set.
 * @code
 * [extraSettings setProtocolString:identifiedProtocolString];
 * [extraSettings setAccessory:identifiedAccessory];
 *
 * NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
 * NSString *profilePath = [resourcePath
 *stringByAppendingPathComponent:@"printer_profiles.JSON"];
 *
 * ITCLinePrinter *lp = [[ITCLinePrinter alloc] init:profilePath
 *printerID:@"PR2" extraSettings:printerExtraSettings];
 * @endcode
 *
 * @param aCmdAttribFilePath A string specifying the absolute path of the
 * commands and attributes file, or a string containing the printer commands
 * and attributes.
 * @param aPrinterID A string containing the printer identifier that is
 * used to load the specific printer settings from the specified commands
 * and attributes file.
 * @param aPrinterURI A string in URI format that specifies how to connect
 * to the printer.
 * @param extraSettings An ITCExtraSettings object.
 * @exception LinePrinterException if the parameters are invalid or an
 *unexpected
 * error occurs.
 */
- (id)init:(NSString *)aCmdAttribFilePath
        printerID:(NSString *)aPrinterID
    extraSettings:(ITCExtraSettings *)aExtraSettings;

/**
 * Adds the specified listener to receive print progress events from this
 * object.
 * @code
 * //Example usage
 * ITCPrintProgressListener *myListener = [[ITCPrintProgressListener alloc]
 * init];
 * myListener.delegate = self;
 * [lp addPrintProgressListener:myListener];
 * @endcode
 * @param listener A ITCPrintProgressListener object.
 * @see removePrintProgressListener:
 * @see ITCPrintProgressEvent
 * @see ITCPrintProgressListener
 */
- (void)addPrintProgressListener:(ITCPrintProgressListener *)listener;

/**
 * Releases the resources used by the ITCLinePrinter. This method
 * should be called when you are finished with the ITCLinePrinter
 * object. If you attempt to call other ITCLinePrinter methods after
 * this method call, an ITCLinePrinterException will be thrown.
 * @code
 * //Example usage
 * [lp connect];
 * [lp writeLine:@"stuff"];
 * //Send more commands to printer...
 *
 * [lp disconnect];
 * [lp close];
 * @endcode
 * @exception ITCLinePrinterException if an error occurs in closing the
 * connection
 * to the printer.
 */
- (void)close;

/**
 * Opens a connection to the printer identified in the init method call. After
 * the connection is successfully opened, it sends the commands specified
 * in the Initialize and NormalFont attributes to the printer. These
 * attributes are defined in the commands and attributes file (.JSON file) or
 * string
 * specified in the ITCLinePrinter init method. The first command sent to
 * the printer (writeLine, write, formFeed methods) would trigger a
 * ITCPrintProgressEvent
 * with a ITCPrintProgressEventMessageType - PRINT_PROGRESS_EVENT_STARTDOC.
 *
 * The application must call this method to connect to the printer
 * before invoking other methods to interact with the printer.
 * Otherwise, a LinePrinterException will be thrown.
 * @code
 * //Example usage
 * @try {
 *      [lp connect];
 * }
 * @catch (LinePrinterException *exception) {
 *      //display or log error
 *      NSLog(@"ERROR:%@, errorcode:%tx", [exception reason], [exception
 * getErrorCode]);
 * }
 * @finally {
 *
 * }
 * @endcode
 * @exception LinePrinterException if an error occurs in opening the connection
 * to the printer.
 * @see ITCPrintProgressEvent
 * @see ITCPrintProgressEventMessageType
 */
- (void)connect;

/**
 * Closes the existing printer connection. This method may fire several
 * ITCPrintProgressEvent events with different message types.
 *
 * If the {@link endDoc} method has not been called since the document
 * was started, the events are fired in the the following order:
 * <UL>
 *   <LI> {@link PRINT_PROGRESS_EVENT_ENDDOC}</LI>
 *   <LI> {@link PRINT_PROGRESS_EVENT_FINISHED}</LI>
 *   <LI> {@link PRINT_PROGRESS_EVENT_COMPLETE}</LI>
 * </UL>
 * Note: The event with the {@link PRINT_PROGRESS_EVENT_COMPLETE} message type
 * is only fired
 * if the document has been completely sent/printed without errors or being
 * cancelled.
 * If the \link endDoc \endlink method was called to indicate the end of the
 * document and a new document has not been started, only the \link
 * PRINT_PROGRESS_EVENT_FINISHED \endlink
 * is fired.
 * @exception ITCLinePrinterException if an error occurs in closing the
 * connection to the printer.
 */
- (void)disconnect;

/**
 * Indicates the end of the document and causes one or more
 * ITCPrintProgressEvent events to be fired. The events are fired in
 * the following order:
 * <UL>
 *   <LI> {@link PRINT_PROGRESS_EVENT_ENDDOC}</LI>
 *   <LI> {@link PRINT_PROGRESS_EVENT_COMPLETE}</LI>
 * </UL>
 *
 * Note: The ITCPrintProgressEvent with the \link PRINT_PROGRESS_EVENT_COMPLETE
 * \endlink
 * message type is only fired if the document has been completely sent
 * without errors or being cancelled.
 *
 * If the application does not close the printer connection via the
 * {@link disconnect} method after this method call, the next command or
 * data sent to the printer would trigger a ITCPrintProgressEvent with
 * a message type of \link PRINT_PROGRESS_EVENT_STARTDOC \endlink.
 *
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)endDoc;

/**
 * Sends all the data in the internal print buffer to the printer via the
 * transport medium. Once the printer receives the data, it may not
 * immediately print the data. For many printers this is forced by sending
 * a line feed character.
 * @exception ITCLinePrinterException if an error occurs in sending the text to
 * the printer or if an active connection to the printer does not exist.
 */
- (void)flush;

/**
 * Sends the printer FormFeed command to the printer which feeds the paper
 * to the top of the next page.
 * This method will cause a flush of the internal buffer but will not
 * update the line number or page count properties, or cause break point
 * processing to be performed.
 * @exception ITCLinePrinterException if an error occurs in sending the command
 * to
 * the printer or if an active connection to the printer does not exist.
 */
- (void)formFeed;

/**
 * Gets the number of bytes sent to the printer since the last
 * connect method call.
 * @returns An integer value for the number of bytes sent to the printer
 * since the last connect method call.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (NSInteger)getBytesWritten;

/**
 * Advances the printer the specified number of lines. This method also
 * flushes the internal print buffer.
 * @param numLines An integer value specifying the number of lines to
 * advance the printer media. If zero or a negative value is specified,
 * it will be the same as calling the @b flush method.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 * @see flush
 */
- (void)newLine:(NSInteger)numLines;

/**
 * Removes the specified listener so that it no longer receives
 * ITCPrintProgressEvent
 * messages from this ITCLinePrinter object.
 * @param listener The ITCPrintProgressListener object to remove.
 * @see addPrintProgressListener:
 * @see ITCPrintProgressEvent
 * @see ITCPrintProgressListener
 */
- (void)removePrintProgressListener:(ITCPrintProgressListener *)listener;

/**
 * Sends a custom command identified by the specified command identifier
 * to the internal print buffer. The contents of the internal buffer is
 * sent to the printer via the transport medium when the buffer is full
 * or when a call to @c flush or @c newLine method is made.
 *
 * The @c aCommandID parameter specifies a setting name defined
 * in the current printer settings. The command setting value must be a
 * NSString or a unsigned char array. For example, the following setting defines
 * a
 * slogan
 *
 * \"MySlogan\" : \"Save Time and Money at MYSTORE.com\"
 *
 * When you call:
 * @code
 * [lp sendCustomCommand:@"MySlogan"];
 * @endcode
 * it sends
 *
 * \"Save Time and Money at MYSTORE.com\"
 *
 * to the internal print buffer to be printed.
 *
 * Besides the printer settings loaded from the commands and attributes
 * file specified in the ITCLinePrinter @c init method, you may also add custom
 * commands via the
 * <pre>
 * -(void) setSettingBytes:(NSString*)settingName value:(Byte[]) byteArray;
 * </pre>
 * @code
 * [lp setSettingBytes:@"myCommand" value:byteArray];
 * [lp sendCustomCommand:@"myCommand"];
 * @endcode
 * and
 * <pre>
 * -(void) setSettingString:(NSString*)settingName value:(NSString*)string;
 * </pre>
 * @code
 * unsigned char byteArray[] = {'\x42', '\x43', '\x0d'};
 * [lp setSettingString:@"myCommand" value:@"Send Custom Command Test
 * String\r"];
 * [lp sendCustomCommand:@"myCommand"];
 * @endcode
 * methods.
 * @param aCommandID NSString string containing the name of a setting in the
 * current printer settings.
 * @exception ITCLinePrinterException if the specified command identifier is
 * null or does not exist in the current printer setting, or if an active
 * connection to the printer does not exist.
 * @see setSettingBytes:value:
 * @see setSettingString:value:
 */
- (void)sendCustomCommand:(NSString *)aCommandID;

/**
 * Enables or disables the bold font style on the printer. This method writes
 * the command that turns the font attribute on or off to the internal
 * print buffer. The bytes written may be buffered until the internal
 * buffer is full, or a call to <code>flush</code> or <code>newLine</code>
 * is made.
 * <p>
 * If the printer does not support this font style, no action is taken.
 * @param enabled A Boolean @c YES value to enable bold font or @c NO
 * to disable it.
 *
 * @code
 * //Example usage to enable bold
 * [lp setBold:YES];
 * @endcode

 * @exception ITCLinePrinterException If an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)setBold:(BOOL)enabled;

/**
 * Enables or disables the compressed font style on the printer. This method
 * writes the command that turns the font attribute on or off to the internal
 * print buffer. The bytes written may be buffered until the internal
 * buffer is full, or a call to <code>flush</code> or <code>newLine</code>
 * is made.
 * <p>
 * If the printer does not support this font style, no action is taken.
 * @param enabled A Boolean @c YES value to enable compress font or @c NO
 * to disable it.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)setCompress:(BOOL)enabled;

/**
 * Enables or disables the double high font style on the printer. This method
 * writes the command that turns the font attribute on or off to the internal
 * print buffer. The bytes written may be buffered until the internal
 * buffer is full, or a call to <code>flush</code> or <code>newLine</code>
 * is made.
 * <p>
 * If the printer does not support this font style, no action is taken.
 * @param enabled A Boolean @c YES value to enable double high font or @c NO
 * to disable it.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)setDoubleHigh:(BOOL)enabled;

/**
 * Enables or disables the double wide font style on the printer. This method
 * writes the command that turns the font attribute on or off to the internal
 * print buffer. The bytes written may be buffered until the internal
 * buffer is full, or a call to <code>flush</code> or <code>newLine</code>
 * is made.
 * <p>
 * If the printer does not support this font style, no action is taken.
 * @param enabled A Boolean @c YES value to enable double wide font or @c NO
 * to disable it.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)setDoubleWide:(BOOL)enabled;

/**
 * Adds the specified boolean setting to the current printer settings if
 * the setting name does not already exists; otherwise, the existing
 * setting value will be overridden.
 * @param settingName A string containing the setting name.
 * @param bValue A Boolean setting value @c YES or @c NO.
 * @throws ITCLinePrinterException if an unexpected error occurs.
 */
- (void)setSettingBool:(NSString *)settingName value:(BOOL)bValue;

/**
 * Adds the specified byte array setting to the current printer settings
 * if the setting name does not already exists; otherwise, the existing
 * setting value will be overridden. Method can be used to set a command
 * identifier for sendCustomCommand method.
 * @param settingName NSString containing the setting name.
 * @param bytesArray A unsigned char array for the setting value.
 * @exception ITCLinePrinterException if an unexpected error occurs.
 */
- (void)setSettingBytes:(NSString *)settingName value:(Byte[])byteArray;

/**
 * Adds the specified integer setting to the current printer settings if
 * the setting name does not already exists; otherwise, the existing
 * setting value will be overridden.
 * @param settingName NSString string containing the setting name.
 * @param intValue NSInteger setting value.
 * @exception ITCLinePrinterException if an unexpected error occurs.
 */
- (void)setSettingNum:(NSString *)settingName value:(NSInteger)intValue;

/**
 * Adds the specified string setting to the current printer settings if
 * the setting name does not already exists; otherwise, the existing
 * setting value will be overridden. Method can be used to set a command
 * identifier for sendCustomCommand method.
 * @param settingName NSString string containing the setting name.
 * @param string NSString string containing the setting value.
 * @exception ITCLinePrinterException if an unexpected error occurs.
 */
- (void)setSettingString:(NSString *)settingName value:(NSString *)string;

/**
 * Writes a NSString to the internal print buffer. The data is
 * sent to the printer via the transport medium when the internal
 * buffer is full or when a call to <code>flush</code> or <code>newLine</code>
 * is made.
 * @param string NSString to be written to the internal print
 * buffer.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)write:(NSString *)string;

/**
 * Generates the specified bar code and prints it.
 * <p>
 * The unit of measurement for the <code>aXOffset</code> parameter is
 * printhead dots. Please refer to the specific printer User Manual for the
 * media width in printhead dots. For the PR2 printers, the default is 384 dots.
 * @param aSymbology An integer value identifying the symbology of the
 * bar code to be printed.  Valid values are defined in
 * <code>ITCLinePrinter.h</code>
 * <pre>
 * SYMBOLOGY_CODE39
 * SYMBOLOGY_CODE128
 * </pre>
 * @param aData A string specifying the data contained in the bar code.
 * @param aSize An integer value specifying the height of the bar code in
 * printhead dots.
 * @param aXOffset An integer value specifying the number of dots of white
 * space to the left of the bar code.
 *
 * @code
 * //Example usage
 * //Printing barcode of height 50 dots
 * [lp writeBarcode:SYMBOLOGY_CODE39 data:@"12341234" size:50 xOffset:0];
 * @endcode

 * @exception ITCLinePrinterException if unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)writeBarcode:(NSInteger)aSymbology
                data:(NSString *)aData
                size:(NSInteger)aSize
             xOffset:(NSInteger)aXOffset;

/**
 * Prints the specified graphic file. The recommended graphic file format
 * is black and white monochrome BMP or PNG. The result of other graphic file
 * formats is not guaranteed. The graphic can be rotated and stretched to
 * fit the specified width and height.
 * <p>
 * The unit of measurement for the <code>aXOffset</code>, <code>aWidth</code>,
 * and <code>aHeight</code> parameters is print head dots. Please refer to
 * the specific printer User Manual for the media width in print head dots.
 * For the PR2 printers, the default is 384 dots.
 * @param aGraphicFilePath A string containing the file path of the graphic
 * to be printed.
 * @param aRotation An integer value specifying the degrees to rotate the
 * image. Valid values are defined in <code>ITCLinePrinter.h</code>.
 * <pre>
 * DEGREE_0
 * DEGREE_90
 * DEGREE_180
 * DEGREE_270
 * </pre>
 * @param aXOffset An integer value specifying the horizontal offset in
 * print head dots from the left of the page where the graphic will be printed.
 * @param aWidth An integer value specifying the width in print head dots
 * of the printed graphic. The graphic width will be stretched or shrunk
 * to fit this size. If set to 0, width will scale according to height to
 * maintain aspect ratio.
 * @param aHeight An integer value specifying the height in print head dots
 * of the printed graphic. The graphic height will be stretched or shrunk
 * to fit this size. If set to 0, height will scale according to width to
 * maintain aspect ratio.
 *
 * @code
 * //Example usage
 * NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
 * NSString *graphicFile = [resourcePath
 * stringByAppendingPathComponent:@"image.png"];
 *
 * //Scaling the image to width of 200 dots
 * //Setting height to zero in this case to maintain aspect ratio
 * [lp writeGraphic:graphicFile rotation:DEGREE_0 xOffset:0 width:200 height:0];
 * @endcode
 *
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)writeGraphic:(NSString *)aGraphicFilePath
            rotation:(NSInteger)aRotation
             xOffset:(NSInteger)aXOffset
               width:(NSInteger)aWidth
              height:(NSInteger)aHeight;

/**
 * Prints the specified Base64 graphic file. The recommended graphic file format
 * is black and white monochrome BMP or PNG. The result of other graphic file
 * formats is not guaranteed. The graphic can be rotated and stretched to
 * fit the specified width and height.
 * <p>
 * The unit of measurement for the <code>aXOffset</code>, <code>aWidth</code>,
 * and <code>aHeight</code> parameters is print head dots. Please refer to
 * the specific printer User Manual for the media width in print head dots.
 * For the PR2 printers, the default is 384 dots.
 * @param aBase64Image Base64 encoded image data to
 * be printed saved in a NSString.
 * @param aRotation An integer value specifying the degrees to rotate the
 * image. Valid values are defined in <code>LinePrinter.h</code>.
 * <pre>
 * DEGREE_0
 * DEGREE_90
 * DEGREE_180
 * DEGREE_270
 * </pre>
 * @param aXOffset An integer value specifying the horizontal offset in
 * print head dots from the left of the page where the graphic will be printed.
 * @param aWidth An integer value specifying the width in print head dots
 * of the printed graphic. The graphic width will be stretched or shrunk
 * to fit this size. If set to 0, width will scale according to height to
 * maintain aspect ratio.
 * @param aHeight An integer value specifying the height in print head dots
 * of the printed graphic. The graphic height will be stretched or shrunk
 * to fit this size. If set to 0, height will scale according to width to
 * maintain aspect ratio.
 * @code
 * //Example usage
 * //A base64 encoded image.
 * NSString *data = @"iVBORw0......";
 *
 * //Scaling the image to width of 200 dots
 * //Setting height to zero in this case to maintain aspect ratio
 * //Rotate image by 180 degrees.
 * [lp writeGraphicBase64:data rotation:DEGREE_180 xOffset:0 width:200
 * height:0];
 * @endcode
 *
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)writeGraphicBase64:(NSString *)aBase64Image
                  rotation:(NSInteger)aRotation
                   xOffset:(NSInteger)aXOffset
                     width:(NSInteger)aWidth
                    height:(NSInteger)aHeight;

/**
 * Writes a string to the internal print buffer, followed by a call to
 * <code>newLine:1</code>. The string is converted to ASCII character
 * data before writing to the buffer. This method also flushes the data
 * in the internal buffer because it calls the <code>newLine:1</code>
 * method internally.
 * @param string A NSString to be written to the internal print buffer.
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (void)writeLine:(NSString *)string;

/**
 * Gets data from magnetic strip reader of printers. This is a blocking function
 * and returns 0 if there is data read before timeout period or -1 if no data is
 * available after timeout seconds. Returns -2 if MSR data length exceeds
 * trackMaxLen. numRead will be equal to trackMaxLen. trackMaxLen amount
 * of MSR data will be copied to track array.Incoming data is copied
 * from memory into the byte array track. Memory used to store the incoming MSR
 * data is not automatically cleared when this function returns. Use clearMSR
 * function to clear this memory. If not cleared, subsequent readMSR function
 * calls will definitely return 0 with existing data being copied into track
 * byte array.
 * @param track Byte[] A Byte array to store the MSR data
 * @param trackMaxLen NSInteger specifying the maximum size of track array
 * @param numRead NSInteger* Stores the length of data copied to track array
 * @param timeoutInSeconds NSInteger timeout in seconds
 * @returns A NSInteger of 0 if data available. -1 if no data after timeout.
 * -2 if data exceeds trackMaxLen
 * @exception ITCLinePrinterException if an unexpected error occurs or if an
 * active connection to the printer does not exist.
 */
- (NSInteger)readMSR:(Byte *)track
         trackMaxLen:(NSInteger)maxLen
             numRead:(NSInteger *)numReadPtr
    timeoutInSeconds:(NSInteger)timeout;

/**
 * Clears the MSR input buffer.
 */
- (void)clearMSR;
@end
