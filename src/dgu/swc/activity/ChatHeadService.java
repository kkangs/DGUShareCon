package dgu.swc.activity;


import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.util.Base64;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;
import dgu.swc.main.Global;
import dgu.swc.mobilestreamer.R;
import dgu.swc.webrtc.SocketService;

public class ChatHeadService extends Service  {

	private WindowManager windowManager;
	private ImageView chatHead;
	WindowManager.LayoutParams params;
	
	
	private String email;
	
	long touchTime;

	@Override
	public void onCreate() {
		super.onCreate();
		
		email = new Intent().getStringExtra("email");
		
		windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);

		chatHead = new ImageView(this);
		chatHead.setImageResource(R.drawable.shutter);

		params= new WindowManager.LayoutParams(
				WindowManager.LayoutParams.WRAP_CONTENT,
				WindowManager.LayoutParams.WRAP_CONTENT,
				WindowManager.LayoutParams.TYPE_PHONE,
				WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
				PixelFormat.TRANSLUCENT);

		params.gravity = Gravity.TOP | Gravity.LEFT;
		params.x = 0;
		params.y = 100;
		
		
		
	
		//this code is for dragging the chat head
		chatHead.setOnTouchListener(new View.OnTouchListener() {
			private int initialX;
			private int initialY;
			private float initialTouchX;
			private float initialTouchY;

			@Override
			public boolean onTouch(View v, MotionEvent event) {
				
				switch (event.getAction()) {
				case MotionEvent.ACTION_DOWN:

					touchTime = System.currentTimeMillis();
					
					initialX = params.x;
					initialY = params.y;
					initialTouchX = event.getRawX();
					initialTouchY = event.getRawY();
					return true;
					
					
				case MotionEvent.ACTION_UP:
					
					if(System.currentTimeMillis() - touchTime > 500 )
					{
						
						Intent intent = new Intent(getApplicationContext(),ShootingActivity.class);
						intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
						startActivity(intent);
						Toast.makeText(getApplicationContext(), "화면으로 돌아갑니다.",Toast.LENGTH_SHORT).show();

					
					}
					else
					{
						if( (Math.abs(initialTouchX - event.getRawX())<5) && (Math.abs(initialTouchY - event.getRawY())<5) )
	                    {
							try {
								
								screenshot(v);
								
								
								//chatHead.setOnClickListener(new ImageUploadButtonHandler(this));
								Toast.makeText(getApplicationContext(), "캡쳐 되었습니다.",Toast.LENGTH_SHORT).show();
								
								new Thread(new Runnable() {
									
									@Override
									public void run() {
										// TODO Auto-generated method stub
										try {
											Thread.sleep(3000);
										} catch (InterruptedException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										}
									}
								}).run();
								
								//String capturedPath = "/storage/emulated/0/img.png"; //"/sdcard/img.png";
								
								BitmapFactory.Options sizeOptions = new BitmapFactory.Options();
								sizeOptions.inJustDecodeBounds = true;
								sizeOptions.inSampleSize = 4;
								
								int dstWidth = 90;
								int dstHeight = 80;
								Bitmap captured = BitmapFactory.decodeFile("/sdcard/img.png");
								Bitmap resized = Bitmap.createScaledBitmap(captured, dstWidth, dstHeight, true);
								
								if(captured == null) {
									Log.e("adasasd", "captured is null");	
								}
									SocketService.getInstance().imageUpload(Global.getEmail(), 
											Base64.encodeToString(bitmapToByteArray(resized), 0));
										
									Toast.makeText(getApplicationContext(), "전송 되었습니다.",Toast.LENGTH_SHORT).show();
									
								//activity.callbackImageSelected();
								
							} catch (Exception e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
	                   
	                    }
						

						//int inSampleSize = calculateInSampleSize(sizeOptions, width, height);

						//sizeOptions.inJustDecodeBounds = false;
						//sizeOptions.inSampleSize = inSampleSize;
						//return BitmapFactory.decodeFile(picturePath, sizeOptions);
						
						//ImageUploadButtonHandler
					}
					

					return true;
				case MotionEvent.ACTION_MOVE:
					params.x = initialX
							+ (int) (event.getRawX() - initialTouchX);
					params.y = initialY
							+ (int) (event.getRawY() - initialTouchY);
					windowManager.updateViewLayout(chatHead, params);
					return true;
				}
				return false;
			}
		});
		windowManager.addView(chatHead, params);
	}
	


	
public void screenshot(View v) {
		
		Process sh;
		try {
			sh = Runtime.getRuntime().exec("su", null,null);
			 
	        OutputStream  os = sh.getOutputStream();
	        os.write(("/system/bin/screencap -p " + "/sdcard/img.png").getBytes("ASCII"));
	        os.flush();

	        os.close();
	        sh.waitFor();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	

	@Override
	public void onDestroy() {
		super.onDestroy();
		if (chatHead != null)
			windowManager.removeView(chatHead);
	}

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public byte[] bitmapToByteArray(Bitmap bitmap) {
		ByteArrayOutputStream stream = new ByteArrayOutputStream();
		bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);

		return stream.toByteArray();
	}
	

}