package com.mgg.callbackhandler

import android.content.pm.ApplicationInfo
import android.content.pm.PackageInfo
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.mgg.callbackhandler.databinding.ActivityMainBinding
import org.chromium.base.library_loader.Linker
import timber.log.Timber
import java.io.File
import java.io.FileNotFoundException
import java.io.IOException

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val linker = Linker()
        linker.loadLibrary("callbackhandler")
        binding.sampleText.text = stringFromJNI()

        test()

        testLoader()

        testAppList()
        val test = Test()
        test.consumer(Handler(Looper.getMainLooper())) {
            Timber.e("Test JavaCallBack")
        }
        test.producer()
        test.setCompletedCallback(Handler(Looper.getMainLooper())) {
            Timber.e("Test JavaSetCompletedCallback")
        }
    }

    private fun generateLCItemFromPackageInfo(
        pi: PackageInfo
    ): LCItem {
        return LCItem(
            pi.packageName,
            pi.getAppName() ?: "null",
            pi.versionName ?: "null",
            pi.getVersionCode(),
            pi.firstInstallTime,
            pi.lastUpdateTime,
            (pi.applicationInfo.flags and ApplicationInfo.FLAG_SYSTEM) == ApplicationInfo.FLAG_SYSTEM,
            pi.applicationInfo.targetSdkVersion.toShort(),
            // variant
        )
    }

    private fun test() {
        try {
            // Open the .so file and create a FileInputStream
            val fis = assets.open("libc++_shared.so")

            // Reading the first 52 bytes to get the ELF header
            val header = ByteArray(52)
            fis.read(header)

            // Get the ELF identifier
            val magic = String(header, 0, 4)
            if (magic != "\u007fELF") {
                Log.e("ReadElf", "EI_MAG : " + "Unknown")
            } else {
                Log.e("ReadElf", "EI_MAG : " + "0x7fELF (ELF)")
            }

            // Get elf class (32-bit or 64-bit)
            when (header[4].toInt()) {
                1 -> {
                    Log.e("ReadElf", "EI_CLASS : " + "ELFCLASS32 (32-bit)")
                }

                2 -> {
                    Log.e("ReadElf", "EI_CLASS : " + "ELFCLASS64 (64-bit)")
                }

                else -> {
                    Log.e("ReadElf", "EI_CLASS : " + "ELFCLASSNONE (This class is invalid)")
                }
            }

            // Get data encoding
            when (header[5].toInt()) {
                1 -> {
                    Log.e("ReadElf", "EI_DATA : " + "ELFDATA2LSB (Little-endian)")
                }

                2 -> {
                    Log.e("ReadElf", "EI_DATA : " + "ELFDATA2MSB (Big-endian)")
                }

                else -> {
                    Log.e("ReadElf", "EI_DATA : " + "ELFDATANONE (Unknown data format)")
                }
            }

            // Get the ELF version
            val version = header[6].toInt()
            if (version == 1) {
                Log.e("ReadElf", "EI_VERSION : " + "EV_CURRENT (Current version)")
            } else {
                Log.e("ReadElf", "EI_VERSION : " + "EV_NONE (Invalid version)")
            }

            // Get the value of the EI_OSABI
            val osAbiStr = when (header[7].toInt()) {
                0 -> "ELFOSABI_NONE/ELFOSABI_SYSV (UNIX System V ABI)"
                1 -> "ELFOSABI_HPUX (HP-UX ABI)"
                2 -> "ELFOSABI_NETBSD (NetBSD ABI)"
                3 -> "ELFOSABI_LINUX (Linux ABI)"
                6 -> "ELFOSABI_SOLARIS (Solaris ABI)"
                8 -> "ELFOSABI_IRIX (IRIX ABI)"
                9 -> "ELFOSABI_FREEBSD (FreeBSD ABI)"
                10 -> "ELFOSABI_TRU64 (TRU64 UNIX ABI)"
                97 -> "ELFOSABI_ARM (ARM architecture ABI)"
                255.toByte().toInt() -> "ELFOSABI_STANDALONE (Stand-alone (embedded) ABI)"
                else -> "Unknown"
            }
            Log.e("ReadElf", "EI_OSABI : $osAbiStr")

            // Get ELF file type
            val typeStr = when (header[16] + (header[17].toInt() shl 8)) {
                1 -> "ET_REL (A Relocatable file)"
                2 -> "ET_EXEC (An Executable file)"
                3 -> "ET_DYN (A Shared object file)"
                4 -> "ET_CORE (A Core file)"
                else -> "ET_NONE (An unknown type)"
            }
            Log.e("ReadElf", "ET : $typeStr")

            // Close FileInputStream
            fis.close()
        } catch (e: FileNotFoundException) {
            Toast.makeText(applicationContext, "The file does not exist", Toast.LENGTH_LONG).show()
        } catch (e: IOException) {
            Toast.makeText(applicationContext, "Failed to read the file", Toast.LENGTH_LONG).show()
        }
    }

    private fun testLoader() {
        val ainfo = this.applicationContext.packageManager.getApplicationInfo(
            "com.mgg.callbackhandler",
            PackageManager.GET_SHARED_LIBRARY_FILES
        )
        Log.e("ReadElf", "native library dir ${ainfo.nativeLibraryDir}")
        Log.e(
            "ReadElf",
            "native library libcallbackhandler.so ${File(ainfo.nativeLibraryDir + File.separator + "libcallbackhandler.so").exists()}"
        )
    }

    private fun testAppList() {
        val appList = LocalAppDataSource.getApplicationList().toMutableList()

        val lcItems = mutableListOf<LCItem>()
        var progressCount = 0
        for (info in appList) {
            try {
                lcItems.add(generateLCItemFromPackageInfo(info))
                progressCount++
                // updateInitProgress(progressCount * 100 / appList.size)
            } catch (e: Throwable) {
                Timber.e(e, "initItems: ${info.packageName}")
                continue
            }
        }
        Log.e(
            "LocalAppDataSource",
            "ApplicationList ${lcItems.size}"
        )
    }

    /**
     * A native method that is implemented by the 'callbackhandler' native library,
     * which is packaged with this application.
     */
    private external fun stringFromJNI(): String
}