# screen_brightness_util

A flutter plugin to config and monitor brightness.

## Getting Started

### Get Brightness
```dart
final ScreenBrightnessUtil _screenBrightnessUtil = ScreenBrightnessUtil();
double brightness = await _screenBrightnessUtil.getBrightness();
if(brightness == -1) {
  debugPrint("Oops... something wrong!");
}
```

### Set Brightness
```dart
double brightness = 0.5;
final ScreenBrightnessUtil _screenBrightnessUtil = ScreenBrightnessUtil();
bool success = await _screenBrightnessUtil.setBrightness(brightness);
```
### Listen Brightness Change
```dart
final ScreenBrightnessUtil _screenBrightnessUtil = ScreenBrightnessUtil();
StreamSubscription ss = _screenBrightnessUtil.getBrightnessChangeStream().listen((brightness) {
  debugPrint("brightness:$brightness");
});

...

ss.cancel();
```
