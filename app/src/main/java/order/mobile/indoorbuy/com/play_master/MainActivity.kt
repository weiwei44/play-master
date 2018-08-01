package order.mobile.indoorbuy.com.play_master

import android.Manifest
import android.annotation.TargetApi
import android.os.Build
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.support.annotation.RequiresApi
import com.tbruyelle.rxpermissions2.RxPermissions
import kotlinx.android.synthetic.main.activity_main.*
import order.mobile.indoorbuy.com.play_master.play.XPlay

class MainActivity : AppCompatActivity() {

    private val dirPath = "${Environment.getExternalStorageDirectory().absolutePath}/weiwei/v1080.mp4"

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        requestPermission()
        val xpaly = XPlay()
        xpaly.init(dirPath)
    }

    /**
     * 权限获取
     */
    @RequiresApi(Build.VERSION_CODES.JELLY_BEAN)
    private fun requestPermission(){
        RxPermissions(this)
                .requestEach(
                        Manifest.permission.WRITE_EXTERNAL_STORAGE,
                        Manifest.permission.READ_EXTERNAL_STORAGE
                )
                .subscribe{
                    when{
                        it.granted -> {
                            // 用户已经同意该权限

                        }
                        it.shouldShowRequestPermissionRationale -> {
                            // 用户拒绝了该权限，没有选中『不再询问』（Never ask again）,那么下次再次启动时，还会提示请求权限的对话框
                        }
                        else -> {
                            // 用户拒绝了该权限，并且选中『不再询问』
                        }
                    }
                }
    }
}
