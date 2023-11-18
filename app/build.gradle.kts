@Suppress("DSL_SCOPE_VIOLATION") // TODO: Remove once KTIJ-19369 is fixed
plugins {
    alias(libs.plugins.com.android.application)
    alias(libs.plugins.org.jetbrains.kotlin.android)
}

android {
    namespace = "com.mgg.callbackhandler"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.mgg.callbackhandler"
        minSdk = 29
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                arguments += "-DANDROID_STL=c++_shared"
                cppFlags += "-std=c++17"
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildFeatures {
        viewBinding = true
    }

    buildFeatures.prefab = true
}
apply(
    "flatbuffers.gradle"
)
apply(
    "leveldb.gradle"
)
apply(
    "snappy.gradle"
)
apply(
    "curl.gradle"
)
apply(
    "abseil-cpp.gradle"
)
apply(
    "glog.gradle"
)
dependencies {

    implementation(libs.core.ktx)
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.test.ext.junit)
    androidTestImplementation(libs.espresso.core)
    implementation("com.jakewharton.timber:timber:5.0.1")
    implementation("com.google.flatbuffers:flatbuffers-java:23.5.26")
    implementation("com.android.ndk.thirdparty:curl:7.79.1-beta-1")
    implementation("com.android.ndk.thirdparty:jsoncpp:1.9.5-beta-1")
    implementation("com.android.ndk.thirdparty:openssl:1.1.1l-beta-1")
}