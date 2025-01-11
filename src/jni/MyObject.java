package jni;

public class MyObject {
    public String[] filePaths;
    public String[] files;

    public MyObject(String[] filePaths) {
        this.filePaths = filePaths;
        files = new NativePipe().readFiles(filePaths);
    }
}
