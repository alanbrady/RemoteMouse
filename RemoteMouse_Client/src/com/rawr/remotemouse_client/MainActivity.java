package com.rawr.remotemouse_client;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;

public class MainActivity extends Activity {

	private TextView mXText;
	private TextView mYText;
		
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mXText = (TextView)findViewById(R.id.text_xedit);
		mYText = (TextView)findViewById(R.id.text_yedit);
		
		TrackpadView tp = (TrackpadView)findViewById(R.id.trackpad_view);
		tp.setCallback(new TrackpadCallback() {
			public void callback(float x, float y) {
				calculateMousePhysics(x, y);
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private void calculateMousePhysics(float x, float y) {
		mXText.setText(String.valueOf(x));
		mYText.setText(String.valueOf(y));
	}

}
