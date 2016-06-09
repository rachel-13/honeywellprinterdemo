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

NSString * PRINTER_IP_KEY = @"PRINTER_IP_ADDRESS";

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self registerDefaultsFromSettingsBundle];
    
    NSString * printerIP = [[NSUserDefaults standardUserDefaults] objectForKey:PRINTER_IP_KEY];
    
    printer = [[HoneywellPrinterUtilities alloc]init];
    [printer initNetworkCommunication:printerIP port:9100];
    
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

#pragma mark ibactions

- (IBAction)didPressPrint:(id)sender {
   
    NSMutableDictionary * dic = [[NSMutableDictionary alloc]init];
    [dic setObject:_inputTextField.text forKey:HONEYWELLPRT_KEY_BARCODE_INPUT];
    [dic setObject:_barCodeTypeTextField.text forKey:HONEYWELLPRT_KEY_BARCODETYPE_CODE];
    [dic setObject:@"A&W Orange-Strawberry-Kiwi-Grapefruit Flavour 250ml Can" forKey:HONEYWELLPRT_KEY_ITEM_DESC];
    [dic setObject:@"RM28080.88" forKey:HONEYWELLPRT_KEY_ITEM_PRICE];
    
    [printer printDataOn50x30mmLabel:dic templateType:FOOD_INFO_LABEL];

}

- (IBAction)didPressChangePrinterIP:(id)sender {
    
    UIAlertController * alert = [UIAlertController alertControllerWithTitle:@"Change Printer IP" message:@"" preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction * okAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        
        NSLog(@"Printer address change OK");
        
        NSString * newPrinterIP = alert.textFields[0].text;
        [printer closeNetworkConnection];
        [printer initNetworkCommunication:newPrinterIP port:9100];
        
        [[NSUserDefaults standardUserDefaults] setObject:newPrinterIP forKey:PRINTER_IP_KEY];
        
    }];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil];
    
    [alert addAction:okAction];
    [alert addAction:cancelAction];
    
    [alert addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
       
        NSString * currentPrinterIP = [[NSUserDefaults standardUserDefaults]objectForKey:PRINTER_IP_KEY];
        textField.text = currentPrinterIP;
    }];
    
    
    [self showViewController:alert sender:nil];
    
}

#pragma mark picker delegates

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

// Capture the picker view selection
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    _barCodeTypeTextField.text = barCodeTypeArray[row];
}

#pragma mark general functions

- (void)registerDefaultsFromSettingsBundle
{
    NSString *settingsBundle = [[NSBundle mainBundle] pathForResource:@"Settings" ofType:@"bundle"];
    if(!settingsBundle)
    {
        //NSLog(@"Could not find Settings.bundle");
        return;
    }
    
    NSDictionary *settings = [NSDictionary dictionaryWithContentsOfFile:[settingsBundle stringByAppendingPathComponent:@"Root.plist"]];
    NSArray *preferences = [settings objectForKey:@"PreferenceSpecifiers"];
    
    NSMutableDictionary *defaultsToRegister = [[NSMutableDictionary alloc] initWithCapacity:[preferences count]];
    for(NSDictionary *prefSpecification in preferences)
    {
        NSString *key = [prefSpecification objectForKey:@"Key"];
        if(key)
        {
            [defaultsToRegister setObject:[prefSpecification objectForKey:@"DefaultValue"] forKey:key];
        }
    }
    
    [[NSUserDefaults standardUserDefaults] registerDefaults:defaultsToRegister];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
