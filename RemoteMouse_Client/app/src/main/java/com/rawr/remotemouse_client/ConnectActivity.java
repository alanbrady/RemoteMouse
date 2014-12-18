package com.rawr.remotemouse_client;

import com.rawr.remotemouse_client.SocketService.SocketBinder;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.os.IBinder;
import android.content.Context;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.widget.TextView;
import android.content.Intent;
import android.content.ServiceConnection;
import android.util.Log;

public class ConnectActivity extends Activity {
//	String m_ip;
//	String m_id;
//	String m_key;
	SocketService m_socketService;
	boolean m_isBound;
	TextView m_statusText;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.connect_screen_layout);
		
		Intent i = getIntent();
//		m_ip = i.getStringExtra("ip");
//		m_id = i.getStringExtra("id");
//		m_key = i.getStringExtra("key");
		
		m_statusText = (TextView) findViewById(R.id.connectStatusText);
		
		Intent intent = new Intent(this, SocketService.class);
		bindService(intent, m_conn, Context.BIND_AUTO_CREATE);
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		if (m_isBound) {
			unbindService(m_conn);
			Log.d("connect_activity", "Socket service unbound");
		}
	}
	
	private ServiceConnection m_conn = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			SocketBinder binder = (SocketBinder)service;
			m_socketService = binder.getService();
			if (m_socketService != null) {
				Log.d("connect_act", "Service has been bound, attempting socket connect.");
				m_isBound = true;
				m_socketService.setStatusCallback(new SocketStatusCallback());
				m_socketService.setVerificationCallback(new SocketVerificationCallback());
//                m_socketService.connectSocket();
			}
		}
		
		@Override
		public void onServiceDisconnected(ComponentName className) {
			m_isBound = false;
		}
	};
	
	private class SocketStatusCallback implements SocketService.StatusCallback {

		@Override
		public void newStatus(final String str) {
			runOnUiThread(new Runnable() {
				public void run() {
					m_statusText.setText(str);					
				}
			});
		}
	}
	
	private class SocketVerificationCallback implements SocketService.VerificationCallback {

		@Override
		public void verificationPass() {
			Intent trackpad = new Intent(getApplicationContext(), TrackpadActivity.class);
			startActivity(trackpad);
		}

		@Override
		public void verificationFail() {
			runOnUiThread(new Runnable() {
				public void run() {
					verificationErrorMessage();
				}
			});
		}
		
	}
	
	private void verificationErrorMessage() {
		AlertDialog.Builder dlgAlert = new AlertDialog.Builder(this);
		dlgAlert.setTitle("Verification Fail");
		dlgAlert.setMessage("Client verification failed.  Make sure client ID and key are correct.");
		dlgAlert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				finish();
			}
		});
		dlgAlert.create().show();
	}
}
