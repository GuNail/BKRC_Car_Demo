package car.bkrc.com.car2021.My_Code;

import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import java.util.Arrays;

import car.bkrc.com.car2021.ActivityView.FirstActivity;
import car.bkrc.com.car2021.DataProcessingModule.ConnectTransport;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.Connect_Transport;
import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;
import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

public class My_Task{
    Trffic_Light trffic_light_A=new Trffic_Light(0x0E);
    Trffic_Light trffic_light_B=new Trffic_Light(0x0F);

    public boolean seate=false;

    synchronized public  void run(byte[] data){
        seate=true;
        switch (data[2]) {
            case 0x01:
                IPCAM.UP_30_IPCAM();//旋转至预设位
                if (data[3] == 0x0A) {
                    trffic_light_A.Task(10);
                } else if (data[3] == 0x0B) {
                    trffic_light_B.Task(10);
                }
                Finsh();
                break;

            case 0x02:
                Qr_Code.Task(10);
                Finsh();
                break;

            case 0x03:
                Tu_Xin.Task(10);
                Finsh();
                break;

            case 0x04:
                CP_Tools.Task(10);
                Finsh();
                break;

            case 0x05:
                Trffic_Flag.Task(10);
                Finsh();
                break;

            default:
                showText("无效的指令！");
                break;
        }
        IPCAM.Rest_IPCAM();
        seate=false;
        Connect_Transport.yanchi(1000);
     }

    void Finsh() {
        Connect_Transport.A72_Task_Finsh();
        Connect_Transport.yanchi(200);
        Connect_Transport.A72_Task_Finsh();
        Connect_Transport.yanchi(200);
        Connect_Transport.A72_Task_Finsh();
        Connect_Transport.yanchi(200);
    }

}
