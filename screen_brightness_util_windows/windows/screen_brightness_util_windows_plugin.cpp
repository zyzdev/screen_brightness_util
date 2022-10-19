#include "screen_brightness_util_windows_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

#include <highlevelmonitorconfigurationapi.h>


using flutter::MethodChannel;

using flutter::EncodableMap;
using flutter::EncodableValue;


namespace screen_brightness_util_windows {

	std::unique_ptr<
		flutter::MethodChannel<flutter::EncodableValue>,
		std::default_delete<flutter::MethodChannel<flutter::EncodableValue>>>
		channel = nullptr;
	std::unique_ptr<ScreenBrightnessUtilWindowsPlugin>  plugin = nullptr;

	// static
	void ScreenBrightnessUtilWindowsPlugin::RegisterWithRegistrar(
		flutter::PluginRegistrarWindows* registrar) {
	
		channel =
			std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
				registrar->messenger(), "screen_brightness_util",
				&flutter::StandardMethodCodec::GetInstance());
		plugin = std::make_unique<ScreenBrightnessUtilWindowsPlugin>(registrar);

		channel->SetMethodCallHandler(
			[plugin_pointer = plugin.get()](const auto& call, auto result) {
				plugin_pointer->HandleMethodCall(call, std::move(result));
			});
	
		registrar->AddPlugin(std::move(plugin));
	}

	ScreenBrightnessUtilWindowsPlugin::ScreenBrightnessUtilWindowsPlugin(flutter::PluginRegistrarWindows* registrar) : registrar(registrar) {}

	ScreenBrightnessUtilWindowsPlugin::~ScreenBrightnessUtilWindowsPlugin() {}

	void ScreenBrightnessUtilWindowsPlugin::HandleMethodCall(
		const flutter::MethodCall<flutter::EncodableValue>& method_call,
		std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
		std::string method = method_call.method_name();
	
		//std::cout << "onMethodCall, method:" << method << std::endl;

		if (method_call.method_name().compare("getPlatformVersion") == 0) {
			result->Success(flutter::EncodableValue("10"));
		}
		else if (method.compare("brightness#get") == 0) {
			result->Success(flutter::EncodableValue(getBrightness()));
		}
		else if (method.compare("brightness#set") == 0) {
			auto brightness = *std::get_if<double>(method_call.arguments());
						
			std::cout << "onMethodCall, brightness:" << brightness << std::endl;
			result->Success(flutter::EncodableValue(setBrightness(brightness)));
		}
		else {
			result->NotImplemented();
		}
	}


	float ScreenBrightnessUtilWindowsPlugin::getBrightness() {

		HMONITOR hMonitor = NULL;
		DWORD cPhysicalMonitors;
		LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;
		HWND hWnd = ::GetAncestor(registrar->GetView()->GetNativeWindow(), GA_ROOT);

		// Get the monitor handle.
		hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
		// Get the number of physical monitors.

		BOOL bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(
			hMonitor,
			&cPhysicalMonitors
		);

		if (bSuccess)
		{
			// Allocate the array of PHYSICAL_MONITOR structures.
			pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(cPhysicalMonitors * sizeof(PHYSICAL_MONITOR));

			// Get the array.
			bSuccess = GetPhysicalMonitorsFromHMONITOR(
				hMonitor, cPhysicalMonitors, pPhysicalMonitors);

			// Use the monitor handles (not shown).

			HANDLE pmh = pPhysicalMonitors[0].hPhysicalMonitor;
			//SetMonitorBrightness(pmh, 80);
			DWORD  pdwMinimumBrightness = 0;
			DWORD  pdwCurrentBrightness = 0;
			DWORD  pdwMaximumBrightness = 0;

			GetMonitorBrightness(pmh, &pdwMinimumBrightness, &pdwCurrentBrightness, &pdwMaximumBrightness);
			DWORD pdwRangeBrightness = pdwMaximumBrightness - pdwMinimumBrightness;
			if (pdwRangeBrightness <= 0) return -1.0;
			float appScreenBrightness = (pdwCurrentBrightness - pdwMinimumBrightness) / (float)pdwRangeBrightness;
			//std::cout << "pdwMinimumBrightness:" << pdwMinimumBrightness << " pdwCurrentBrightness:" << pdwCurrentBrightness << " pdwMaximumBrightness:" << pdwMaximumBrightness << std::endl;

			// Close the monitor handles.
			bSuccess = DestroyPhysicalMonitors(
				cPhysicalMonitors,
				pPhysicalMonitors);

			// Free the array.
			free(pPhysicalMonitors);
			return appScreenBrightness;
		}
		return -1.0;
	}

	bool ScreenBrightnessUtilWindowsPlugin::setBrightness(const double brightness) {

		HMONITOR hMonitor = NULL;
		DWORD cPhysicalMonitors;
		LPPHYSICAL_MONITOR pPhysicalMonitors = NULL;
		HWND hWnd = ::GetAncestor(registrar->GetView()->GetNativeWindow(), GA_ROOT);

		// Get the monitor handle.
		hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
		// Get the number of physical monitors.

		BOOL bSuccess = GetNumberOfPhysicalMonitorsFromHMONITOR(
			hMonitor,
			&cPhysicalMonitors
		);

		if (bSuccess)
		{
			// Allocate the array of PHYSICAL_MONITOR structures.
			pPhysicalMonitors = (LPPHYSICAL_MONITOR)malloc(cPhysicalMonitors * sizeof(PHYSICAL_MONITOR));


			// Get the array.
			bSuccess = GetPhysicalMonitorsFromHMONITOR(
				hMonitor, cPhysicalMonitors, pPhysicalMonitors);

			// Use the monitor handles (not shown).

			HANDLE pmh = pPhysicalMonitors[0].hPhysicalMonitor;
		
			DWORD  pdwMinimumBrightness = 0;
			DWORD  pdwCurrentBrightness = 0;
			DWORD  pdwMaximumBrightness = 0;
			GetMonitorBrightness(pmh, &pdwMinimumBrightness, &pdwCurrentBrightness, &pdwMaximumBrightness);
			DWORD pdwRangeBrightness = pdwMaximumBrightness - pdwMinimumBrightness;
			DWORD brightnessFinal = (DWORD)(pdwRangeBrightness * brightness + pdwMinimumBrightness);

			SetMonitorBrightness(pmh, brightnessFinal);
			std::cout << "brightness:" << brightness << ", brightnessFinal:" << brightnessFinal << std::endl;

			// Close the monitor handles.
			bSuccess = DestroyPhysicalMonitors(
				cPhysicalMonitors,
				pPhysicalMonitors);

			// Free the array.
			free(pPhysicalMonitors);
			return true;
		}
		return false;
	}
}  // namespace screen_brightness_util_windows
