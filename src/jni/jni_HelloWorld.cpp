#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <jni.h>
#include "C:/Users/Eyal/IdeaProjects/jniAttempts/out/bonus/jni_HelloWorld.h"

using namespace std;

inline vector<vector<float>> fileOutput(string data);

vector<string> split(string x, char delim);

inline vector<vector<float>> sortValues(string data);

string jstringToStdString(JNIEnv* env, jstring jStr);

jfloatArray convertVectorToJava(JNIEnv* env, const std::vector<float>& vec);

jobjectArray create2DFloatArray(JNIEnv* env, const std::vector<jfloatArray>& inputVec);


JNIEXPORT _jobjectArray* JNICALL Java_jni_HelloWorld_fileOutput(JNIEnv* env, jobject obj,jstring data) {
    vector<vector<float>> matrix = fileOutput(jstringToStdString(env,data));
    size_t size = matrix.size();
    vector<jfloatArray> array(matrix.size());
    for(int i = 0 ; i < size ; i++) {
        array[i] = convertVectorToJava(env,matrix[i]);
    }
    return create2DFloatArray(env,array);
}

inline vector<vector<float>> fileOutput(string data) {
    return sortValues(data);
}

inline vector<vector<float>> sortValues(string data) {
    vector<vector<float>> ret(7);
    vector<string> lines = split(data,'\n');
    vector<string> tempRow(7);
    for(int i = 0 ;i < lines.size() ; i++) {
        tempRow = split(lines[i],',');
        for(int f = 0 ; f < 7 ; f++) {
            ret[f].push_back(stof(tempRow[f]));
        }
    }
    return ret;
}

inline jfloatArray convertVectorToJava(JNIEnv* env, const std::vector<float>& vec) {
    size_t size = vec.size();
    jfloatArray javaArray = env->NewFloatArray(size);
    env->SetFloatArrayRegion(javaArray, 0, size, vec.data());
    return javaArray;
}

inline jobjectArray create2DFloatArray(JNIEnv* env, const std::vector<jfloatArray>& inputVec)
{
    jclass floatArrayClass = env->FindClass("[F");
    size_t rows = inputVec.size();
    jobjectArray twoDimArray = env->NewObjectArray(rows, floatArrayClass, nullptr);
    for (size_t i = 0; i < rows; i++) {
        env->SetObjectArrayElement(twoDimArray, i, inputVec[i]);
    }
    return twoDimArray;
}

inline string jstringToStdString(JNIEnv* env, jstring jStr) {
    if (!jStr) {
        return "";
    }
    const char* utfChars = env->GetStringUTFChars(jStr, nullptr);
    string result(utfChars);
    env->ReleaseStringUTFChars(jStr, utfChars);
    return result;
}

inline vector<string> split(string x, char delim)
{
    x += delim;
    vector<string> splitted;
    string temp = "";
    for (int i = 0; i < x.length(); i++)
    {
        if (x[i] == delim)
        {
            splitted.push_back(temp);
            temp = "";
            i++;
        }
        temp += x[i];
    }
    return splitted;
}