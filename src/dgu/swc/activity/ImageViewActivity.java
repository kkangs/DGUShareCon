package dgu.swc.activity;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.widget.ImageView;
import dgu.swc.mobilestreamer.R;

public class ImageViewActivity extends Activity {

	private ImageView present_shareView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image_view);
		
		Intent i = getIntent();
		String imageStr = i.getStringExtra("imgdata");
		
		Log.i("adasd", ""+imageStr);
		
		present_shareView = (ImageView)findViewById(R.id.present_shareView);
		
		byte[] decodedBytes = Base64.decode(imageStr, 0);
		Bitmap bitmapImage = BitmapFactory.decodeByteArray(decodedBytes, 0, decodedBytes.length);
		present_shareView.setImageBitmap(bitmapImage);
		
	}
}
