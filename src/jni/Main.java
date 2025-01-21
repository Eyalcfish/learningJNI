package jni;

import static jni.NativePipe.fileOutput;

public class Main {
    public static void main(String[] args) {
        int accuracy = 3;

        Profiles obj = new Profiles(new String[]{"C:/Users/fishministrator/IdeaProjects/learningJNI/src/jni/thing.txt"}
        ,accuracy); // puts all the files ready for read

        System.out.println(obj.files[0].length);

        double startTime = System.nanoTime();

        fileOutput(obj.files[0],accuracy);

        double endTime = System.nanoTime();
        double executionTime = (endTime - startTime)/1000000;
        System.out.println("Function takes " + executionTime + "ms");

        float[][] a = fileOutput(obj.files[0],accuracy); // float[7][x]

        for(int i = 0 ; i<1 ; i++){
            for(int j = 0 ; j<a.length ; j++){
                System.out.print(a[j][i] + " ");
            }
            System.out.println();
        }
    }
}
