package com.rawr.remotemouse_client;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
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
//	private PrintWriter m_out = null;
	private BufferedOutputStream m_out = null;
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
				m_socket.setSoTimeout(5000);
				m_socket.connect(new InetSocketAddress(m_ip, SERVER_PORT), 1000);
				
				Log.e("socket_serv", "Initializing input/output streams");
//				m_out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(m_socket.getOutputStream())), true);
				m_out = new BufferedOutputStream(m_socket.getOutputStream());
				m_in = new BufferedReader(new InputStreamReader(m_socket.getInputStream()));
				
				Log.e("socket_serv", "Performing challenge validation");
				char[] challenge = getChallenge();
				if (challenge != null) {
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
	
	private char[] getChallenge() {
//		m_out.println("CHAL_REQ");
		try {
			m_out.write("CHAL_REQ\n".getBytes("ASCII"));
			m_out.flush();
		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		try {
			char[] buf = new char[256];
			m_in.read(buf, 0, 256);
			return buf;
		} catch(IOException e) {
			Log.e("socket_serv", "IOException: " + e.toString());
		}
		return null;
	}
	
	private void sendChallengeResponse(char[] challenge) {
		int chalLen = challenge.length;
		int keyLen = m_key.length();
		byte[] hash = new byte[keyLen];
		int k = 0;
		for (int i = 0; i < chalLen; i++) {
			if (k >= keyLen) {
				k = 0;
			}
			hash[k] = (byte)(((byte)challenge[i]) ^ ((byte)m_key.charAt(k)) ^ hash[k]);
			k++;
		}
//		m_out.print("CHAL_RSP");
		try {
			m_out.write("CHAL_RSP".getBytes("ASCII"));
			m_out.write((byte)m_id.length());
//			m_out.print((byte)m_id.length());
//			m_out.print(m_id);
			m_out.write(m_id.getBytes("ASCII"));
			m_out.write(hash);
			m_out.write('\n');
			m_out.flush();
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		
	}
}
