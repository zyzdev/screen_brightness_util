import 'dart:async';
import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:screen_brightness_util_platform_interface/src/channel/platform_channel.dart';

class ScreenBrightnessMethodChannel extends PlatformChannel {
  static const MethodChannel _channel = MethodChannel('screen_brightness_util');

  final StreamController<double> _appBrightnessChangeStreamController =
      StreamController<double>.broadcast();

  ScreenBrightnessMethodChannel() : super() {
    _channel.setMethodCallHandler(_handleMethodCall);
    if(Platform.isWindows) _windowsListener();
  }

  void _windowsListener() async {
    double preBrightness = await getBrightness();
    Timer.periodic(const Duration(milliseconds: 500), (timer) async {
      double curBrightness = await getBrightness();
      if(preBrightness != curBrightness) {
        _appBrightnessChangeStreamController.add(curBrightness);
        preBrightness = curBrightness;
      }
    });
  }
  @override
  Future<double> getBrightness() async {
    return (await _channel.invokeMethod<double>("brightness#get"))!;
  }

  @override
  Future<bool> setBrightness(double percent) async {
    return await _channel.invokeMethod("brightness#set", percent);
  }

  @override
  Stream<double> onBrightnessChange() {
    return _appBrightnessChangeStreamController.stream;
  }

  Future<dynamic> _handleMethodCall(MethodCall call) async {
    String method = call.method;
    var args = call.arguments;
    switch (method) {
      case "print#log":
        debugPrint("print#log, info:$args");
        break;
      case "appBrightness#onChange":
        _appBrightnessChangeStreamController.add(args as double);
        break;
      default:
        throw MissingPluginException();
    }
  }
}
