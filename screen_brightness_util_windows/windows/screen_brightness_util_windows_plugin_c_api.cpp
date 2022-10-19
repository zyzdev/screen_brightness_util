#include "include/screen_brightness_util_windows/screen_brightness_util_windows_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "screen_brightness_util_windows_plugin.h"

void ScreenBrightnessUtilWindowsPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  screen_brightness_util_windows::ScreenBrightnessUtilWindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
