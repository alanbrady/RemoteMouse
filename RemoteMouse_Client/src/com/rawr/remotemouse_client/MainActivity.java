package com.rawr.remotemouse_client;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		TextView xtext = (TextView)findViewById(R.id.text_xedit);
		TextView ytext = (TextView)findViewById(R.id.text_yedit);
		
		TrackpadView tp = (TrackpadView)findViewById(R.id.trackpad_view);
		tp.setXYTextView(xtext, ytext);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
