package com.rawr.remotemouse_client;

import com.rawr.remotemouse_client.SocketService.SocketBinder;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
//import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
//import java.util.concurrent.ThreadPoolExecutor;

public class TrackpadActivity extends Activity implements View.OnTouchListener {
	SocketService m_socketService = null;
	TrackpadView m_trackpadView = null;
	boolean m_isBound= false;
	ExecutorService executor = Executors.newCachedThreadPool();
    double m_firstX;
    double m_firstY;
    double m_lastX;
    double m_lastY;
//    double m_accel = 2.5;

    @Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_trackpad);
		
		m_trackpadView = (TrackpadView)findViewById(R.id.trackpadView);
//		m_trackpadView.setCallback(m_trackpadCallback);

        m_trackpadView.setOnTouchListener(this);
		Intent intent = new Intent(this, SocketService.class);
		bindService(intent, m_conn, Context.BIND_AUTO_CREATE);
	}
	
	@Override
	public void onDestroy() {
		super.onDestroy();
		if (m_isBound) {
			unbindService(m_conn);
			Log.d("trackpad_activity", "Socket service unbound");
		}
	}

    @Override
    public boolean onTouch(View view, final MotionEvent event) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                processTouchEvent(event);
            }
        };
        executor.execute(runnable);
//        new Thread(runnable).start();
//        Log.d("trackpad_activity", "Touch event|Thread:" + Thread.currentThread().getId());
        return true;
    }

    private void processTouchEvent(MotionEvent event) {
//        Log.d("trackpad_activity", "Thread:" + Thread.currentThread().getId());
        int action = event.getActionMasked();
        switch(action) {
            case MotionEvent.ACTION_DOWN:
                mouseDown(event);
                break;
            case MotionEvent.ACTION_UP:
                mouseUp(event);
                break;
            case MotionEvent.ACTION_MOVE:
                mouseMove(event);
                break;
        }

    }

//	private TrackpadView.TrackpadCallback m_trackpadCallback = new TrackpadView.TrackpadCallback() {
//
//		@Override
    public void mouseDown(MotionEvent e) {
        m_firstX = m_lastX = e.getX();
        m_firstY = m_lastY = e.getY();
    }
//
//		@Override
    public void mouseUp(MotionEvent e) {
        if (e.getX() == m_firstX && e.getY() == m_firstY) {
            m_socketService.sendMouseClick();
        }
    }
//
//		@Override
    public void mouseMove(MotionEvent e) {
        double newX;
        double newY;
        double diffX;
        double diffY;
        newX = e.getX();
        newY = e.getY();
        diffX = newX - m_lastX;
        m_lastX = newX;
        diffY = newY - m_lastY;
        m_lastY = newY;
        if (diffX != 0 && diffY != 0) {
            m_socketService.sendMouseMove(diffX, diffY);
        }
    }
//
//        private double firstX;
//        private double firstY;
//        private double lastX;
//        private double lastY;
//        private double accel = 2.5;
//
//	};
	
	private ServiceConnection m_conn = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			SocketBinder binder = (SocketBinder)service;
			m_socketService = binder.getService();
			if (m_socketService != null) {
				Log.d("trackpad_activity", "Socket service bound.");
				m_isBound = true;
			}
		}
		
		@Override
		public void onServiceDisconnected(ComponentName className) {
			Log.d("trackpad_activity", "Socket service is unbound");
            m_isBound = false;
		}
	};
}
