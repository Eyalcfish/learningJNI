package jni;

import static jni.NativePipe.fileOutput;

public class Main {
    public static void main(String[] args) {
        Profiles obj = new Profiles(new String[]{"C:/Users/fishministrator/IdeaProjects/learningJNI/src/jni/thing.txt"}); // puts all the files ready for read

        double startTime = System.nanoTime();

        fileOutput(obj.files[0]);

        double endTime = System.nanoTime();
        double executionTime = (endTime - startTime)/1000000;
        System.out.println("Function takes " + executionTime + "ms");

        float[][] a = fileOutput(obj.files[0]); // float[7][x]

        System.out.println(a[0][0]);
    }
}
