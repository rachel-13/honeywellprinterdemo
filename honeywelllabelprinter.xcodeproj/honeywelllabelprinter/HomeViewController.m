//
//  ViewController.m
//  honeywelllabeprinter
//
//  Created by pohyee on 25/05/2016.
//  Copyright © 2016 ritebozz. All rights reserved.
//

#import "HomeViewController.h"
#import <ExternalAccessory/EAAccessoryManager.h>
#import <printersdk/ITCLinePrinter.h>
#import <printersdk/ITCLinePrinterException.h>
#import "HoneywellPrinterUtilities.h"

@interface HomeViewController ()
{
    NSInputStream *inputStream;
    NSOutputStream *outputStream;
    HoneywellPrinterUtilities * printer;
    NSMutableArray * barCodeTypeArray;
}
@end

@implementation HomeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    printer = [[HoneywellPrinterUtilities alloc]init];
    [printer initNetworkCommunication:@"192.168.0.164" port:9100];
    
    barCodeTypeArray = [[NSMutableArray alloc]init];
    [barCodeTypeArray addObject:@"EAN8"];
    [barCodeTypeArray addObject:@"EAN8_CC"];
    [barCodeTypeArray addObject:@"EAN13"];
    [barCodeTypeArray addObject:@"EAN13_CC"];
    [barCodeTypeArray addObject:@"EAN128"];
    [barCodeTypeArray addObject:@"EAN128A"];
    [barCodeTypeArray addObject:@"EAN128B"];
    [barCodeTypeArray addObject:@"EAN128C"];
    [barCodeTypeArray addObject:@"EAN128_CCAB"];
    [barCodeTypeArray addObject:@"EAN128_CCC"];
    [barCodeTypeArray addObject:@"UPCA"];
    [barCodeTypeArray addObject:@"UPCA_CC"];
    [barCodeTypeArray addObject:@"UPCD1"];
    [barCodeTypeArray addObject:@"UPCD2"];
    [barCodeTypeArray addObject:@"UPCD3"];
    [barCodeTypeArray addObject:@"UPCD4"];
    [barCodeTypeArray addObject:@"UPCD5"];
    [barCodeTypeArray addObject:@"UPCE"];
    [barCodeTypeArray addObject:@"UPCE-CC"];
    [barCodeTypeArray addObject:@"UPCSCC"];
    
    [_barCodeTypeTextField setUserInteractionEnabled:NO];
}


- (IBAction)didPressPrint:(id)sender {
   
    NSMutableDictionary * dic = [[NSMutableDictionary alloc]init];
    [dic setObject:_inputTextField.text forKey:@"input"];
    [dic setObject:_barCodeTypeTextField.text forKey:@"barcode"];
    
    [printer printData:dic];

}

// The number of columns of data
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

// The number of rows of data
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return barCodeTypeArray.count;
}

// The data to return for the row and component (column) that's being passed in
- (NSString*)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return barCodeTypeArray[row];
}

// Catpure the picker view selection
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    _barCodeTypeTextField.text = barCodeTypeArray[row];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
