#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <jni.h>
#include "C:/Users/fishministrator/IdeaProjects/learningJNI/out/bonus/jni_NativePipe.h"

#define getBit(num,bit) (1 == ( (num >> bit) & 1))
#define setBit(num,bit,bol) (num |= bol << bit)

using namespace std;

vector<vector<float>> decompress_matrix(string str);

string compress_matrix(string str);

inline vector<string> readFiles(vector<string> paths);

inline vector<vector<float>> fileOutput(string data);

std::vector<std::string> split(const std::string& input, char delimiter);

inline vector<vector<float>> sortValues(string data);

jfloatArray convertVectorToJava(JNIEnv* env, const std::vector<float>& vec);

jobjectArray create2DFloatArray(JNIEnv* env, const std::vector<jfloatArray>& inputVec);

jobjectArray convertVectorToByteArray(JNIEnv* env, vector<string>& vector);

vector<string> jobjectArrayToVector(JNIEnv* env, jobjectArray jArray);


// NativePipe:
// public float[][] fileOutput(String data)
// Native implementation
JNIEXPORT _jobjectArray* JNICALL Java_jni_NativePipe_fileOutput(JNIEnv* env, jclass obj,jbyteArray data) {
    jsize length = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    std::string str(reinterpret_cast<char*>(bytes), length);
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    //
    vector<vector<float>> matrix = fileOutput(str);
    size_t size = matrix.size();
    vector<jfloatArray> array(matrix.size());
    for(int i = 0 ; i < size ; i++) {
        array[i] = convertVectorToJava(env,matrix[i]);
    }
    return create2DFloatArray(env,array);
}

// NativePipe:
// public String[] readFiles(String[] paths)
// Native implementation
JNIEXPORT jobjectArray JNICALL Java_jni_NativePipe_readFiles(JNIEnv * env, jclass obj, jobjectArray paths) {
    vector<string> CPP_paths = jobjectArrayToVector(env,paths);
    vector<string> datas = readFiles(CPP_paths);
    return convertVectorToByteArray(env,datas);
}

// HelloWorld:
// public string[] readFiles(String[] paths)
// CPP
inline vector<string> readFiles(vector<string> paths) {
    size_t amountOfPaths = paths.size();
    vector<string> contents(amountOfPaths);
    vector<string> rets(amountOfPaths);
    string s;
    for(int i = 0; i < amountOfPaths ; i++) {
        ifstream file;
        file.open(paths[i]);
        getline(file, s);
        while (getline(file, s))
            contents[i] += s + '\n';
        file.close();
    }
    for(int i = 0 ; i < amountOfPaths ; i++) {
        rets[i] = compress_matrix(contents[i]);
    }
    return rets;
}

// HelloWorld:
// public float[][] fileOutput(String data)
// CPP
inline vector<vector<float>> fileOutput(string data) {
    return decompress_matrix(data);
}

inline vector<vector<float>> sortValues(string data) {
    vector<vector<float>> ret(7);
    vector<string> lines = split(data,'\n');
    size_t linesAmount = lines.size();
    for(int i = 0 ; i < 7 ; i++) {
        ret[i] = vector<float>(linesAmount);
    }
    for(int i = 0 ; i < linesAmount ; i++) {
        vector<string> splited = split(lines[i],',');
        for(int f = 0 ; f < 7 ; f++) {
            ret[f][i] = stof(splited[f]);
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

inline jobjectArray create2DFloatArray(JNIEnv* env, const std::vector<jfloatArray>& inputVec){
    jclass floatArrayClass = env->FindClass("[F");
    size_t rows = inputVec.size();
    jobjectArray twoDimArray = env->NewObjectArray(rows, floatArrayClass, nullptr);
    for (size_t i = 0; i < rows; i++) {
        env->SetObjectArrayElement(twoDimArray, i, inputVec[i]);
    }
    return twoDimArray;
}

std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    result.reserve(10); // Initial reserve (adjust based on expected size if known)

    size_t start = 0;
    size_t end;

    while ((end = input.find(delimiter, start)) != string::npos) {
        result.emplace_back(input, start, end - start); // Efficient in-place construction
        start = end + 1;
    }

    if (start < input.size()) { // Add the last segment
        result.emplace_back(input, start, input.size() - start);
    }

    return result;
}

jobjectArray convertVectorToByteArray(JNIEnv* env, vector<string>& vector) {
    jclass byteArrayClass = env->FindClass("[B");
    jobjectArray outerArray = env->NewObjectArray(vector.size(), byteArrayClass, nullptr);
    for (size_t i = 0; i < vector.size(); ++i) {
        const std::string& str = vector[i];
        jbyteArray innerArray = env->NewByteArray(str.size());
        env->SetByteArrayRegion(innerArray, 0, str.size(), reinterpret_cast<const jbyte*>(str.data()));
        env->SetObjectArrayElement(outerArray, i, innerArray);
        env->DeleteLocalRef(innerArray);
    }
    return outerArray;
}

vector<string> jobjectArrayToVector(JNIEnv* env, jobjectArray jArray) {
    vector<string> vec;

    jsize arrayLength = env->GetArrayLength(jArray);
    for (jsize i = 0; i < arrayLength; ++i) {
        jstring jStr = (jstring)env->GetObjectArrayElement(jArray, i);
        const char* cStr = env->GetStringUTFChars(jStr, nullptr);
        if (cStr) {
            vec.emplace_back(cStr);
            env->ReleaseStringUTFChars(jStr, cStr);
        }
        env->DeleteLocalRef(jStr);
    }
    return vec;
}

inline float bytesToFloat(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3)
{
    float output;

    *((unsigned char*)(&output) + 3) = b0;
    *((unsigned char*)(&output) + 2) = b1;
    *((unsigned char*)(&output) + 1) = b2;
    *((unsigned char*)(&output) + 0) = b3;

    return output;
}

string compress_matrix(string str) {
    vector<vector<float>> matrix = sortValues(str);
    size_t size = matrix[0].size();
    string ret;
    ret.resize(7*size*4);
    for(int i = 0 ; i < 7 ; i++) {
        for(int f = 0 ; f < size ; f++) {
            float cur = matrix[i][f];
            unsigned char buffer[4]={0,0,0,0};
            memcpy(buffer,&cur,4);
            size_t place = i*size*4+f*4;
            for(int j = 0; j < 4 ; j++) {
                ret[place+j] = buffer[j];
            }
        }
    }
    return ret;
}

vector<vector<float>> decompress_matrix(string str) {
    size_t size = str.size();
    size_t relativeSize = size/28;
    vector<vector<float>> matrix(7);
    for(int i = 0; i < 7 ; i++) {
        matrix[i].resize(relativeSize);
    }
    for(int i = 0 ; i < 7 ; i++) {
        for(int f = 0; f < relativeSize ; f++) {
            short place = i*relativeSize*4+f*4;
            matrix[i][f] = bytesToFloat(str[place + 3], str[place + 2], str[place + 1], str[place]);
        }
    }

    return matrix;
}