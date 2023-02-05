package car.bkrc.com.car2021.My_Code;
import car.bkrc.com.car2021.ActivityView.FirstActivity;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.imgproc.Imgproc;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;
import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

import static car.bkrc.com.car2021.My_Code.My_OpenCV.*;

public class Trffic_Light{

    public int Trffic_Color=0;
    private int AorB;
    public boolean seate=false;

    private String TAG="Trffic_Light";

    public Trffic_Light(int Type){
        AorB = Type;
    }

    /**
     交通灯颜色识别
     */
   synchronized public void Task(int msg){
        switch (msg) {
            case 10:
                if (!seate) {
                    seate = true;
                    if (Get_Scr()) {
                        showText("进入交通灯识别模式！");
                        Trffic_Light_Recon();
                    } else {
                        showText("没有连接到摄像头，请连接到摄像头后再试！");
                    }
                    seate = false;
                } else {
                    showText("正在识别中！");
                }
            break;

            case 20:
                Send_reslut();
                break;

            case 30:
                showText("未识别成功！");
                Random r=new Random();
                Trffic_Color=r.nextInt(3)+1;
                Task(20);
                break;
        }
    }

    /**
     交通灯识别接口
     */
    public void Trffic_Light_Recon(){
        Start_Recon();
        Trffic_Color = Traffic_Light();
        if (Trffic_Color != 0) {
            Task(20);
        } else {
            Task(30);
        }
    }


    public int Traffic_Light() {
        int[] RED={156,100,230,180,255,255};
        int[] GREEN={44,89,219,99,255,255};
        int[] YELLOW={18,0,196,34,255,255};
        ArrayList<int[]> Colors=new ArrayList<>();
        Colors.add(RED);
        Colors.add(GREEN);
        Colors.add(YELLOW);
        Mat scr=new Mat();
        Get_Scr(scr);
        if(scr.empty()) {
            return -1;
        }
        for(int cont=0;cont<3;cont++) {
            Mat imgEqua;
            imgEqua=stackImgage(scr,Colors.get(cont));
            Mat hierarchy=new Mat();
            List<MatOfPoint> contours=new ArrayList<>();
            Imgproc.findContours(imgEqua,contours,hierarchy,Imgproc.RETR_EXTERNAL,Imgproc.CHAIN_APPROX_NONE);
            for(int i=0;i<contours.size();i++) {
                double Area=Imgproc.contourArea(contours.get(i));
                if(Area>200) {
                    if(cont==0) {
                        toastUtil.ShowToast("红色");
                    }
                    else if(cont==1) {
                        toastUtil.ShowToast("绿色");
                    }
                    else if(cont==2) {
                        toastUtil.ShowToast( "黄色");
                    }
                    return cont+1;
                }
            }
        }
        return 0;
    }


    private void Start_Recon(){
        FirstActivity.Connect_Transport.traffic_control(AorB,0x01,0x00);
        FirstActivity.Connect_Transport.yanchi(300);
        FirstActivity.Connect_Transport.traffic_control(AorB,0x01,0x00);
        FirstActivity.Connect_Transport.yanchi(300);
        FirstActivity.Connect_Transport.traffic_control(AorB,0x01,0x00);
        FirstActivity.Connect_Transport.yanchi(1000);
    }

    private void Send_reslut(){
        FirstActivity.Connect_Transport.traffic_control(AorB, 0x02, Trffic_Color);
        FirstActivity.Connect_Transport.yanchi(300);
        FirstActivity.Connect_Transport.traffic_control(AorB, 0x02, Trffic_Color);
        FirstActivity.Connect_Transport.yanchi(300);
        FirstActivity.Connect_Transport.traffic_control(AorB, 0x02, Trffic_Color);
        FirstActivity.Connect_Transport.yanchi(300);
    }
}



