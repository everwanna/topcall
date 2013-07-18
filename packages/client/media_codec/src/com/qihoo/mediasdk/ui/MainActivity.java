package com.qihoo.mediasdk.ui;

import com.qihoo.mediasdk.MediaSDK;
import com.vcap.codec.R;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
	private Button mBtnStart = null;
	private Button mBtnStop = null;
	private Button mBtnRecord = null;
	private Button mBtnFile = null;
	private MediaSDK mSDK = null;	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mSDK = new MediaSDK(null, 10000);
		mSDK.login("218.30.117.201", 2041);
		
		mBtnStart = (Button)findViewById(R.id.btn_start);
		mBtnStart.setOnClickListener( new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				mSDK.call(1, 1, 1, 1);
			}
		});
		//
		
		mBtnStop = (Button)findViewById(R.id.btn_stop);
		mBtnStop.setOnClickListener( new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				if( null != mSDK ) {
					mSDK.getAudioMgr().getRecorder().stopRecord();
					mSDK.getAudioMgr().getWriter().close();
				}
			}
		});
		
		mBtnRecord = (Button)findViewById(R.id.btn_record);
		mBtnRecord.setOnClickListener( new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				mSDK.getAudioMgr().getRecorder().startRecord();
			}
		});
		
		mBtnFile = (Button)findViewById(R.id.btn_file);
		mBtnFile.setOnClickListener( new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				mSDK.getAudioMgr().getFilePlayer().play();
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onResume() {
		super.onResume();
	}
}
