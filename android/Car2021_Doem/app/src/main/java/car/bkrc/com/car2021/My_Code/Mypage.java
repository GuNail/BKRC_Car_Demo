package car.bkrc.com.car2021.My_Code;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.util.Log;

import com.google.zxing.qrcode.encoder.QRCode;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;

public class Mypage {
    private Context context=null;
    Trffic_Light trffic_light_A=new Trffic_Light(0x0E);
    Trffic_Light trffic_light_B=new Trffic_Light(0x0F);

    public Mypage(Context mcontext){
       context = mcontext;
    }

    public void Trffic_Light_Conrtol(){

        final AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("交通灯识别");
        String[] set_item = {"交通灯A", "交通灯B"};
        builder.setSingleChoiceItems(set_item,-1,new AlertDialog.OnClickListener(){
            @Override
            public void onClick(DialogInterface dialog, int i){
                switch (i){
                    case 0:
                        new Thread(()->{trffic_light_A.Task(10);}).start();
                        break;
                    case 1:
                        new Thread(()->{trffic_light_B.Task(10);}).start();
                        break;
                }
            }
        });
        builder.create().show();
    }

    public void TX_Conrtol(){
        final AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("TFT图形颜色识别");
        String[] set_item = {"开始识别", "输出图形结果","输出颜色结果"};
        builder.setSingleChoiceItems(set_item,-1,new AlertDialog.OnClickListener(){
            @Override
            public void onClick(DialogInterface dialog, int i){
                switch (i){
                    case 0:
                        new Thread(()->{Tu_Xin.Task(10);}).start();
                        break;
                    case 1:
                        Tu_Xin.Task(20);
                        break;
                    case 2:
                        Tu_Xin.Task(21);
                        break;
                }
            }
        });
        builder.create().show();
    }

    public void CP_Conrtol(){
        final AlertDialog.Builder builder =new AlertDialog.Builder(context);
        builder.setTitle("TFT车牌识别");
        String[] set_item={"车牌识别","输出车牌"};
        builder.setSingleChoiceItems(set_item,-1,new AlertDialog.OnClickListener(){
            @Override
            public void onClick(DialogInterface dialog, int i){
                switch (i){
                    case 0:
                        new Thread(()->{CP_Tools.Task(10);}).start();
                        break;
                    case 1:
                        CP_Tools.Task(20);
                        break;
                }
            }
        });
        builder.create().show();
    }

    public void QRcode_Conrtol(){
        final AlertDialog.Builder builder =new AlertDialog.Builder(context);
        builder.setTitle("二维码识别");
        String[] set_item={"二维码识别","输出结果"};
        builder.setSingleChoiceItems(set_item,-1,new AlertDialog.OnClickListener(){
            @Override
            public void onClick(DialogInterface dialog, int i){
                switch (i){
                    case 0:
                        new Thread(()->{Qr_Code.Task(10);}).start();
                        break;
                    case 1:
                        Qr_Code.Task(20);
                        break;
                }
            }
        });
        builder.create().show();
    }

    public void Trffic_Flag_Conrtol(){
        final AlertDialog.Builder builder =new AlertDialog.Builder(context);
        builder.setTitle("交通标志识别");
        String[] set_item={"交通标志识别","输出结果"};
        builder.setSingleChoiceItems(set_item,-1,new AlertDialog.OnClickListener(){
            @Override
            public void onClick(DialogInterface dialog, int i){
                switch (i){
                    case 0:
                        new Thread(()->{ Trffic_Flag.Task(10);}).start();
                        break;
                    case 1:
                        Trffic_Flag.Task(20);
                        break;
                }
            }
        });
        builder.create().show();
    }

    public void OpenCv_Conrtol(){
        final AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("OpenCv_图像识别");
        String[] Tools={"拍照","交通灯","二维码","TFT图形识别","TFT车牌识别","TFT交通标志识别","测试用接口"};
        builder.setSingleChoiceItems(Tools,-1,new AlertDialog.OnClickListener(){
            @Override
            public void onClick(DialogInterface dialog, int i){
                switch (i){
                    case 0:
                        My_OpenCV.shoop();
                        break;
                    case 1://交通灯
                        Trffic_Light_Conrtol();
                        break;
                    case 2://二维码
                        QRcode_Conrtol();
                        break;
                    case 3://TFT图形识别
                        TX_Conrtol();
                        break;
                    case 4://TFT车牌识别
                        CP_Conrtol();
                        break;
                    case 5://TFT交通标志识别
                        Trffic_Flag_Conrtol();
                        break;
                    case 6:

                        break;
                }
            }
        });
        builder.create().show();
    }
}
