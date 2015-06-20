package dgu.swc.mobilestreamer;

import com.google.android.gcm.GCMBaseIntentService;

import dgu.swc.main.Global;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;
import android.util.Log;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;

import dgu.swc.activity.ImageViewActivity;
import dgu.swc.activity.SignInActivity;
import dgu.swc.activity.util.Constants;
import dgu.swc.mobilestreamer.R;


public class GCMIntentService extends GCMBaseIntentService {
	private static void generateNotification(Context context, String msg, String imageStr) {
		 
		int icon = R.drawable.ic_launcher;
		long when = System.currentTimeMillis();
		 
		 
		NotificationManager notificationManager = (NotificationManager) context
		.getSystemService(Context.NOTIFICATION_SERVICE);
		 
		
		
		Notification notification = new Notification(icon, msg, when);
		 
		String title = context.getString(R.string.app_name);
		 
		Intent notificationIntent = new Intent(context, ImageViewActivity.class);
		notificationIntent.putExtra("imgdata", imageStr);
		
		notificationIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP 
		        | Intent.FLAG_ACTIVITY_SINGLE_TOP);
		
		PendingIntent intent = PendingIntent.getActivity(context, 0,
		notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
		 
		
		 
		 
		notification.setLatestEventInfo(context, title, msg, intent);
		 
		notification.flags |= Notification.FLAG_AUTO_CANCEL;
		 
		notificationManager.notify(0, notification);
		 
		}

	/**
	 * GCM Server로부터 발급받은 Project ID를 통해 SuperClass인
	 * GCMBaseIntentService를 생성해야한다. 
	 */
	public GCMIntentService() {
		super(Constants.PROJECT_ID);
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onError(Context arg0, String arg1) {
		// TODO Auto-generated method stub
		/**
		 * GCM 오류 발생 시 처리해야 할 코드를 작성한다.
		 * ErrorCode에 대해선 GCM 홈페이지와 GCMConstants 내 static variable 참조한다. 
		 */
		

	}

	@Override
	protected void onMessage(Context arg0, Intent intent) {
		// TODO Auto-generated method stub
		/**
		 * GCMServer가 전송하는 메시지가 정상 처리 된 경우 구현하는 메소드이다.
		 * Notification, 앱 실행 등등 개발자가 하고 싶은 로직을 해당 메소드에서 구현한다.
		 * 전달받은 메시지는 Intent.getExtras().getString(key)를 통해 가져올 수 있다.
		 */
		Log.d("GCMIntentxz", intent.getExtras().toString());
		
		String msg = intent.getExtras().getString("msg");
		Log.d("GCMIntent", "msg : "+msg);
		
		String imageStr = intent.getExtras().getString("img");
		generateNotification(arg0, msg, imageStr);
		

	}

	@Override
	protected void onRegistered(Context arg0, String regId) {
		// TODO Auto-generated method stub
		/**
		 * GCMRegistrar.getRegistrationId(context)가 실행되어 registrationId를 발급받은 경우 해당 메소드가 콜백된다.
		 * 메시지 발송을 위해 regId를 서버로 전송하도록 하자.
		 */

	}

	@Override
	protected void onUnregistered(Context arg0, String arg1) {
		// TODO Auto-generated method stub
		/**
		 * GCMRegistrar.unregister(context) 호출로 해당 디바이스의 registrationId를 해지요청한 경우 해당 메소드가 콜백된다.
		 */

	}

}
