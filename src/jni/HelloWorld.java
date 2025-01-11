package jni;

public class HelloWorld {

    static {
        System.loadLibrary("native");
    }

    public native float[][] fileOutput(String data);
}