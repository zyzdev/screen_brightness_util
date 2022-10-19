package zyz.flutter.plugin.screen_brightness_util_android

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.database.ContentObserver
import android.net.Uri
import android.os.Build
import android.os.Handler
import android.os.Looper
import android.provider.Settings
import android.view.Window
import android.widget.Toast
import androidx.annotation.NonNull
import androidx.databinding.Observable
import androidx.databinding.ObservableFloat
import androidx.lifecycle.DefaultLifecycleObserver
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.embedding.engine.plugins.activity.ActivityAware
import io.flutter.embedding.engine.plugins.activity.ActivityPluginBinding
import io.flutter.embedding.engine.plugins.lifecycle.HiddenLifecycleReference
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.MethodCallHandler
import io.flutter.plugin.common.MethodChannel.Result
import io.flutter.plugin.common.PluginRegistry

/** ScreenBrightnessUtilPlugin */
class ScreenBrightnessUtilAndroidPlugin : FlutterPlugin, MethodCallHandler, ActivityAware {

    companion object {
        private const val MAXIMUM_BRIGHTNESS_VALUE: Float = 255F
    }

    private val dTag: String = javaClass.simpleName

    private lateinit var channel: MethodChannel

    private lateinit var applicationContext: Context
    private lateinit var activity: Activity
    private val defScreenBrightness: Float by lazy {
        getBrightness()
    }
    private val appBrightnessObservable: ObservableFloat by lazy {
        ObservableFloat(getBrightness()).apply {
            this.addOnPropertyChangedCallback(object :
                Observable.OnPropertyChangedCallback() {
                override fun onPropertyChanged(
                    sender: Observable?,
                    propertyId: Int
                ) {
                    val brightness = appBrightnessObservable.get()
                    channel.invokeMethod("appBrightness#onChange", brightness)
                }
            })
        }
    }

    override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
        applicationContext = flutterPluginBinding.applicationContext
        channel = MethodChannel(flutterPluginBinding.binaryMessenger, "screen_brightness_util")
        channel.setMethodCallHandler(this)
    }

    override fun onDetachedFromEngine(@NonNull binding: FlutterPlugin.FlutterPluginBinding) {
        channel.setMethodCallHandler(null)
    }

    override fun onAttachedToActivity(binding: ActivityPluginBinding) {
        activity = binding.activity
        init()
    }

    override fun onDetachedFromActivityForConfigChanges() {
    }

    override fun onReattachedToActivityForConfigChanges(binding: ActivityPluginBinding) {

    }

    override fun onDetachedFromActivity() {

    }

    override fun onMethodCall(@NonNull call: MethodCall, @NonNull result: Result) {
        val method = call.method
        val args = call.arguments
        //printLog("onMethodCall, method:$method, args:$args")
        when (method) {
            "brightness#get" -> result.success(getBrightness())
            "brightness#set" -> result.success(setBrightness((args as Double).toFloat()))
            else -> result.notImplemented()
        }
    }

    private fun init() {
        defScreenBrightness
        appBrightnessObservable
    }

    private fun getBrightness(): Float {
        val localWindow: Window = activity.window
        val localLayoutParams = localWindow.attributes
        val appScreenBrightness = localLayoutParams.screenBrightness
        return if (appScreenBrightness < 0) Settings.System.getInt(
            applicationContext.contentResolver,
            Settings.System.SCREEN_BRIGHTNESS
        ) / MAXIMUM_BRIGHTNESS_VALUE
        else appScreenBrightness
    }

    private fun setBrightness(percent: Float): Boolean {
        val brightness: Float = percent
        val localWindow: Window = activity.window
        val localLayoutParams = localWindow.attributes
        localLayoutParams.screenBrightness = brightness
        localWindow.attributes = localLayoutParams
        appBrightnessObservable.set(percent)
        return true
    }

    private fun printLog(info: Any) {
        channel.invokeMethod("print#log", "$dTag, $info")
    }
}
