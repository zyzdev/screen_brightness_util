#import "ScreenBrightnessUtilIosPlugin.h"
#if __has_include(<screen_brightness_util_ios/screen_brightness_util_ios-Swift.h>)
#import <screen_brightness_util_ios/screen_brightness_util_ios-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "screen_brightness_util_ios-Swift.h"
#endif

@implementation ScreenBrightnessUtilIosPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftScreenBrightnessUtilIosPlugin registerWithRegistrar:registrar];
}
@end
