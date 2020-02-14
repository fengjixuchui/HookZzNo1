//
// Created by z on 2018/7/7.
//
//

#include "hook_utils.h"



/* ====================== BEGIN ======================  */
//    jclass      (*FindClass)(JNIEnv*, const char*);
jclass (*FindClass_old)(JNIEnv *env, const char *cc);

jclass FindClass_new(JNIEnv *env, const char *cc) {
    LOGE("[FindClass][Native] %s", cc);
    return FindClass_old(env, cc);
}


void FindClass_hook(JNIEnv *env) {

    void *func_addr = (void *) *(long *) ((*(long *) env) + 6 * sizeof(void *));

    LOGE("[func_addr][FindClass] :%p", func_addr);


    int ret = ZzReplace((void *) func_addr, (void *) FindClass_new, (void **) &FindClass_old);

    if (ret != RS_SUCCESS) {
        LOGE("[FindClass][Native] ERROR: ZzReplace failed %d", ret);
        return;
    }
}
/* ======================  End  ======================  */


/* This function will be call when the library first be loaded */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("JNI_OnLoad could not get JNI env");
        return JNI_VERSION_1_4;
    }


    FindClass_hook(env);

    LOGE("env->FindClass(\"java/lang/String\")   %d", env->FindClass("java/lang/String"));


    return JNI_VERSION_1_4;
}