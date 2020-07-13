# How to add C/C++ files to android
1. Create a new project with empty Activity. Leave everything to be default.
2. Install NDK and CMake using steps from the following [link](https://developer.android.com/studio/projects/install-ndk)
3. Add NDK Version to app's build.gradle file. I am working on Mac and I found my NDK version in the directory `/Users/karangrover/Library/Android/sdk/ndk`. Add it as `ndkVersion "21.3.6528147"`. Sync the gradle files with the project. [ [Go to commit](https://github.com/krngrvr09/NDKTest2/commit/735da7f3ab5e64ceea462bc8878ef5578ef652f1) ]
4. In `app/src/main`, create a `cpp` folder along side java. This is where all the cpp files will go.[Go to commit](https://github.com/krngrvr09/NDKTest2/commit/7d934aad4693ce1c5d938e30e224df4137d02da7)
5. Add a `test_file.c` to the `cpp` folder and add the following code to it.[ [Go to commit](https://github.com/krngrvr09/NDKTest2/commit/7d934aad4693ce1c5d938e30e224df4137d02da7) ]

```
#include <jni.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ndktest2_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("Hello from JNI LIBS!");
}
```
6. Add a `CMakeLists.txt` to the `cpp` folder and add the following to it.[ [Go to commit](https://github.com/krngrvr09/NDKTest2/commit/7d934aad4693ce1c5d938e30e224df4137d02da7) ]

```
cmake_minimum_required(VERSION 3.4.1)

add_library(test_file SHARED
            test_file.c)

# Include libraries needed for test_file lib
target_link_libraries(test_file
                      android
                      log)
```
7. Add the following lines to app's `build.gradle` file and sync gradle with the project.[ [Go to commit](https://github.com/krngrvr09/NDKTest2/commit/7d934aad4693ce1c5d938e30e224df4137d02da7) ]
  ``` 
  // Put this inside defaultConfig block
  externalNativeBuild {
        cmake {
          arguments '-DANDROID_STL=c++_static'
          }
      }
  ```
  ```
  // Put this inside android block
  externalNativeBuild {
        cmake {
            version '3.10.2'
            path 'src/main/cpp/CMakeLists.txt'
        }
    }

  ```
  8. Inside the MainActivity(outside all the functions, at the top) add the following. [ [Go to commit](https://github.com/krngrvr09/NDKTest2/commit/b93accc913f5e4925cfa67b4a35ca6d7421bcdd1) ]
  
 ```
     public native String  stringFromJNI();
    static {
        System.loadLibrary("test_file");
    }
```
9. Now you can call the function `stringFromJNI()` in Java and it will return you a String. To test it with a Toast, add the following in `onCreate()` [ [Go to commit](https://github.com/krngrvr09/NDKTest2/commit/b93accc913f5e4925cfa67b4a35ca6d7421bcdd1) ]
```
        Toast toast = Toast.makeText(getApplicationContext(),
                stringFromJNI(),
                Toast.LENGTH_SHORT);

        toast.show();
```

# FAQs

Q. What is the extern "C" for?

A. `extern "C"` construct is a C++ specific thing, it can't be used in C. And the compiler treats your source file as a C source file. Since C++ has overloading of function names and C does not, the C++ compiler cannot just use the function name as a unique id to link to, so it mangles the name by adding information about the arguments. A C compiler does not need to mangle the name since you can not overload function names in C. When you state that a function has extern "C" linkage in C++, the C++ compiler does not add argument/parameter type information to the name used for linkage. [[link1](https://stackoverflow.com/questions/1041866/what-is-the-effect-of-extern-c-in-c)] [[link2](https://stackoverflow.com/questions/16192872/extern-c-error-2040-expected-an-identifier)]

Q. Why is the extern "C" not compiling for .c files when used without `ifdef __cplusplus`

A. As mentioned above, `extern "C"` construct is a C++ specific thing, hence it gives an error because the C compiler doesn't know how to process it. Which compiler is used for the file is automatically determined. I don't know the internals of it though.

Q. Why do I get this error `Member reference base type 'JNIEnv' (aka 'const struct JNINativeInterface *') is not a structure or union` in my cpp file?

A. If you get this error, you are trying to access a member of an entity that is not a structure or a union. "int", for example, doesnt have any members so if you try to do something like this: `int a=5; string b = a->name;`, then this error would come up. I saw this error when I was trying to use JNI function meant for C++ in a .c file or vice versa. If you check [this](https://github.com/krngrvr09/NDKTest2/commit/cd9a419ddce54d2abe490323e19cbc7c075319db) commit, then you'll see I have added two different function definitions for C/C++. If you go further into `jnni.h`, you'll see that `JNIEnv` is defined differently for C and C++ and that is where the error comes from. 

Q. What does `JNIEXPORT jstring JNICALL` do?

A. JNIEXPORT and JNICALL are macros that are defined in `jni.h`. jstring is the return type of the function. For starters, I guess you can remember that return types for C/C++ functions when used with NDK are jstring, jint, jlong, etc. Normal return-types but with "j" prefixed. JNIEXPORT is used to make native functions appear in the dynamic table of the built binary (*.so file). If these functions are not in the dynamic table, JNI will not be able to find the functions to call them so the RegisterNatives call will fail at runtime. But it depends from platform to platform. On mine, this MACRO is empty. JNICALL is an empty define on android (as above) but it's there for platform compatibility. On windows JDKs JNICALL is defined as `__stdcall`.

Q. What are the parameters passed to `stringFromJNI()`?

A. The two arguments passed are JNIEnv and jobject. I dont understand what exactly JNIEnv is for, but you can find out [here](https://developer.android.com/training/articles/perf-jni#javavm-and-jnienv). Please send a PR with explanation if you do understand. `jobject` on the other hand, is used to call Java functions from C code. I don't know how to do that yet, but that's what it's used for. Check out [this](https://github.com/android/ndk-samples/tree/17ec60701db042eff14fde3bfdfbd88ae2d3a5a4/hello-jniCallback) repo for that.

Q. So, in CMake, I add a shared test_file library, is it a library? I am also loading "test_file" as a library in MainActivity.java

A. Yes. A library is constructed out of all the C/C++ source code provided and that is then used. I dont exactly know how this works. Please send a PR if you do.

Q. What does `arguments '-DANDROID_STL=c++_static'` mean in build.gradle?

A. D flag in CMake is used to define the variable. Here the variable is `ANDROID_STL` and it is set to `c++_static`. `ANDROID_STL` will be used to set a runtime for C++.

Q. Does the CMake version number in build.gradle really matter?

A. If the version set in your app's build.gradle is lower than the `cmake_minimum_required`, then it will give an error. Also, you cannot add any random version number in your build.gradle file. It has to be the CMake version your downloaded.
