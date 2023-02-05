package car.bkrc.com.car2021.FragmentView;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import car.bkrc.com.car2021.My_Code.My_Task;
import car.bkrc.com.car2021.Utils.CameraUtile.XcApplication;

import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import car.bkrc.com.car2021.MessageBean.DataRefreshBean;
import car.bkrc.com.car2021.Utils.OtherUtil.WiFiStateUtil;
import car.bkrc.com.car2021.R;
import car.bkrc.com.car2021.ActivityView.FirstActivity;
import car.bkrc.com.car2021.ActivityView.LoginActivity;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.Connect_Transport;
import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;
import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

public class RightFragment1 extends Fragment {

    String Camera_show_ip = null;

    private TextView Data_show = null;
    private EditText speededit = null;
    private EditText coded_discedit = null;
    private EditText angle_dataedit = null;
    private TextView stateTV, psStatusTV, codedDiskTV, lightTV, ultraSonicTV;

    public static final String TAG = "RightFragment1";
    private View view = null;


    public static RightFragment1 getInstance() {
        return RightFragment1Holder.sInstance;
    }

    private static class RightFragment1Holder {
        private static final RightFragment1 sInstance = new RightFragment1();
    }

    private My_Task Task=new My_Task();
    // 接受显示设备发送的数据
    @SuppressLint("HandlerLeak")
    private Handler rehHandler = new Handler() {
        @SuppressLint("SetTextI18n")
        public void handleMessage(Message msg) {
            if (msg.what == 1) {
                byte[] mByte = (byte[]) msg.obj;
                if (mByte[0] == 0x55) {
                    // 光敏状态
                    long psStatus = mByte[3] & 0xff;
                    // 超声波数据
                    long ultraSonic = mByte[5] & 0xff;
                    ultraSonic = ultraSonic << 8;
                    ultraSonic += mByte[4] & 0xff;
                    // 光照强度
                    long light = mByte[7] & 0xff;
                    light = light << 8;
                    light += mByte[6] & 0xff;
                    // 码盘
                    long codedDisk = mByte[9] & 0xff;
                    codedDisk = codedDisk << 8;
                    codedDisk += mByte[8] & 0xff;

                    Camera_show_ip = FirstActivity.IPCamera;
                    if (mByte[1] == (byte) 0xaa) {  //主车
                        if (FirstActivity.chief_status_flag == true) {
                            // 显示数据
                            stateTV.setText(mByte[2]+"");              // 运行状态
                            psStatusTV.setText(psStatus+"");        // 光敏电阻
                            codedDiskTV.setText(codedDisk+"");      // 码盘
                            lightTV.setText(light+" lx");           // 光照度
                            ultraSonicTV.setText(ultraSonic+" mm"); // 超声波
                            Data_show.setText("主  车");
                        }
                    }
                    else if (mByte[1] == (byte) 0x02) //从车
                    {
                        if (FirstActivity.chief_status_flag == false) {
                            if (mByte[2] == -110) {
                                byte[] newData = new byte[50];
                                Log.e("data", "" + mByte[4]);
                                newData = Arrays.copyOfRange(mByte, 5, mByte[4] + 5);
                                Log.e("data", "" + "长度" + newData.length);
                                try {
                                    String str = new String(newData, "ascii");//第二个参数指定编码方式
                                    Toast.makeText(getActivity(), "" + str, Toast.LENGTH_LONG).show();
                                } catch (UnsupportedEncodingException e) {
                                    e.printStackTrace();
                                }

                            } else {
                                // 显示数据
                                stateTV.setText(mByte[2]+"");              // 运行状态
                                psStatusTV.setText(psStatus+"");        // 光敏电阻
                                codedDiskTV.setText(codedDisk+"");      // 码盘
                                lightTV.setText(light+" lx");           // 光照度
                                ultraSonicTV.setText(ultraSonic+" mm"); // 超声波
                                Data_show.setText("从  车");
                            }
                        }
                    }
                    else if(mByte[1] == (byte) 0x72){
                        new Thread(()->{
                            if(!Task.seate) {
                                Task.run(mByte);
                            }else{
                                showText("正在运行");
                            }
                        }).start();
                    }
                }
            }
        }
    };

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Log.d(TAG, "onCreateView");
        if (view != null) {
            ViewGroup parent = (ViewGroup) view.getParent();
            if (parent != null) {
                parent.removeView(view);
            }
        } else {
            if (LoginActivity.isPad(getActivity()))
                view = inflater.inflate(R.layout.right_fragment1, container, false);
            else
                view = inflater.inflate(R.layout.right_fragment1_mobilephone, container, false);
        }
        FirstActivity.recvhandler = rehHandler;
        EventBus.getDefault().register(this); // EventBus消息注册
        control_init();
        connect_Open();
        return view;
    }

    /**
     * 页面初始化
     */
    private void control_init() {
        Data_show = view.findViewById(R.id.rvdata);
        speededit = view.findViewById(R.id.speed_data);
        coded_discedit = view.findViewById(R.id.coded_disc_data);
        angle_dataedit = view.findViewById(R.id.angle_data);
        stateTV = view.findViewById(R.id.stateTV);
        psStatusTV = view.findViewById(R.id.psStatusTV);
        codedDiskTV = view.findViewById(R.id.codedDiskTV);
        lightTV = view.findViewById(R.id.lightTV);
        ultraSonicTV = view.findViewById(R.id.ultraSonicTV);

        ImageButton up_bt = view.findViewById(R.id.up_button);
        ImageButton blew_bt = view.findViewById(R.id.below_button);
        ImageButton stop_bt = view.findViewById(R.id.stop_button);
        ImageButton left_bt = view.findViewById(R.id.left_button);
        ImageButton right_bt = view.findViewById(R.id.right_button);

        up_bt.setOnClickListener(new onClickListener2());
        blew_bt.setOnClickListener(new onClickListener2());
        stop_bt.setOnClickListener(new onClickListener2());
        left_bt.setOnClickListener(new onClickListener2());
        right_bt.setOnClickListener(new onClickListener2());
        up_bt.setOnLongClickListener(new onLongClickListener2());
    }

    /**
     * 接收Eventbus消息
     *
     * @param refresh
     */
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEventMainThread(DataRefreshBean refresh) {
        if (refresh.getRefreshState() == 1 && new WiFiStateUtil(getActivity()).wifiInit()) {
            connect_Open();
        } else if (refresh.getRefreshState() == 3) {
            toastUtil.ShowToast("平台已连接");
        } else if (refresh.getRefreshState() == 4) {
            toastUtil.ShowToast("平台连接失败！");
        } else toastUtil.ShowToast("请检查WiFi连接状态！");

    }

    private void connect_Open() {
        if (XcApplication.isserial == XcApplication.Mode.SOCKET) {
            connect_thread();                            //开启网络连接线程
        } else if (XcApplication.isserial == XcApplication.Mode.SERIAL) {
            serial_thread();   //使用纯串口uart4
        }
    }

    private void connect_thread() {
        XcApplication.executorServicetor.execute(new Runnable() {
            @Override
            public void run() {
                FirstActivity.Connect_Transport.connect(rehHandler, FirstActivity.IPCar);
            }
        });
    }

    private void serial_thread() {
        XcApplication.executorServicetor.execute(new Runnable() {
            @Override
            public void run() {
                FirstActivity.Connect_Transport.serial_connect(rehHandler);
            }
        });
    }

    // 速度和码盘方法
    private int getSpeed() {
        String src = speededit.getText().toString();
        int speed = 90;
        if (!src.equals("")) {
            speed = Integer.parseInt(src);
        } else {
            toastUtil.ShowToast("请输入设备运行速度！");
        }
        return speed;
    }

    private int getEncoder() {
        String src = coded_discedit.getText().toString();
        int encoder = 20;
        if (!src.equals("")) {
            encoder = Integer.parseInt(src);
        } else {
            toastUtil.ShowToast("请输入码盘值！");
        }
        return encoder;
    }

    private int getAngle() {
        String src = angle_dataedit.getText().toString();
        int angle = 50;
        if (!src.equals("")) {
            angle = Integer.parseInt(src);
        } else {
            toastUtil.ShowToast("请输入循迹速度值！");
        }
        return angle;
    }

    // 速度与码盘值
    private int sp_n;

    private class onClickListener2 implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            sp_n = getSpeed();

            switch (v.getId()) {
                case R.id.up_button:
                    int en_n = getEncoder();
                    FirstActivity.Connect_Transport.go(sp_n, en_n);
                    break;
                case R.id.left_button:
                    FirstActivity.Connect_Transport.left(sp_n);
                    break;
                case R.id.right_button:
                    FirstActivity.Connect_Transport.right(sp_n);
                    break;
                case R.id.below_button:
                    en_n = getEncoder();
                    FirstActivity.Connect_Transport.back(sp_n, en_n);
                    break;
                case R.id.stop_button:
                    FirstActivity.Connect_Transport.stop();
                    break;
            }
        }
    }

    private class onLongClickListener2 implements View.OnLongClickListener {
        @Override
        public boolean onLongClick(View view) {
            if (view.getId() == R.id.up_button) {
                sp_n = getAngle();
                FirstActivity.Connect_Transport.line(sp_n);
            }
    /*如果将onLongClick返回false，那么执行完长按事件后，还有执行单击事件。
    如果返回true，只执行长按事件*/
            return true;
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }
}


