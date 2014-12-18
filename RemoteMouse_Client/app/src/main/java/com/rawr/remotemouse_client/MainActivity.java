package com.rawr.remotemouse_client;

import android.content.Context;
import android.content.SharedPreferences;
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

        SharedPreferences prefs = getSharedPreferences(getString(R.string.preferences_key), Context.MODE_PRIVATE);
        ((EditText)findViewById(R.id.server_ip)).setText(prefs.getString(getString(R.string.server_ip_key), ""));
        ((EditText)findViewById(R.id.client_id)).setText(prefs.getString(getString(R.string.client_id_key), ""));
        ((EditText)findViewById(R.id.client_key)).setText(prefs.getString(getString(R.string.client_key_key), ""));
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

        SharedPreferences.Editor editor = getSharedPreferences(getString(R.string.preferences_key),
                                                       Context.MODE_PRIVATE).edit();
        editor.putString(getString(R.string.server_ip_key), serverIpText.getText().toString());
        editor.putString(getString(R.string.client_id_key), clientIdText.getText().toString());
        editor.putString(getString(R.string.client_key_key), clientKeyText.getText().toString());
        editor.commit();

		Intent connectScreen = new Intent(getApplicationContext(), ConnectActivity.class);
//		connectScreen.putExtra("ip", serverIpText.getText().toString());
//		connectScreen.putExtra("id", clientIdText.getText().toString());
//		connectScreen.putExtra("key", clientKeyText.getText().toString());
		Log.d("main", "Attempting to connect to: " + serverIpText.getText());
		startActivity(connectScreen);
	}
}
