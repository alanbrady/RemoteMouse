package com.rawr.remotemouse_client;

//import com.rawr.remotemouse_client.ConnectActivity.SocketStatusCallback;
//import com.rawr.remotemouse_client.ConnectActivity.SocketVerificationCallback;
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
//import android.view.Menu;
//import android.view.MenuItem;

public class TrackpadActivity extends Activity {
//	TrackpadCallback m_trackpad = null;
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
	
//	private class TrackpadCallback implements TrackpadView.TrackpadCallback {
	private TrackpadView.TrackpadCallback m_trackpadCallback = new TrackpadView.TrackpadCallback() {

		@Override
		public void mouseDown(MotionEvent e) {
//			Log.d("trackpad_activity", "Mouse down");
            firstX = lastX = e.getX();
            firstY = lastY = e.getY();
		}

		@Override
		public void mouseUp(MotionEvent e) {
//			Log.d("trackpad_activity", "Mouse up");
            if (e.getX() == firstX && e.getY() == firstY) {
                m_socketService.sendMouseClick();
            }
		}

		@Override
		public void mouseMove(MotionEvent e) {
//            Log.d("trackpad_activity", "Mouse move");
//            final int pointer = e.findPointerIndex(e.getPointerId(0));
//            double newX = e.getX();
//            double newY = e.getY();
//            double diffX = newX - lastX;
//            lastX = newX;
//            double diffY = newY - lastY;
//            lastY = newY;
//
//            diffX *= accel;
//            diffY *= accel;
//
//            if (diffX != 0 && diffY != 0) {
//                m_socketService.sendMouseMove(diffX, diffY);
//            }


            double newX;
            double newY;
            double diffX;
            double diffY;
//            final int histSize = e.getHistorySize();
//            for (int i = 0; i < histSize; i++) {
//                newX = e.getHistoricalX(i);
//                newY = e.getHistoricalY(i);
//                diffX = newX - lastX;
//                lastX = newX;
//                diffY = newY - lastY;
//                lastY = newY;
//                if (diffX != 0 && diffY != 0) {
//                    m_socketService.sendMouseMove(diffX, diffY);
//                }
//            }
            newX = e.getX();
            newY = e.getY();
            diffX = newX - lastX;
            lastX = newX;
            diffY = newY - lastY;
            lastY = newY;
            if (diffX != 0 && diffY != 0) {
                m_socketService.sendMouseMove(diffX, diffY);
            }

//            Log.d("trackpad_activity", "Mouse Move: " + diffX + " " + diffY);
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
//				m_socketService.setStatusCallback(new SocketStatusCallback());
//				m_socketService.setVerificationCallback(new SocketVerificationCallback());
//				m_socketService.connectSocket(m_ip, m_id, m_key);
			}
		}
		
		@Override
		public void onServiceDisconnected(ComponentName className) {
			Log.d("trackpad_activity", "Socket service is unbound");
            m_isBound = false;
		}
	};
}
