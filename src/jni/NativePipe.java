package jni;

public class NativePipe {

    static {
        System.loadLibrary("native");
    }

    public static native byte[][] readFiles(String[] paths,int accuracy);

    public static native float[][] fileOutput(byte[] data,int accuracy);
}