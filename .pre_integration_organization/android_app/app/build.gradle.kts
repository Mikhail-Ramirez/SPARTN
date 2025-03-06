plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.kotlin.compose)
}

android {
    namespace = "com.example.spartn1"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.example.spartn1"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    buildTypes {
        getByName("release") {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }

    buildFeatures {
        compose = true
    }
    composeOptions {
        kotlinCompilerExtensionVersion = "1.5.1"
    }
}


dependencies {
    add("implementation", libs.androidx.core.ktx)
    add("implementation", libs.androidx.lifecycle.runtime.ktx)
    add("implementation", libs.androidx.activity.compose)

    add("implementation", "org.jetbrains.kotlinx:kotlinx-coroutines-core:1.7.3")
    add("implementation", "org.jetbrains.kotlinx:kotlinx-coroutines-android:1.7.3")

    add("implementation", platform(libs.androidx.compose.bom))

    add("implementation", "androidx.compose.material:material")

    add("implementation", libs.androidx.ui)
    add("implementation", libs.androidx.ui.graphics)
    add("implementation", libs.androidx.ui.tooling.preview)

    // testImplementation => add("testImplementation", libs.junit)
    // androidTestImplementation => add("androidTestImplementation", ...)
    // debugImplementation => add("debugImplementation", ...)
    // etc.
}
