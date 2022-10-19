import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'screen_brightness_util_platform_interface.dart';

/// An implementation of [ScreenBrightnessUtilPlatform] that uses method channels.
class MethodChannelScreenBrightnessUtil extends ScreenBrightnessUtilPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('screen_brightness_util');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
