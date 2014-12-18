package com.rawr.remotemouse_client;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.content.Intent;
import android.util.Log;
import android.widget.EditText;

public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.server_info_layout);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void connect(View view) {
		final EditText serverIpText = (EditText) findViewById(R.id.server_ip);
		final EditText clientIdText = (EditText) findViewById(R.id.client_id);
		final EditText clientKeyText = (EditText) findViewById(R.id.client_key);
		
		Intent connectScreen = new Intent(getApplicationContext(), ConnectActivity.class);
		connectScreen.putExtra("ip", serverIpText.getText().toString());
		connectScreen.putExtra("id", clientIdText.getText().toString());
		connectScreen.putExtra("key", clientKeyText.getText().toString());
		Log.d("main", "Attempting to connect to: " + serverIpText.getText());
		startActivity(connectScreen);
	}
}
