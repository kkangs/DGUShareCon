package dgu.swc.jni;



public class TayoJNI {
	static {
		//System.loadLibrary("opencv_java");
		System.loadLibrary("TayoNDK");
	}
	
	public native String stringFromJNI();

}
