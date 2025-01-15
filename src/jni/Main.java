package jni;

public class Main {
    public static void main(String[] args) {
        MyObject obj = new MyObject(new String[]{"C:/Users/Eyal/IdeaProjects/jniAttempts/src/jni/thing.txt"}); // puts all the files ready for read
        System.out.println(obj.files[0].length);
        float[][] a = new NativePipe().fileOutput(obj.files[0]); // float[7][x]
        System.out.println(a[4][1]);
    }
}