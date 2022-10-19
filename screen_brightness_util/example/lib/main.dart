import 'dart:async';

import 'package:flutter/material.dart';
import 'package:screen_brightness_util/screen_brightness_util.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  late final ScreenBrightnessUtil _screenBrightnessUtil =
      ScreenBrightnessUtil();
  late StreamSubscription ss;
  String _info = "";
  double _sliderBrightness = 0;

  @override
  void initState() {
    ss = _screenBrightnessUtil.getBrightnessChangeStream().listen((brightness) {
      _updateInfo(brightness);
    });
    _screenBrightnessUtil.getBrightness().then((brightness) {
      _updateInfo(brightness);
    });

    super.initState();
  }

  @override
  void dispose() {
    ss.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Column(
          mainAxisSize: MainAxisSize.min,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const SizedBox(
              height: 36,
            ),
            Row(
              children: [
                const SizedBox(
                  width: 24,
                ),
                Text(
                  _info,
                  style: const TextStyle(fontSize: 16),
                ),
              ],
            ),
            const SizedBox(
              height: 16,
            ),
            Slider(
                value: _sliderBrightness,
                onChanged: (setBrightness) {
                  _screenBrightnessUtil.setBrightness(setBrightness);
                  setState(() {
                    _sliderBrightness = setBrightness;
                  });
                }),
          ],
        ),
      ),
    );
  }

  void _updateInfo(double brightness) {
    setState(() {
      bool error = brightness < 0;
      if (error) {
        _info = "OOPS, something wrong!";
        return;
      } else {
        _info = "Brightness: $brightness";
      }
      _sliderBrightness = brightness;
    });
  }
}
