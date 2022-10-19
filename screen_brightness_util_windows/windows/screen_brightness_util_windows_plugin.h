#ifndef FLUTTER_PLUGIN_SCREEN_BRIGHTNESS_UTIL_WINDOWS_PLUGIN_H_
#define FLUTTER_PLUGIN_SCREEN_BRIGHTNESS_UTIL_WINDOWS_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace screen_brightness_util_windows {

class ScreenBrightnessUtilWindowsPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  ScreenBrightnessUtilWindowsPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~ScreenBrightnessUtilWindowsPlugin();

  // Disallow copy and assign.
  ScreenBrightnessUtilWindowsPlugin(const ScreenBrightnessUtilWindowsPlugin&) = delete;
  ScreenBrightnessUtilWindowsPlugin& operator=(const ScreenBrightnessUtilWindowsPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  
  float getBrightness();
  bool setBrightness(const double brightness);
  flutter::PluginRegistrarWindows* registrar;
};

}  // namespace screen_brightness_util_windows

#endif  // FLUTTER_PLUGIN_SCREEN_BRIGHTNESS_UTIL_WINDOWS_PLUGIN_H_
