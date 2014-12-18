package com.rawr.remotemouse_client;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class TrackpadView extends View {
	private TrackpadCallback m_trackPadCallback;
	
	public TrackpadView(Context context) {
		super(context);
		init(null, 0);
	}

	public TrackpadView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(attrs, 0);
	}

	public TrackpadView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(attrs, defStyle);
	}

	private void init(AttributeSet attrs, int defStyle) {
		m_trackPadCallback = null;
	}

	@Override
	protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
    }

	
	public void setCallback(TrackpadCallback callback) {
		m_trackPadCallback = callback;
	}
	
	@SuppressLint("ClickableViewAccessibility") @Override
	public boolean onTouchEvent(MotionEvent e) {
		if (m_trackPadCallback != null) {
//			mTpCallback.callback(e);
			int action = e.getActionMasked();
			switch(action) {
			case MotionEvent.ACTION_DOWN:
				m_trackPadCallback.mouseDown(e);
				break;
			case MotionEvent.ACTION_UP:
				m_trackPadCallback.mouseUp(e);
				break;
			case MotionEvent.ACTION_MOVE:
				m_trackPadCallback.mouseMove(e);
				break;
			}
		} else {
			Log.e("trackpad_view", "Error: no callback initiated");
		}
		return true;
	}

	public interface TrackpadCallback {
		public void mouseDown(MotionEvent e);
		public void mouseUp(MotionEvent e);
		public void mouseMove(MotionEvent e);
	}
}


