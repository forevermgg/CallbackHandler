////////////////////////////////////////////////////////////////////////////////////////////////////
// ANDROID SPECIFIC BITS
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "common/CallbackUtils.h"
#include "VirtualMachineEnv.h"
#include <jni.h>
#include <android/bitmap.h>

#define BITMAP_CONFIG_ALPHA_8 0
#define BITMAP_CONFIG_RGB_565 1
#define BITMAP_CONFIG_RGBA_4444 2
#define BITMAP_CONFIG_RGBA_8888 3
#define BITMAP_CONFIG_RGBA_F16 4
#define BITMAP_CONFIG_HARDWARE 5

//! Pixel Data Format
enum class PixelDataFormat : uint8_t {
    R,                  //!< One Red channel, float
    R_INTEGER,          //!< One Red channel, integer
    RG,                 //!< Two Red and Green channels, float
    RG_INTEGER,         //!< Two Red and Green channels, integer
    RGB,                //!< Three Red, Green and Blue channels, float
    RGB_INTEGER,        //!< Three Red, Green and Blue channels, integer
    RGBA,               //!< Four Red, Green, Blue and Alpha channels, float
    RGBA_INTEGER,       //!< Four Red, Green, Blue and Alpha channels, integer
    UNUSED,             // used to be rgbm
    DEPTH_COMPONENT,    //!< Depth, 16-bit or 24-bits usually
    DEPTH_STENCIL,      //!< Two Depth (24-bits) + Stencil (8-bits) channels
    ALPHA               //! One Alpha channel, float
};

//! Pixel Data Type
enum class PixelDataType : uint8_t {
    UBYTE,                //!< unsigned byte
    BYTE,                 //!< signed byte
    USHORT,               //!< unsigned short (16-bit)
    SHORT,                //!< signed short (16-bit)
    UINT,                 //!< unsigned int (32-bit)
    INT,                  //!< signed int (32-bit)
    HALF,                 //!< half-float (16-bit float)
    FLOAT,                //!< float (32-bits float)
    COMPRESSED,           //!< compressed pixels, @see CompressedPixelDataType
    UINT_10F_11F_11F_REV, //!< three low precision floating-point numbers
    USHORT_565,           //!< unsigned int (16-bit), encodes 3 RGB channels
    UINT_2_10_10_10_REV,  //!< unsigned normalized 10 bits RGB, 2 bits alpha
};

class AutoBitmap : public JniCallback {
 private:
  AutoBitmap(JNIEnv* env, jobject bitmap) noexcept
      : JniCallback(), mBitmap(env->NewGlobalRef(bitmap)) {
    if (mBitmap) {
      AndroidBitmap_getInfo(env, mBitmap, &mInfo);
      AndroidBitmap_lockPixels(env, mBitmap, &mData);
    }
  }

  AutoBitmap(JNIEnv* env, jobject bitmap, jobject handler,
             jobject runnable) noexcept
      : JniCallback(env, handler, runnable),
        mBitmap(env->NewGlobalRef(bitmap)) {
    if (mBitmap) {
      AndroidBitmap_getInfo(env, mBitmap, &mInfo);
      AndroidBitmap_lockPixels(env, mBitmap, &mData);
    }
  }

  void release(JNIEnv* env) {
    if (mBitmap) {
      AndroidBitmap_unlockPixels(env, mBitmap);
      env->DeleteGlobalRef(mBitmap);
    }
  }

  ~AutoBitmap() override = default;

 public:
  void* getData() const noexcept { return mData; }

  size_t getSizeInBytes() const noexcept { return mInfo.height * mInfo.stride; }

  PixelDataFormat getFormat(int format) const noexcept {
    // AndroidBitmapInfo does not capture the HARDWARE and RGBA_F16 formats
    // so we switch on the Bitmap.Config values directly
    switch (format) {
      case BITMAP_CONFIG_ALPHA_8:
        return PixelDataFormat::ALPHA;
      case BITMAP_CONFIG_RGB_565:
        return PixelDataFormat::RGB;
      default:
        return PixelDataFormat::RGBA;
    }
  }

  PixelDataType getType(int format) const noexcept {
    switch (format) {
      case BITMAP_CONFIG_RGB_565:
        return PixelDataType::USHORT_565;
      case BITMAP_CONFIG_RGBA_F16:
        return PixelDataType::HALF;
      default:
        return PixelDataType::UBYTE;
    }
  }

  // create a AutoBitmap
  static AutoBitmap* make(JNIEnv* env, jobject bitmap, jobject handler,
                          jobject runnable) {
    return new AutoBitmap(env, bitmap, handler, runnable);
  }

  // execute the callback on the java thread and destroy ourselves
  static void invoke(void*, size_t, void* user) {
    auto* autoBitmap = reinterpret_cast<AutoBitmap*>(user);
    JNIEnv* env = VirtualMachineEnv::get().getEnvironment();
    releaseCallbackJni(env, autoBitmap->mCallbackUtils, autoBitmap->mHandler,
                       autoBitmap->mCallback);
    autoBitmap->release(env);
    delete autoBitmap;
  }

  // create a AutoBitmap without a handler
  static AutoBitmap* make(JNIEnv* env, jobject bitmap) {
    return new AutoBitmap(env, bitmap);
  }

  // just destroy ourselves
  static void invokeNoCallback(void*, size_t, void* user) {
    auto* autoBitmap = reinterpret_cast<AutoBitmap*>(user);
    JNIEnv* env = VirtualMachineEnv::get().getEnvironment();
    autoBitmap->release(env);
    delete autoBitmap;
  }

 private:
  void* mData = nullptr;
  jobject mBitmap{};
  AndroidBitmapInfo mInfo{};
};
