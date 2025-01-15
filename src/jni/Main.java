package jni;

import static jni.NativePipe.fileOutput;


public class Main {
    public static void main(String[] args) {
        Profiles obj = new Profiles(new String[]{"C:/Users/Eyal/IdeaProjects/jniAttempts/src/jni/thing.txt"}); // puts all the files ready for read
        System.out.println(obj.files[0].length);
        float[][] a = fileOutput(obj.files[0]); // float[7][x]
    }
}