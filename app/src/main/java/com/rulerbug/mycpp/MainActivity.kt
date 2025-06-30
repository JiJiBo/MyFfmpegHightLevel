package com.rulerbug.mycpp

import android.Manifest
import android.app.Activity
import android.content.ContentUris
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.MediaStore
import android.util.Log
import android.view.SurfaceView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.rulerbug.myldemo.Demo
import java.io.File
class MainActivity : AppCompatActivity() {

    private val REQUEST_OPEN_DOCUMENT = 1001

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 打开文件选择器
        openFilePicker()
    }

    private fun openFilePicker() {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "*/*"  // 可选为 "audio/*" 或指定 "audio/mpeg"
        }
        startActivityForResult(intent, REQUEST_OPEN_DOCUMENT)
    }
    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == REQUEST_OPEN_DOCUMENT && resultCode == RESULT_OK) {
            val uri = data?.data ?: return
            val filePath = getPathFromUri(this, uri)
            if (filePath != null) {
                Log.d("路径", "真实路径是：$filePath")
                val demo = Demo()
                val surfaceView = findViewById<SurfaceView>(R.id.sv)
                demo.play(filePath, surfaceView)
            } else {
                Toast.makeText(this, "无法获取文件路径", Toast.LENGTH_SHORT).show()
            }
        }
    }

    fun getPathFromUri(context: Context, uri: Uri): String? {
        // DocumentProvider
        if (DocumentsContract.isDocumentUri(context, uri)) {
            val docId = DocumentsContract.getDocumentId(uri)
            when {
                uri.authority == "com.android.externalstorage.documents" -> {
                    val split = docId.split(":")
                    val type = split[0]
                    if (type.equals("primary", ignoreCase = true)) {
                        return "${Environment.getExternalStorageDirectory()}/${split[1]}"
                    }
                }
                uri.authority == "com.android.providers.downloads.documents" -> {
                    val contentUri = ContentUris.withAppendedId(
                        Uri.parse("content://downloads/public_downloads"),
                        docId.toLongOrNull() ?: return null
                    )
                    return getDataColumn(context, contentUri, null, null)
                }
                uri.authority == "com.android.providers.media.documents" -> {
                    val split = docId.split(":")
                    val contentUri = when (split[0]) {
                        "audio" -> MediaStore.Audio.Media.EXTERNAL_CONTENT_URI
                        "video" -> MediaStore.Video.Media.EXTERNAL_CONTENT_URI
                        "image" -> MediaStore.Images.Media.EXTERNAL_CONTENT_URI
                        else -> null
                    }
                    return contentUri?.let {
                        getDataColumn(context, it, "_id=?", arrayOf(split[1]))
                    }
                }
            }
        } else if (uri.scheme.equals("content", ignoreCase = true)) {
            return getDataColumn(context, uri, null, null)
        } else if (uri.scheme.equals("file", ignoreCase = true)) {
            return uri.path
        }
        return null
    }

    fun getDataColumn(context: Context, uri: Uri, selection: String?, selectionArgs: Array<String>?): String? {
        val projection = arrayOf(MediaStore.MediaColumns.DATA)
        context.contentResolver.query(uri, projection, selection, selectionArgs, null)?.use { cursor ->
            if (cursor.moveToFirst()) {
                val columnIndex = cursor.getColumnIndexOrThrow(MediaStore.MediaColumns.DATA)
                return cursor.getString(columnIndex)
            }
        }
        return null
    }

}
