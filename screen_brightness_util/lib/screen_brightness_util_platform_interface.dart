import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'screen_brightness_util_method_channel.dart';

abstract class ScreenBrightnessUtilPlatform extends PlatformInterface {
  /// Constructs a ScreenBrightnessUtilPlatform.
  ScreenBrightnessUtilPlatform() : super(token: _token);

  static final Object _token = Object();

  static ScreenBrightnessUtilPlatform _instance = MethodChannelScreenBrightnessUtil();

  /// The default instance of [ScreenBrightnessUtilPlatform] to use.
  ///
  /// Defaults to [MethodChannelScreenBrightnessUtil].
  static ScreenBrightnessUtilPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [ScreenBrightnessUtilPlatform] when
  /// they register themselves.
  static set instance(ScreenBrightnessUtilPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
