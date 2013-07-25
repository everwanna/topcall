package com.topcall.andsdk;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
	private ANDSDK mSDK = null;
	private Button mBtnLogin = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mSDK = new ANDSDK();
		
		mBtnLogin = (Button)findViewById(R.id.btn_login);
		mBtnLogin.setOnClickListener( new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				mSDK.getLoginMgr().login("yunzed", "password1");
			}
		});
		//
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
