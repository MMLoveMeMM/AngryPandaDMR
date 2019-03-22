#include <assert.h>
#include <jni.h>
#include <string.h>
#include <sys/types.h>
#include "Platinum.h"
#include "PltMediaRenderer.h"
#include "PltUPnP.h"
#include "PltDeviceHost.h"
#include "PltFileMediaServer.h"
#include "PltMediaRendererDelegate.h"
#include "PltService.h"

#include <android/log.h>

#include<android/log.h>
#define TAG "Platinum-jni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)


PLT_DeviceHostReference device;
PLT_UPnP upnp;

PltMediaRendererDelegate *mediaRenderDelegate;
PLT_MediaRenderer *mediaRenderer;

JavaVM *g_vm = NULL;
jclass g_inflectClass = NULL;
jmethodID g_methodID = NULL;

NPT_SET_LOCAL_LOGGER("platinum.android.jni")

void InitInflectClass(JavaVM* vm)
{
  	g_vm = vm;

    JNIEnv *env = NULL;
    int status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if(status != 0)
    {
        return ;
    }
    // com.pumpkin.platinum
    jclass inflectClass = env->FindClass("com/pumpkin/platinum/DlanCore");

    if (inflectClass == NULL)
    {
    	LOGI("inflectClass == NULL return");
        return ;
    }
    g_inflectClass = (jclass)env->NewGlobalRef(inflectClass);
    jmethodID methodID = env->GetStaticMethodID(inflectClass, "respActionEvent", "(ILjava/lang/String;Ljava/lang/String;)V");
    if (methodID == NULL)
    {
    	LOGI("methodID == NULL return");
        return ;
    }
	g_methodID = methodID;
	return ;
}

/*----------------------------------------------------------------------
|    JNI_OnLoad
+---------------------------------------------------------------------*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    NPT_LogManager::GetDefault().Configure("plist:.level=FINE;.handlers=ConsoleHandler;.ConsoleHandler.outputs=2;.ConsoleHandler.colors=false;.ConsoleHandler.filter=59");
	LOGI("jni_OnLoad....");

	InitInflectClass(vm);

	return JNI_VERSION_1_4;
}

char* ConvertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray, jbyte *&bytes)
{
	char *chars = NULL;
	bytes = env->GetByteArrayElements(bytearray, 0);
	int chars_len = env->GetArrayLength(bytearray);
	chars = new char[chars_len + 1];
	memcpy(chars, bytes, chars_len);
	chars[chars_len] = 0;
	return chars;
}


/*
 * Class:     com_pumpkin_platinum_DlanCore
 * Method:    startMediaRender
 * Signature: ([B[B)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_platinum_DlanCore_startMediaRender
  (JNIEnv *env, jclass clazz, jbyteArray name, jbyteArray uuid){

    NPT_LOG_INFO("start");
    int ret = -1;

    jbyte *bytes;
    char *c_name = ConvertJByteaArrayToChars(env, name, bytes);
    jbyte *bytes2;
    char *c_uuid = ConvertJByteaArrayToChars(env, uuid, bytes2);

    LOGI("name =  %s,uuid = %s",c_name,c_uuid);

    if(mediaRenderDelegate == NULL)
        mediaRenderDelegate = new PltMediaRendererDelegate();

    mediaRenderer= new PLT_MediaRenderer(c_name,false,c_uuid);
    mediaRenderer->SetDelegate(mediaRenderDelegate);

    PLT_DeviceHostReference device1(mediaRenderer);
    device = device1;

    upnp.AddDevice(device);
    ret = upnp.Start();
    env->ReleaseByteArrayElements(name, bytes, JNI_ABORT);
    env->ReleaseByteArrayElements(uuid, bytes2, JNI_ABORT);
    delete c_name;
    delete c_uuid;

    return ret;

  }

/*
 * Class:     com_pumpkin_platinum_DlanCore
 * Method:    stopMediaRender
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_platinum_DlanCore_stopMediaRender
  (JNIEnv *env, jclass clazz){
    NPT_LOG_INFO("stop");
    LOGI("stop");

    upnp.RemoveDevice(device);
    upnp.Stop();

    return 0;
  }

/*
 * Class:     com_pumpkin_platinum_DlanCore
 * Method:    respActionEvent
 * Signature: (I[B[B)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pumpkin_platinum_DlanCore_respActionEvent
  (JNIEnv *env, jclass clazz, jint cmd, jbyteArray value, jbyteArray data){
        //    NPT_LOG_INFO("response");
        jbyte *bytes;
        char *c_value = ConvertJByteaArrayToChars(env, value, bytes);
        jbyte *bytes2;
        char *c_data = ConvertJByteaArrayToChars(env, data, bytes2);

        LOGI("ResponseGenaEvent value = %s data = %s",c_value,c_data);

        mediaRenderer->UpdateServices(c_value,c_data);

        env->ReleaseByteArrayElements(value, bytes, JNI_ABORT);
        env->ReleaseByteArrayElements(data, bytes2, JNI_ABORT);
        delete c_value;
        delete c_data;

        return true;
  }

/*
 * Class:     com_pumpkin_platinum_DlanCore
 * Method:    enableLogPrint
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pumpkin_platinum_DlanCore_enableLogPrint
  (JNIEnv *env, jclass clazz, jboolean enable){
      NPT_LogManager::GetDefault().Configure("plist:.level=FINE;.handlers=ConsoleHandler;.ConsoleHandler.outputs=2;.ConsoleHandler.colors=false;.ConsoleHandler.filter=59");
      return true;
  }