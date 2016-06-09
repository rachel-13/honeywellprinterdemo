/*
 * ITCPrintProgressListener.h
 * IOS Printing SDK
 *
 * Created by Boon Keat on 26/11/14.
 * Copyright (c) 2014 Honeywell. All rights reserved.
 */

#import "ITCPrintProgressEvent.h"

/**
 * The listener interface for receiving print progress events. These events
 * inform the application of when the report is started and when the printer
 * is done printing the report.
 *
 * The class that is interested in processing the print progress events should
 * implement this interface and register itself with a LinePrinter
 * object via its addPrintProgressListener method.
 *
 * @brief Class for processing print progress events.
 *
 * Your interface file:
 * @code
 * // YourClass.h
 * #import "printersdk/LinePrinter.h"
 *
 * @interface YourClass <PrintProgressDelegate>
 *
 * @end
 * @endcode
 * Your implementation file:
 *
 * @code
 * // YourClass.m
 * @implementation YourClass
 *
 * - (void) yourInitMethod
 * {
 *       //lp is an instance of the LinePrinter class
 *       PrintProgressListener *myListener = [[PrintProgressListener alloc]
 *init];
 *       myListener.delegate = self;
 *       [lp addPrintProgressListener:myListener]; //add listener to your
 *LinePrinter instance.
 * }
 *
 *
 * - (void) receivedStatus:(PrintProgressEvent *)message
 * {
 *   switch ([message getMessageType])
 *   {
 *       case PRINT_PROGRESS_EVENT_STARTDOC:
 *           //Display "Start printing document"
 *           break;
 *       case PRINT_PROGRESS_EVENT_ENDDOC:
 *           //Display "Document end"
 *           break;
 *       case PRINT_PROGRESS_EVENT_FINISHED:
 *           //Display "Document finished"
 *           break;
 *       case PRINT_PROGRESS_EVENT_COMPLETE:
 *           //Display "Document Complete"
 *           break;
 *       case PRINT_PROGRESS_EVENT_CANCEL:
 *           //Display "Document cancelled"
 *           break;
 *       default:
 *           //Display "Unknown message" or do nothing.
 *           break;
 *   }
 * }
 * @end
 * @endcode
 * @see PrintProgressEvent
 */
@interface ITCPrintProgressListener : NSObject
//\cond
@property (nonatomic, assign) id delegate;

- (void)callDelegate:(int)typeFromCPCL;
//\endcond
@end

@class ITCPrintProgressListener;

/**
 * The protocol that must be implemented to receive print progress events.
 * These events inform the application of when the report is started and when
 * the printer is done printing the report.
 * @brief The protocol to implement to receive print progress events.
 */
@protocol PrintProgressDelegate <NSObject>

/**
 * the message
 * The delegate method for receiving print progress events. These events
 * inform the application of when the report is started and when the report has
 * been sent to the printer.
 * @brief The delegate method that is called when a PrintProgressEvent fires.
 */
@optional
- (void)receivedStatus:(ITCPrintProgressEvent *)message;

@end
