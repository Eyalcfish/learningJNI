package jni;

public class NativePipe {

    static {
        System.loadLibrary("native");
    }

    public native String[] readFiles(String[] paths);

    public native float[][] fileOutput(String data);
}