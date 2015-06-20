package dgu.swc.activity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gcm.GCMRegistrar;

import dgu.swc.activity.util.Constants;
import dgu.swc.activity.util.HashUtils;
import dgu.swc.main.Global;
import dgu.swc.mobilestreamer.R;
import dgu.swc.webrtc.SocketEvent;
import dgu.swc.webrtc.SocketService;
import dgu.swc.webrtc.SocketService.EventListener;

public class SignInActivity extends Activity implements EventListener, OnClickListener {

	private static final String TAG = SignInActivity.class.getName();
	private SocketService socketService = SocketService.getInstance();
	private EditText edtEmail;
	private EditText edtPassword;
	private Button btnSignIn;
	private TextView txtNewAccount;
	private Context appContext;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_signin);

		socketService.addListener(this);

		appContext = getApplicationContext();
		edtEmail = (EditText)findViewById(R.id.edtSignInEmail);
		edtPassword = (EditText)findViewById(R.id.edtSignInPassword);
		btnSignIn = (Button)findViewById(R.id.btnSignIn);
		btnSignIn.setOnClickListener(this);

		txtNewAccount = (TextView)findViewById(R.id.txtSignUp);
		txtNewAccount.setOnClickListener(this);
	
	}

	@Override
	public void onClick(View v) {
		switch(v.getId()) {
		case R.id.btnSignIn:
			onClickBtnSignIn(v);
			break;
		case R.id.btnFaceSignIn:
			Log.d(TAG, "Face Login.");
			break;
		case R.id.txtSignUp:
			onClickBtnNewAccount(v);
			break;
		}
	}

	/**
	 * 사인-인 버튼을 눌렀을 때 밸리데이션 체크를 실시하며 서버로 로그인 이벤트를 전송합니다.
	 */
	private void onClickBtnSignIn(View v) {
		String email = edtEmail.getText().toString().trim();
		String password = edtPassword.getText().toString().trim();

		if(email.length() == 0) {
			Toast.makeText(this, "이메일을 입력하세요.", Toast.LENGTH_SHORT).show();
			edtEmail.selectAll();
			return;
		}

		if(password.length() == 0) {
			Toast.makeText(this, "비밀번호를 입력하세요.", Toast.LENGTH_SHORT).show();
			edtPassword.selectAll();
			return;
		}

		// 패스워드를 md5로 암호화합니다. 실제 인터넷에 md5 암호화 결과 테이블이 많이 존재하기 때문에
		// 접두사를 추가하여 예상 패스워드 공격을 방지합니다.
		password = HashUtils.md5("prefix" + password);

		/**
		 * Local로 email set하기
		 */
		Global.setEmail(email);
		
		// 사인-인 이벤트 발생
		socketService.signIn(email, password);
	}

	/**
	 * 새 계정 버튼을 눌렀을 때 사인-온 액티비티로 이동합니다.
	 */
	private void onClickBtnNewAccount(View v) {
		startActivity(new Intent(getApplicationContext(), SignUpActivity.class));
	}

	/**
	 * 서버에서 이벤트를 받았을 때 호출합니다.
	 * 
	 * 사인-인 이벤트를 처리합니다.
	 */
	@Override
	public void onListen(int event, int code, Object data) {
		switch(event) {
		case SocketEvent.MSG_SIGN_IN:
			onSignIn(code, data);
			break;
		}
	}

	/**
	 * 사인-인 이벤트를 처리합니다. 
	 * 
	 * @param code 이벤트 결과 코드
	 * @param data 이벤트 결과 데이터, 에러 발생 시 String값으로 에러 메시지를 받습니다.
	 */
	private void onSignIn(int code, Object data) {
		switch(code) {
		case SocketEvent.SUCCESS:
			Toast.makeText(this, "로그인 되었습니다.", Toast.LENGTH_SHORT).show();
			
			startActivity(new Intent(this, ShootingActivity.class));
			startGCM();//GCM 시작 메소드
			finish();
			break;
		case SocketEvent.FAILURE:
			Toast.makeText(this, (String)data, Toast.LENGTH_SHORT).show();
			break;
		}
	}
	
	/**
	 * GCM 서비스를 시작한다.
	 */
	private void startGCM(){
		
		/**
		 * GCM Service가 이용 가능한 Device인지 체크한다.
		 * api 8(Android 2.2) 미만인 경우나 GCMService를 이용할 수 없는
		 * 디바이스의 경우 오류를 발생시키니 반드시 예외처리하도록 한다.
		 */
		try {
			GCMRegistrar.checkDevice(appContext);
		} catch (Exception e) {
			// TODO: handle exception
			Log.e(TAG, "This device can't use GCM");
			return;
		}
		
		
		/**
		 * 2.SharedPreference에 저장된 RegistrationID가 있는지 확인한다.
		 * 없는 경우 null이 아닌 ""이 리턴
		 */
		 String regId = GCMRegistrar.getRegistrationId(appContext);
		
		/**
		 * Registration Id가 없는 경우(어플리케이션 최초 설치로 발급받은 적이 없거나,
		 * 삭제 후 재설치 등 SharedPreference에 저장된 Registration Id가 없는 경우가 이에 해당한다.)
		 */	
		if(regId.isEmpty()){
			/**
			 * 3.RegstrationId가 없는 경우 GCM Server로 Regsitration ID를 발급 요청한다.
			 * 발급 요청된 Registration ID는 SharedPreference에 저장된다.
			 */
			Log.d("GCMGCM", "regId is empty");
			GCMRegistrar.register(appContext, Constants.PROJECT_ID);
			
		//SharedPreference에 저장된 Registration Id가 존재하는 경우
		}else{
			//GCMRegistrar.unregister(appContext);
			//GCMRegistrar.register(appContext, Constants.PROJECT_ID);
			//regId = GCMRegistrar.getRegistrationId(appContext);
			Log.d("GCMGCM", "regID : "+ regId);
			Toast.makeText(appContext, "Exist Registration Id: " + regId, Toast.LENGTH_LONG).show();
			//startActivity(new Intent(this, ImageUploadActivity.class));
			Log.d("GCMGCM", Global.getEmail() + " ");

			
			socketService.regIdUpload(Global.getEmail(), regId);
			
		}
	

	}
}