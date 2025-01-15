#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <jni.h>
#include "C:/Users/Eyal/IdeaProjects/jniAttempts/out/bonus/jni_NativePipe.h"

#define getBit(num,bit) (1 == ( (num >> bit) & 1))
#define setBit(num,bit,bol) (num |= bol << bit)

using namespace std;

int get_file_size(std::string filename);

vector<string> compress(vector<string> contents);

string decompress(const string& content);

inline vector<string> readFiles(vector<string> paths);

inline vector<vector<float>> fileOutput(string data);

std::vector<std::string> split(const std::string& input, char delimiter);

inline vector<vector<float>> sortValues(string data);

string jstringToStdString(JNIEnv* env, jstring jStr);

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
    string s;
    for(int i = 0; i < amountOfPaths ; i++) {
        ifstream file;
        file.open(paths[i]);
        getline(file, s);
        while (getline(file, s))
            contents[i] += s + '\n';
        file.close();
    }
    //return contents;//compress(contents);
    return compress(contents);
    //return {"1"};
}

// HelloWorld:
// public float[][] fileOutput(String data)
// CPP
inline vector<vector<float>> fileOutput(string data) {
    //return sortValues(data);//decompress(data));
    return sortValues(decompress(data));
    //string str = decompress(data);
    //string str = "1,2,3,4,5,6,7,\n1,2,3,4,5,6,7";
    //sortValues(str);
    //decompress(data)
    //return {{1}};
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

inline string jstringToStdString(JNIEnv* env, jstring jStr) {
    if (!jStr) {
        return "";
    }
    const char* utfChars = env->GetStringUTFChars(jStr, nullptr);
    string result(utfChars);
    env->ReleaseStringUTFChars(jStr, utfChars);
    return result;
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

int get_file_size(std::string filename) {
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

vector<string> compress(vector<string> contents) {
    vector<string> ret(contents.size());
    int a = 0;
    for(string str : contents) {
        int size = (str.length()+1)/2;
        char buffer = 0;
        string value = "";
        for(int i = 0 ; i < size ; i++) {
            char num = 0;
            for(int f = 0 ; f < 2 ; f++) {
                char curchar = str[i*2+f];
                switch(curchar){
                    case ',':
                        num =  10;
                        break;
                    case '-':
                        num = 11;
                        break;
                    case '.':
                        num = 12;
                        break;
                    case '\n':
                        num = 13;
                        break;
                    case 'E':
                        num = 14;
                        break;
                    default:
                        num = curchar-'0';
                }
                for(int j = 0 ; j < 4 ; j++) {
                    size_t place = j;
                    //cout << getBit(num,place);
                    setBit(buffer,place+f*4,getBit(num,place));
                }
                //cout << endl;
            }
            value += buffer;
            buffer = 0;
        }
        ret[a] = value;
    }
    return ret;
}

string decompress(const string& content) {
    size_t strSize = content.length();
    string value;
    value.resize(strSize * 2);
    static constexpr char lookup[15] = {
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',',', '-', '.', '\n', 'E'
    };
    for (size_t f = 0; f < strSize; ++f) {
        char curchar = content[f];
        char buffer1 = 0;
        char buffer2 = 0;
        for (int j = 0; j < 4; ++j) {
            setBit(buffer1, j, getBit(curchar, j));
            setBit(buffer2, j, getBit(curchar, j + 4));
        }
        buffer1 = (buffer1 >= 10 && buffer1 <= 14) ? lookup[buffer1] : buffer1 + '0';
        buffer2 = (buffer2 >= 10 && buffer2 <= 14) ? lookup[buffer2] : buffer2 + '0';
        value[f * 2] = buffer1;
        value[f * 2 + 1] = buffer2;
    }
    return value;
}
