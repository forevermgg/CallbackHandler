package com.mgg.callbackhandler;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import timber.log.Timber;

@Keep
public class Test {

    private long mNativeObject = 0;
    private Object mObject = new Object();

    Test() {
        mNativeObject = createTest();
    }

    public long getNativeObject() {
        synchronized (mObject) {
            if (mNativeObject == 0) {
                throw new IllegalStateException("Calling method on destroyed Test");
            }
            return mNativeObject;
        }
    }

    void clearNativeObject() {
        mNativeObject = 0;
    }

    private long createTest() {
        synchronized (mObject) {
            long nativeView = nCreateTest();
            if (nativeView == 0) throw new IllegalStateException("Couldn't create View");
            return nativeView;
        }
    }

    public void destroyTest() {
        synchronized (mObject) {
            Timber.e("destroyTest " + getNativeObject());
            nDestroyTest(getNativeObject());
            clearNativeObject();
        }
    }

    @Override
    protected void finalize() throws Throwable {
        synchronized (mObject) {
            if (getNativeObject() != 0) {
                destroyTest();
            }
        }
        super.finalize();
    }

    public static class PickingQueryResult {
        public int code;
        public String msg;
    }

    public interface OnPickCallback {
        void onPick(@NonNull PickingQueryResult result);
    }

    private static class InternalOnPickCallback implements Runnable {
        public InternalOnPickCallback(OnPickCallback mUserCallback) {
            this.mUserCallback = mUserCallback;
        }

        @Override
        public void run() {
            mPickingQueryResult.code = code;
            mPickingQueryResult.msg = msg;
            mUserCallback.onPick(mPickingQueryResult);
        }

        private final OnPickCallback mUserCallback;
        private final PickingQueryResult mPickingQueryResult = new PickingQueryResult();
        int code;
        String msg;
    }

    public void consumer(@Nullable Object handler, @Nullable OnPickCallback callback) {
        InternalOnPickCallback internalCallback = new InternalOnPickCallback(callback);
        nConsumer(getNativeObject(), handler, internalCallback);
    }

    public void producer() {
        nProducer(getNativeObject());
    }

    private static native void nConsumer(long nativeTest, Object handler, InternalOnPickCallback internalCallback);

    private static native void nProducer(long nativeTest);

    private static native long nCreateTest();

    private static native boolean nDestroyTest(long nativeTest);
}
