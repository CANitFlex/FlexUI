package com.btb.AndroidCanController;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;

import org.qtproject.qt.android.QtNative;

public class GamepadBridge {
    private static final String TAG = "GamepadBridge";

    // Load the native library
    static {
        Log.d(TAG, "Native library loaded successfully");
    }

    // Native method declarations
    public static native void nativeTestJNI();
    public static native void nativeOnGamepadButton(int keyCode, boolean down);
    public static native void nativeOnGamepadJoystick(float lx, float ly, float rx, float ry);

    public static void init() {
        Log.d(TAG, "GamepadHelper initialized");

        // Test the JNI connection
        try {
            nativeTestJNI();
            Log.d(TAG, "JNI test successful");
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "JNI test failed: " + e.getMessage());
        }

        // Check for connected gamepads
        checkForGamepads();
    }

    public static void checkForGamepads() {
        int[] deviceIds = InputDevice.getDeviceIds();
        Log.d(TAG, "Checking " + deviceIds.length + " input devices");

        for (int deviceId : deviceIds) {
            InputDevice device = InputDevice.getDevice(deviceId);
            if (device != null) {
                int sources = device.getSources();
                Log.d(TAG, "Device: " + device.getName() + " Sources: " + sources);

                if (((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD) ||
                    ((sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK)) {
                    Log.d(TAG, "Found gamepad: " + device.getName());
                    Log.d(TAG, "I am here ");
                }
            }
        }
    }

    public static boolean handleKeyEvent(KeyEvent event, boolean isDown) {
        Log.d(TAG, "handleKeyEvent: keyCode=" + event.getKeyCode() + " source=" + event.getSource() + " down=" + isDown);

        if (isGamepadEvent(event)) {
            Log.d(TAG, "Gamepad button " + (isDown ? "down" : "up") + ": " + event.getKeyCode());
            nativeOnGamepadButton(event.getKeyCode(), isDown);
            return true;
        }
        return false;
    }

    public static boolean handleMotionEvent(MotionEvent event) {
        Log.d(TAG, "handleMotionEvent: source=" + event.getSource() + " action=" + event.getAction());

        if ((event.getSource() & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK &&
            event.getAction() == MotionEvent.ACTION_MOVE) {

            // Get joystick values
            float lx = getCenteredAxis(event, MotionEvent.AXIS_X);
            float ly = getCenteredAxis(event, MotionEvent.AXIS_Y);
            float rx = getCenteredAxis(event, MotionEvent.AXIS_Z);
            float ry = getCenteredAxis(event, MotionEvent.AXIS_RZ);

            // Only log if there's actual movement to avoid spam
            if (lx != 0 || ly != 0 || rx != 0 || ry != 0) {
                Log.d(TAG, "Joystick moved - LX:" + lx + " LY:" + ly + " RX:" + rx + " RY:" + ry);
                nativeOnGamepadJoystick(lx, ly, rx, ry);
            }
            return true;
        }
        return false;
    }

    private static float getCenteredAxis(MotionEvent event, int axis) {
        final InputDevice device = event.getDevice();
        if (device == null) return 0;

        final InputDevice.MotionRange range = device.getMotionRange(axis, event.getSource());
        if (range != null) {
            final float flat = range.getFlat();
            final float value = event.getAxisValue(axis);
            if (Math.abs(value) > flat) {
                return value;
            }
        }
        return 0;
    }

    private static boolean isGamepadEvent(KeyEvent event) {
        int source = event.getSource();
        return (source & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD ||
               (source & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK;
    }

    private static Activity getQtActivity() {
        try {
            Class<?> qtNative = Class.forName("org.qtproject.qt.android.QtNative");
            java.lang.reflect.Method m = qtNative.getDeclaredMethod("activity");
            m.setAccessible(true);
            Object ret = m.invoke(null);
            if (ret instanceof Activity) return (Activity) ret;
        } catch (Exception e) {
            Log.e(TAG, "getQtActivity failed", e);
        }
        return null;
    }


    public static void setLandscape() {
        final Activity activity = getQtActivity();
        if (activity != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
                }
            });
        }
    }

        public static void setPortrait() {
        final Activity activity = getQtActivity();
        if (activity != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
                }
            });
        }
    }

    public static void setSensor() {
        final Activity activity = getQtActivity();
        if (activity != null) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
                }
            });
        }
    }
}
