import 'dart:async';
import 'dart:ui';

import 'package:screen_brightness_util_platform_interface/screen_brightness_util_platform_interface.dart';

class ScreenBrightnessUtil {

  PlatformChannel get _platformChannel => PlatformChannel.instance;

  /// return current brightness
  /// range 0 <= brightness <= 1, -1 means something wrong
  /// `Windows`, return the brightness of the screen that the app window located.
  Future<double> getBrightness() async {
    return _platformChannel.getBrightness();
  }

  /// return true means success, otherwise false
  /// `Windows`, set the brightness of the screen that the app window located.
  Future<bool> setBrightness(double brightness) async {
    return _platformChannel.setBrightness(brightness);
  }

  /// brightness listener and return brightness was changed
  /// range 0 <= brightness <= 1, -1 means something wrong
  /// `Windows`, get the brightness of the screen the app window located
  Stream<double> getBrightnessChangeStream() =>
      _platformChannel.onBrightnessChange();
}
