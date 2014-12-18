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

public class TrackpadActivity extends Activity {
	SocketService m_socketService = null;
	TrackpadView m_trackpadView = null;
	boolean m_isBound;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_trackpad);
		
		m_trackpadView = (TrackpadView)findViewById(R.id.trackpadView);
		m_trackpadView.setCallback(m_trackpadCallback);
		
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
	
	private TrackpadView.TrackpadCallback m_trackpadCallback = new TrackpadView.TrackpadCallback() {

		@Override
		public void mouseDown(MotionEvent e) {
            firstX = lastX = e.getX();
            firstY = lastY = e.getY();
		}

		@Override
		public void mouseUp(MotionEvent e) {
            if (e.getX() == firstX && e.getY() == firstY) {
                m_socketService.sendMouseClick();
            }
		}

		@Override
		public void mouseMove(MotionEvent e) {
            double newX;
            double newY;
            double diffX;
            double diffY;
            newX = e.getX();
            newY = e.getY();
            diffX = newX - lastX;
            lastX = newX;
            diffY = newY - lastY;
            lastY = newY;
            if (diffX != 0 && diffY != 0) {
                m_socketService.sendMouseMove(diffX, diffY);
            }
		}

        private double firstX;
        private double firstY;
        private double lastX;
        private double lastY;
        private double accel = 2.5;

	};
	
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
