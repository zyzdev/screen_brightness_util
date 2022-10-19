import Flutter
import UIKit

public class SwiftScreenBrightnessUtilIosPlugin: NSObject, FlutterPlugin {
    private let DEBUG = true
    
    private var methodChannel: FlutterMethodChannel
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "screen_brightness_util", binaryMessenger: registrar.messenger())
        let instance = SwiftScreenBrightnessUtilIosPlugin(methodChannel: channel)
        registrar.addMethodCallDelegate(instance, channel: channel)
        instance.listenBrightnessChange()
    }
    
    init(
        methodChannel: FlutterMethodChannel
    ) {
        self.methodChannel = methodChannel
    }

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        let method = call.method
        let args = call.arguments
        // debug(method, true)
        switch method {
        case "brightness#get":
            result(getBrightness())
        case "brightness#set":
            result(setBrightness(args))
            methodChannel.invokeMethod("appBrightness#onChange", arguments: args)
        default:
            result(FlutterMethodNotImplemented)
        }
    }
    
    func getBrightness() -> Double {
        return UIScreen.main.brightness
    }
    
    func setBrightness(_ args: Any?) -> Bool {
        UIScreen.main.brightness = args as! CGFloat
        return true
    }
    
    func listenBrightnessChange() {
        let noteCenter = NotificationCenter.default
        noteCenter.addObserver(self,
                               selector: #selector(brightnessDidChange),
                               name: UIScreen.brightnessDidChangeNotification,
                               object: nil)
    }

    @objc private func brightnessDidChange() {
        methodChannel.invokeMethod("appBrightness#onChange", arguments: UIScreen.main.brightness)
    }
}

extension SwiftScreenBrightnessUtilIosPlugin {
    private func isDouble(_ args: Any?) -> Bool {
        return args is Bool
    }
    
    private func toS(_ arg: Any?) -> String {
        return arg != nil ? String(describing: arg) : ""
    }
    
    private func debug(_ log: Any?, _ debug: Bool = false) {
        if !DEBUG, !debug { return }
        methodChannel.invokeMethod("print#log", arguments: "iOS flutter_google_street_view: \(log is String ? log as! String : toS(log))")
    }
}
