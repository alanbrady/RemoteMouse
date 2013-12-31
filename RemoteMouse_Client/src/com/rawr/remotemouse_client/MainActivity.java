package com.rawr.remotemouse_client;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;
import android.view.MotionEvent;
import java.math.*;

public class MainActivity extends Activity {

	private TextView mXText;
	private TextView mYText;
	private TextView mDXText;
	private TextView mDYText;
	private TextView mVelText;

	private float mLastX;
	private float mLastY;
	private long mLastTime;
	
	class CalcAndTransmitRunnable implements Runnable {
		private float mX;
		private float mY;
		
		public CalcAndTransmitRunnable(float x, float y) {
			mX = x;
			mY = y;
		}
		
		public void run() {
//			calculateMousePhysics();
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mXText = (TextView)findViewById(R.id.text_xedit);
		mYText = (TextView)findViewById(R.id.text_yedit);
		mDXText = (TextView)findViewById(R.id.text_dxedit);
		mDYText = (TextView)findViewById(R.id.text_dyedit);
		mVelText = (TextView)findViewById(R.id.text_veledit);
		
		TrackpadView tp = (TrackpadView)findViewById(R.id.trackpad_view);
		tp.setCallback(new TrackpadCallback() {
			public void callback(MotionEvent e) {
				// TODO: Reimplement thread - while playing with views
				// this breaks concurrency without adding more scaffolding
//				CalcAndTransmitRunnable runnable = new CalcAndTransmitRunnable(x, y);
//				Thread task = new Thread(runnable);
//				task.start();
				calculateMousePhysics(e);
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private void calculateMousePhysics(MotionEvent e) {
		mXText.setText(String.valueOf(e.getX()));
		mYText.setText(String.valueOf(e.getY()));
		long currTime = System.currentTimeMillis();
		long dt = currTime - mLastTime;
		if (e.getAction() != MotionEvent.ACTION_DOWN && dt != 0) {
			float dx = mLastX - e.getX();
			float dy = mLastY - e.getY();
			double dxdt = dx/dt;
			double dydt = dy/dt;
			double vel = Math.sqrt(Math.pow(dxdt, 2) + Math.pow(dydt, 2));
			mDXText.setText(String.valueOf(dx));
			mDYText.setText(String.valueOf(dy));
			mVelText.setText(String.valueOf(vel));
			mLastX = e.getX();
			mLastY = e.getY();
			mLastTime = currTime;
		}
		
	}

}
