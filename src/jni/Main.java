package jni;

import static jni.NativePipe.fileOutput;


public class Main {
    public static void main(String[] args) {
        Profiles obj = new Profiles(new String[]{"C:/Users/Eyal/IdeaProjects/jniAttempts/src/jni/thing.txt",
                "C:/Users/Eyal/IdeaProjects/jniAttempts/src/jni/thing2.txt"}); // puts all the files ready for read



        long startTime = System.nanoTime();

        fileOutput(obj.files[0]);

        long endTime = System.nanoTime();
        long executionTime = (endTime - startTime);
        System.out.println("Function takes " + executionTime);


        float[][] a = fileOutput(obj.files[0]); // float[7][x]

        System.out.println(a[0][0]);
    }
}