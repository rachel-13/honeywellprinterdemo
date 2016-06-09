/*
 * ITCPrintProgressEvent.h
 * iOS Printing SDK
 *
 * Created by Teo Boon Keat on 27/11/14.
 * Copyright (c) 2014 Honeywell. All rights reserved.
 */

#import <Foundation/Foundation.h>
/** @file
 * @brief Contains the enumeration PrintProgressEventMessageType
 */

/**
 * This enumeration defines the constants representing the print progress
 * message types.
 * \enum PrintProgressEventMessageType
 */
typedef NS_ENUM(NSInteger, ITCPrintProgressEventMessageType)
{

  /**
   * Unknown message type.
   */
  ITC_PRINT_PROGRESS_EVENT_NONE = 0,

  /**
   * Document printing has begun or data has begun to be sent to printer.
   */
  ITC_PRINT_PROGRESS_EVENT_STARTDOC = 1,

  /**
   * The last line of the document has been sent, but the LinePrinter
   * has not yet disconnected from the printer.
   */
  ITC_PRINT_PROGRESS_EVENT_ENDDOC = 4,

  /**
   * The LinePrinter has determined the printing should be canceled.
   */
  ITC_PRINT_PROGRESS_EVENT_CANCEL = 6,

  /**
   * The LinePrinter has disconnected from the printer without regard to
   * the print status.
   */
  ITC_PRINT_PROGRESS_EVENT_FINISHED = 7,

  /**
   * The document has been completely sent to printer without errors or being
   * cancelled.
   */
  ITC_PRINT_PROGRESS_EVENT_COMPLETE = 8,
};

/**
 * The print progress event is fired by the LinePrinter class during printing to
 * inform the application when the report is started and when the report has
 * been sent to
 * printer.
 * @brief This class contains the print progress event data.
 * @see PrintProgressListener
 * @see PrintProgressEventMessageType
 */
@interface ITCPrintProgressEvent : NSObject

//\cond
@property ITCPrintProgressEventMessageType m_iMsgType;
//\endcond

/**
 * Gets the type of progress message that the LinePrinter is notifying
 * the application of. The return value is one of the constants defined
 * in PrintProgressEventMessageType.
 * @brief Returns the progress message LinePrinter is trying to notify
 * application of.
 * @return The type of progress message that the LinePrinter is notifying
 * the application of.
 * @see PrintProgressEventMessageType
 */
- (ITCPrintProgressEventMessageType)getMessageType;

@end
