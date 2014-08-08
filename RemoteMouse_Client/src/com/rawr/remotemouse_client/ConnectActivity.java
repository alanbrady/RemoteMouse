package com.rawr.remotemouse_client;

import com.rawr.remotemouse_client.SocketService.SocketBinder;

import android.app.Activity;
import android.os.Bundle;
import android.os.IBinder;
import android.content.Context;
import android.content.ComponentName;
//import android.view.Menu;
//import android.view.MenuItem;
import android.content.Intent;
import android.content.ServiceConnection;
import android.util.Log;

public class ConnectActivity extends Activity {
	String m_ip;
	String m_id;
	String m_key;
	SocketService m_socketService;
	boolean m_isBound;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.connect_screen_layout);
		
		Intent i = getIntent();
		m_ip = i.getStringExtra("ip");
		m_id = i.getStringExtra("id");
		m_key = i.getStringExtra("key");
		
//		Log.e("n", "Received: " + m_ip + " " + m_id + " " + m_key);
		Intent intent = new Intent(this, SocketService.class);
		bindService(intent, mConn, Context.BIND_AUTO_CREATE);
		
//		if (m_isBound) {
			
//		}
	}
	
	private ServiceConnection mConn = new ServiceConnection() {
		
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			SocketBinder binder = (SocketBinder)service;
			m_socketService = binder.getService();
			if (m_socketService != null) {
				Log.e("connect_act", "Service has been bound, attempting socket connect.");
				m_isBound = true;
				m_socketService.connectSocket(m_ip, m_id, m_key);
			}
		}
		
		@Override
		public void onServiceDisconnected(ComponentName className) {
			m_isBound = false;
		}
	};
}
