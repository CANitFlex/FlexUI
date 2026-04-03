package com.btb.AndroidCanController;

import android.view.KeyEvent;
import android.view.MotionEvent;
import com.btb.AndroidCanController.GamepadBridge;

public class QtActivity extends org.qtproject.qt.android.bindings.QtActivity {
    
    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        // Forward motion events to GamepadBridge
        if (GamepadBridge.handleMotionEvent(event)) {
            return true;
        }
        return super.onGenericMotionEvent(event);
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (GamepadBridge.handleKeyEvent(event, true)) {
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (GamepadBridge.handleKeyEvent(event, false)) {
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }
}