package jni;

import static jni.NativePipe.readFiles;

public class Profiles {
    public String[] filePaths;
    public byte[][] files;

    public Profiles(String[] filePaths,int accuracy) {
        this.filePaths = filePaths;
        files = readFiles(filePaths,accuracy);
    }
}
