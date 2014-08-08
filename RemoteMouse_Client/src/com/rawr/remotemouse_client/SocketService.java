package com.rawr.remotemouse_client;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.InetAddress;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.os.Binder;

public class SocketService extends Service {
	private String m_ip;
	private String m_id;
	private String m_key;
	private SocketCallback m_callback = null;
	private Socket m_socket;
	private final IBinder m_binder = new SocketBinder();
	private PrintWriter m_out = null;
	private BufferedReader m_in = null;
	
	private final int SERVER_PORT = 48048;
	
	public SocketService() {
	}

	public class SocketBinder extends Binder {
		SocketService getService() {
			return SocketService.this;
		}
	}
	
	private class ConnectRunnable implements Runnable {
		@Override
		public void run() {
			try {
				Log.e("socket_serv", "Connecting socket...");
				m_socket = new Socket();
				m_socket.connect(new InetSocketAddress(m_ip, SERVER_PORT), 1000);
				
				Log.e("socket_serv", "Initializing input/output streams");
				m_out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(m_socket.getOutputStream())), true);
				m_in = new BufferedReader(new InputStreamReader(m_socket.getInputStream()));
				
				Log.e("socket_serv", "Performing challenging validation");
				String challenge = getChallenge();
				if (challenge.length() > 0) {
					sendChallengeResponse(challenge);
				} else {
					Log.e("socket_serv", "Failed to retreive challenge");
				}
			} catch (IOException e) {
				Log.e("socket_serv", "IOException: " + e.toString());
			}
		}
	}
	
	public interface SocketCallback {
		void socketRead(String str);
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return m_binder;
	}
		
	public void setCallback(SocketCallback callback) {
		m_callback = callback;
	}
	
	public void connectSocket(String ip, String id, String key) {
		m_ip = ip;
		m_id = id;
		m_key = key;
		new Thread(new ConnectRunnable()).start();
	}
	
	private String getChallenge() {
		m_out.write("CHAL_REQ");
		try {
			String chal = m_in.readLine();
			return chal;
		} catch(IOException e) {
			Log.e("socket_serv", "IOException: " + e.toString());
		}
		return "";
	}
	
	private void sendChallengeResponse(String challenge) {
		int chalLen = challenge.length();
		int keyLen = m_key.length();
		byte[] hash = new byte[keyLen];
		int k = 0;
		for (int i = 0; i < chalLen; i++) {
			if (k >= keyLen) {
				k = 0;
			}
			hash[k] = (byte)(((byte)challenge.charAt(i)) ^ ((byte)m_key.charAt(k)) ^ hash[k]);
		}
		m_out.print("CHAL_RSP" + hash);
	}
}
