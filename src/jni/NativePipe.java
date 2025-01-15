package jni;

public class NativePipe {

    static {
        System.loadLibrary("native");
    }

    public native byte[][] readFiles(String[] paths);

    public native float[][] fileOutput(byte[] data);
}