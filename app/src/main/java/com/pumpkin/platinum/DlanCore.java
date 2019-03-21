package com.pumpkin.platinum;

/**
 * @author: zhibao.Liu
 * @version:
 * @date: 2019/3/21 16:45
 * @des: 这个类用于dlan封装的jni接口核心类
 * @see {@link }
 */

public class DlanCore {

    static {
        System.loadLibrary("dlan-platinum");
    }

    public static native int startMediaRender(byte[] friendname ,byte[] uuid);
    public static native int stopMediaRender();
    public static native boolean respActionEvent(int cmd, byte[] value ,byte[] data);
    public static native boolean enableLogPrint(boolean flag);

}
