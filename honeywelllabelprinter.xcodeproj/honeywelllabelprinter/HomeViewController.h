//
//  HomeViewController.h
//  honeywelllabeprinter
//
//  Created by pohyee on 25/05/2016.
//  Copyright © 2016 ritebozz. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HomeViewController : UIViewController<NSStreamDelegate,UIPickerViewDelegate,UIPickerViewDataSource>

@property (weak, nonatomic) IBOutlet UITextField *inputTextField;
@property (weak, nonatomic) IBOutlet UIButton *printButton;
@property (weak, nonatomic) IBOutlet UITextField *barCodeTypeTextField;
@property (weak, nonatomic) IBOutlet UIPickerView *barCodeTypePicker;

- (IBAction)didPressPrint:(id)sender;
@end

