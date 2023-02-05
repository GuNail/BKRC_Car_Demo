package car.bkrc.com.car2021.My_Code;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.CpuUsageInfo;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.google.zxing.NotFoundException;
import com.googlecode.tesseract.android.TessBaseAPI;

import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.imgproc.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import car.bkrc.com.car2021.ActivityView.FirstActivity;
import car.bkrc.com.car2021.ActivityView.LoginActivity;
import car.bkrc.com.car2021.FragmentView.LeftFragment;
import car.bkrc.com.car2021.Utils.OtherUtil.ToastUtil;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;
import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

public class CP_Tools {

    static final String TAG="CP_Tools";
    static int Error_Cont=0;
    volatile public static  boolean seate=false;
    public static String CP_data="";

    /**
    *language 语言 eng 英文 ， chi_sim  中文
     */
   static final String language ="chi_sim";


   public static void Task(int msg){
        switch (msg) {
            case 10:
                if(!seate) {
                    seate = true;
                    if (My_OpenCV.Get_Scr()) {
                        showText("TFT车牌识别！");
                        CP_Recon();
                    } else {
                        showText("没有连接到摄像头，请连接到摄像头后再试！");
                    }
                    seate = false;
                }else{
                    showText("正在识别！");
                }
                break;
            case 20:
                if (!CP_data.isEmpty())
                    showText("车牌:" + CP_data);
                break;
            case 30:
                showText("未检测到正确车牌！请调整摄像头角度");
                Error_Cont++;
                break;

            default:
                break;
        }
    }

    /**
     */
    synchronized public static void CP_Recon() {
        Mat scr = new Mat();
        My_OpenCV.Get_Scr(scr);
        Get_Cp(scr, Color);
        if (!CP_data.isEmpty()) {
            Task(20);
        } else {
            Task(30);
        }
   }


    /**
    * 图像处理获取车牌
    * @param scr 输入图片
    * @param color 车牌底色
    */
    public static final int Color[]={0,130,100,180,255,255};
    public static void Get_Cp(Mat scr,int[] color){
        Mat ROI = My_OpenCV.Cut_HSV_RIO(scr,true);
        Mat CP_ROI=My_OpenCV.Cut_HSV_RIO(ROI,color,true);
        if(CP_ROI.empty()){
            return;
        }
        Mat Gray=new Mat();
        Mat imgDila=new Mat();
        Imgproc.cvtColor(CP_ROI,Gray,Imgproc.COLOR_BGR2GRAY,0);
        Imgproc.threshold(Gray,imgDila,120,255,1);
        CP_data=CP_Data_Cheack(doOcr(imgDila));
    }

    /**
    *车牌数据处理
     */
    public static String CP_Data_Cheack(String str){
        String temp=new String();
        Pattern r = Pattern.compile("[A-Z0-9]");
        Matcher m = r.matcher(str);
        while (m.find()){
            temp+=m.group();
        }
        r = Pattern.compile("[A-Z]{1}[0-9]{3}[A-Z]{1}[0-9]{1}$");
        m = r.matcher(temp);
        if(m.matches()){
            return temp;
        }else {
            return "";
        }
    }

    /**
     * 进行图片识别
     * @param bitmap   待识别图片
     * @return 识别结果字符串
     */
    private static String doOcr(Bitmap bitmap) {
        TessBaseAPI baseApi = new TessBaseAPI();
        // 必须加此行，tess-two要求BMP必须为此配置
        baseApi.init(LoginActivity.getSDPath(), language);
        System.gc();
        bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
        baseApi.setImage(bitmap);
        String text = baseApi.getUTF8Text();
        baseApi.clear();
        baseApi.end();
        return text;
    }

    private static String doOcr(Mat mat) {
        Bitmap bitmap=Bitmap.createBitmap(mat.width(),mat.height(),Bitmap.Config.ARGB_8888);
        Utils.matToBitmap(mat,bitmap);
        TessBaseAPI baseApi = new TessBaseAPI();
        // 必须加此行，tess-two要求BMP必须为此配置
        baseApi.init(LoginActivity.getSDPath()+"/res_data", language);
        System.gc();
        bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
        baseApi.setImage(bitmap);
        String text = baseApi.getUTF8Text();
        baseApi.clear();
        baseApi.end();
        return text;
    }

}
