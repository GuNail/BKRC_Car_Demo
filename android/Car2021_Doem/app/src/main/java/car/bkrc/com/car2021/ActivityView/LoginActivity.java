package car.bkrc.com.car2021.ActivityView;

import android.Manifest;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;

import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.provider.Settings;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;

import car.bkrc.com.car2021.Utils.CameraUtile.CameraSearchService;
import car.bkrc.com.car2021.Utils.CameraUtile.XcApplication;
import car.bkrc.com.car2021.MessageBean.DataRefreshBean;
import car.bkrc.com.car2021.Utils.OtherUtil.CameraConnectUtil;
import car.bkrc.com.car2021.Utils.OtherUtil.ToastUtil;
import car.bkrc.com.car2021.Utils.OtherUtil.WiFiStateUtil;
import car.bkrc.com.car2021.R;

public class LoginActivity extends AppCompatActivity implements View.OnClickListener {
    private EditText device_edit = null;
    private EditText login_edit = null;
    private EditText passwd_edit = null;
    private ToastUtil toastUtil;

    private Button bt_connect = null;
    private CheckBox rememberbox = null, wifi_box = null, uart_box = null;

    private ProgressDialog dialog = null;
    private CameraConnectUtil cameraConnectUtil;

    public static Context context;

    @RequiresApi(api = Build.VERSION_CODES.M)
    void Request() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);
            } else {
                if (getConnectWifiSsid() == "<unknown ssid>") {
                    toastUtil.ShowToast("????????????WiFi???" + getConnectWifiSsidTwo());
                } else toastUtil.ShowToast("????????????WiFi???" + getConnectWifiSsid());
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case 1:
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // TODO request success
                    if (getConnectWifiSsid() == "<unknown ssid>") {
                        toastUtil.ShowToast("??????????????????WiFi??????????????????WiFi????????????");
                    } else toastUtil.ShowToast("????????????WiFi???" + getConnectWifiSsid());
                }
                break;
        }
    }

    // ????????????????????????????????????
    private void goToAppSetting() {
        Intent intent = new Intent();
        intent.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        Uri uri = Uri.fromParts("package", getPackageName(), null);
        intent.setData(uri);
        startActivityForResult(intent, 1);
    }

    // ??????????????????
    private String[] permissions = {Manifest.permission.WRITE_EXTERNAL_STORAGE};
    /**
     * ????????????????????????sd????????????
     */
    private void checkReadSd() {
        // ???????????????????????????????????? 23 ?????????????????????????????????????????????
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            // ?????????????????????????????????
            int i = ContextCompat.checkSelfPermission(this, permissions[0]);
            // ?????????????????? ?????? GRANTED---??????  DINIED---??????
            if (i != PackageManager.PERMISSION_GRANTED) {
                // ??????????????????????????????????????????????????????
                ActivityCompat.requestPermissions(this, permissions, 1);
                // ?????????????????????????????????
                Toast.makeText(this, "???????????????????????????????????????", Toast.LENGTH_LONG).show();
                goToAppSetting();
            }
        }
    }



    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        context=this.getBaseContext();
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);
        checkReadSd();
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);// ????????????
        // ?????????????????????
        if (isPad(this)) {
            setContentView(R.layout.activity_login);
        } else {
            setContentView(R.layout.activity_login_mobilephone);
        }
        EventBus.getDefault().register(this); // EventBus????????????
        cameraConnectUtil = new CameraConnectUtil(this);
        findViews();  //???????????????
        cameraConnectUtil.cameraInit();//??????????????????
        Request();
    }


    /**
     * ?????????????????????????????????????????????????????? Google I/O App for Android
     *
     * @param context
     * @return ???????????? True??????????????? False
     */
    public static boolean isPad(Context context) {
        return (context.getResources().getConfiguration().screenLayout
                & Configuration.SCREENLAYOUT_SIZE_MASK)
                >= Configuration.SCREENLAYOUT_SIZE_LARGE;
    }

    private static String ZIKU_PATH = getSDPath() + java.io.File.separator
            + "res_data/tessdata";

    @Override
    protected void onResume() {
        super.onResume();
        Log.d("TAG", "onResume: "+ZIKU_PATH);
        if(!new File(ZIKU_PATH).exists()){
            Toast.makeText(this, "??????????????????????????????", Toast.LENGTH_SHORT).show();
            CopyAssets(ZIKU_PATH);
        }
    }

    private void CopyAssets( final String dir) {
        final String[] files;
        try {
            // ??????Assets?????????????????????,????????????????????????""
            files = this.getResources().getAssets().list("");
        } catch (IOException e1) {
            return;
        }
        final File mWorkingPath = new File(dir);
        // ???????????????????????????
        if (!mWorkingPath.exists()) {
            // ???????????????
            if (!mWorkingPath.mkdirs()) {
                // ?????????????????????????????????
                Toast.makeText(this, "????????????????????????????????????????????????????????????", Toast.LENGTH_SHORT).show();
            }
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < files.length; i++) {
                    try {
                        // ???????????????????????????
                        String fileName = files[i];
                        if (fileName.contains(".traineddata"))
                        {
                            File outFile = new File(mWorkingPath, fileName);
                            if (outFile.exists())
                                outFile.delete();
                            InputStream in = null;
                            in = getAssets().open(fileName);// ????????????

                            OutputStream out = new FileOutputStream(outFile);
                            // Transfer bytes from in to out
                            byte[] buf = new byte[1024];
                            int len;
                            while ((len = in.read(buf)) > 0) {
                                out.write(buf, 0, len);     // ????????????
                            }
                            out.flush();
                            in.close();
                            out.close();
                            FirstActivity.toastUtil.ShowToast("???????????????");
                        }
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }



    /**
     * ??????sd????????????
     * @return ??????????????????
     */
    public static String getSDPath() {

        File sdDir = null;
        boolean sdCardExist = Environment.getExternalStorageState().equals(
                android.os.Environment.MEDIA_MOUNTED); // ??????sd???????????????
        if (sdCardExist) {
            sdDir = Environment.getExternalStorageDirectory();// ??????????????????
        }
        return sdDir.toString();
    }



    private void findViews() {
        toastUtil = new ToastUtil(this);
        device_edit = findViewById(R.id.deviceid);
        login_edit = findViewById(R.id.loginname);
        passwd_edit = findViewById(R.id.loginpasswd);
        Button bt_reset = findViewById(R.id.reset);
        bt_connect = findViewById(R.id.connect);
        rememberbox = findViewById(R.id.remember);
        wifi_box = findViewById(R.id.wifi_each);
        uart_box = findViewById(R.id.uart_each);

        bt_reset.setOnClickListener(this);
        bt_connect.setOnClickListener(this);
        rememberbox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    passwd_edit.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                } else {
                    passwd_edit.setTransformationMethod(PasswordTransformationMethod.getInstance());
                }
            }
        });
        uart_box.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    uart_box.setChecked(true);
                    wifi_box.setChecked(false);
                    XcApplication.isserial = XcApplication.Mode.USB_SERIAL;
                    toastUtil.ShowToast("??????A72?????????????????????????????????????????????");
                } else {
                    uart_box.setChecked(false);
                }
            }
        });
        wifi_box.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    wifi_box.setChecked(true);
                    uart_box.setChecked(false);
                    XcApplication.isserial = XcApplication.Mode.SOCKET;
                    toastUtil.ShowToast("???????????????WiFi???????????????????????????");
                } else {
                    wifi_box.setChecked(false);
                }
            }
        });
    }

    private String getConnectWifiSsid() {
        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        Log.d("wifiInfo", wifiInfo.toString());
        Log.d("SSID", wifiInfo.getSSID());
        return wifiInfo.getSSID();
    }

    public String getConnectWifiSsidTwo() {
        WifiManager wifiManager = ((WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE));
        assert wifiManager != null;

        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        String SSID = wifiInfo.getSSID();

        int networkId = wifiInfo.getNetworkId();
        List<WifiConfiguration> configuredNetworks = wifiManager.getConfiguredNetworks();
        for (WifiConfiguration wifiConfiguration : configuredNetworks) {
            if (wifiConfiguration.networkId == networkId) {
                SSID = wifiConfiguration.SSID;
            }
        }

        return SSID.replace("\"", "");
    }


    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.reset) {
            device_edit.setText("");
            login_edit.setText("");
            passwd_edit.setText("");
            rememberbox.setChecked(false);
        } else if (view.equals(bt_connect)) {

            dialog = new ProgressDialog(this);
            dialog.setMessage("?????????????????????...");
            dialog.show();
            if (XcApplication.isserial == XcApplication.Mode.SOCKET) {
                //1
                useNetwork();
            } else if (XcApplication.isserial != XcApplication.Mode.SOCKET) {

                useUart();
            }
        }
    }

    /**
     * ??????Eventbus??????
     *
     * @param refresh
     */
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEventMainThread(DataRefreshBean refresh) {
        if (refresh.getRefreshState() == 2) {
            startFirstActivity();
        }
    }

    // ????????????cameraIP
    private void search() {
        Intent intent = new Intent(LoginActivity.this, CameraSearchService.class);
        startService(intent);
    }


    private void useUart() {
        // ????????????????????????????????????
        search();
    }

    private void useNetwork() {
        //2.
        if (new WiFiStateUtil(this).wifiInit()) {
            //WiFi???????????????
            search();
        } else {
            dialog.cancel();
            toastUtil.ShowToast("????????????????????????WiFi?????????????????????");
        }
    }

    private void startFirstActivity() {
        dialog.cancel();
        startActivity(new Intent(LoginActivity.this, FirstActivity.class));
        if (FirstActivity.IPCamera.equals("null:81")) {
            toastUtil.ShowToast("??????????????????????????????????????????");
        }
        finish();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this); // EventBus????????????
        if (dialog != null) {
            dialog.cancel();
        }
        Log.e("LoginActivity", "onDestroy");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.e("LoginActivity", "onRestart");
    }

}

