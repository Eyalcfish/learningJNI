@echo off

set OutputPath=C:\Users\Eyal\IdeaProjects\jniAttempts\out\bonus
set NativePackagePath=C:\Users\Eyal\IdeaProjects\jniAttempts\src\jni

set Path_ExtraFiles=%NativePackagePath%\MyObject.java
set Path_MainFile=%NativePackagePath%\Main.java
set Path_NativeFile=%NativePackagePath%\HelloWorld.java

set Full_NativeFile=jni_HelloWorld
set Full_MainFile=jni.Main

set JAVA_HOME=C:\Program Files\Java\jdk-23

cd %NativePackagePath%

javac -d %OutputPath% %Path_NativeFile% %Path_MainFile% %Path_ExtraFiles%

javac -h %OutputPath% -d %OutputPath% %Path_NativeFile%

g++ -c -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" %Full_NativeFile%.cpp -o %OutputPath%\%Full_NativeFile%.o

cd %OutputPath%
g++ -shared -o native.dll %Full_NativeFile%.o -Wl,--add-stdcall-alias

java -cp %OutputPath% -Djava.library.path="%OutputPath%" %Full_MainFile%
