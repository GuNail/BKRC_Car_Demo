package car.bkrc.com.car2021.My_Code;


import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONObject;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.RotatedRect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

import car.bkrc.com.car2021.ActivityView.FirstActivity;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;
import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

public class Tu_Xin {

    volatile public static boolean seate=false;
    private static Mat Input_img=new Mat();
    private static Mat Input_img1=new Mat();

    private static int result_Sum=0;

    public static int rec     =0;//矩形
    public static int tri     =1;//三角形
    public static int pri     =2;//棱形
    public static int star    =3;//五角星
    public static int round   =4;//圆形

    public static int red     =0;//红色
    public static int green   =1;//绿色
    public static int blue    =2;//蓝色
    public static int yellow  =3;//黄色
    public static int purple  =4;//品色
    public static int qin     =5;//青色
    public static int black   =6;//黑色
    public static int white   =7;//白色

    public static String[] shape_Text={"矩形","三角形","棱形","五角星","圆形"};
    public static String[] color_Text={"红色","绿色","蓝色","黄色","品色","青色","黑色","白色"};
    public static String[] color_Text_EN={"Red","Green","Blue","Yellow","Pin","Qin","Black","Wlite"};

    public static int[] color_cont=new int[8];
    public static int[] shape_cont=new int[5];

    private static final int[] RED = { 30,0,0,140,255,255 };
    private static final int[] GREEN = { 50,100,46,77,255,255 };
    private static final int[] BLUE = { 100,120,72,120,255,255 };
    private static final int[] YELLOW = { 20,65,140,45,255,255 };
    private static final int[] PIN = { 125,120,46,155,255,255 };
    private static final int[] QING = { 78,80,46,99,255,255 };
    private static final int[] BLACK = { 0,0,0,180,255,90};
    private static final int[] WHITE = { 0,0,0,180,110,255 };

    private static final List<int[]> color=new ArrayList<>();
    static {
        color.add(RED);
        color.add(GREEN);
        color.add(BLUE);
        color.add(YELLOW);
        color.add(PIN);
        color.add(QING);
        color.add(BLACK);
        color.add(WHITE);
    }

    /**
     *图形识别任务
     */
    public static void Task(int msg) {
        switch (msg) {
            case 10:
                if (!seate) {
                    seate = true;
                    Empty();
                    if (My_OpenCV.Get_Scr()) {
                        showText("TFT图形颜色识别！");
                        Tu_Xin_Recon();
                    } else {
                        showText("没有连接到摄像头，请连接到摄像头后再试！");
                    }
                    seate = false;
                } else {
                    showText("正在识别！");
                }
                break;

            case 20:
                put_Shape_num();
                break;

            case 21:
                put_Color_num();
                break;

            case 30:
                showText("识别失败");
                break;
        }
    }

    synchronized private static void Tu_Xin_Recon(){
        Mat scr = new Mat();
        My_OpenCV.Get_Scr(scr);
        Input_img = My_OpenCV.Cut_HSV_RIO(scr, true);
        Input_img1 = Input_img.clone();
        Mat temp = Input_img.clone();
        Get_Shapes(temp);
        Get_Color(temp, yellow);//yellow 为底色，计算自动排除
        if (Result()) {
            FirstActivity.Connect_Transport.yanchi(2000);
            Task(20);
            FirstActivity.Connect_Transport.yanchi(2000);
            Task(21);
        } else {
            Task(30);
        }
    }

    /**
     * 对数据图形数量进行判断，一般TFT 至少有5个图像
     * @return
     */
    private static boolean Result(){
        if(result_Sum<5){
            Empty();
            return false;
        }else{
            return true;
        }
    }

    /**
     * 输出形状数量
     */
    public static void put_Shape_num(){
        String str=new String();
        for(int i=0;i<5;i++){
            if(shape_cont[i]!=0){
                str+=shape_Text[i]+':'+shape_cont[i]+"个\n"; }
        }
        if(!str.isEmpty()) {
            showText(str); }
    }

    /**
     * 输出颜色数量
     */
    public static void put_Color_num(){
        String str=new String();
        for(int i=0;i<8;i++){
            if(color_cont[i]!=0){
                str+=color_Text[i]+':'+color_cont[i]+"个\n"; }
        }
        if(!str.isEmpty()) {
            showText(str); }
    }

    /**
     * 清空数据
     */
    private static void Empty(){
        for(int i=0;i<color_cont.length;i++){
            color_cont[i]=0; }
        for (int i=0;i<shape_cont.length;i++){
            shape_cont[i]=0; }
        result_Sum=0;
    }

    /**
    获取图片中对应颜色数量，background 背景颜色对应编号,统计颜色时自动去除
     */
    public static void Get_Color(Mat scr,int background){
        for(int i=0;i<7;i++){
            if(i!=background) {
                if(scr.empty()){
                    return;
                }
                Mat img=My_OpenCV.stackImgage(scr,color.get(i));
                int cont=Get_ShapeColor(img,color_Text_EN[i]);
                color_cont[i]=cont;
                result_Sum=result_Sum+cont;
            }
        }
    }

    /**
     * 获取形状个数
     * @param img 输入图像
     */
    public static void Get_Shapes(Mat img){
        if(img.empty()){
            return;
        }
        //图片预处理
        Mat Gray=new Mat();
        Mat Blur=new Mat();
        Mat imgCanny=new Mat();
        Mat imgDila=new Mat();

        Imgproc.cvtColor(img,Gray,Imgproc.COLOR_BGR2GRAY,0);
        Imgproc.GaussianBlur(Gray,Blur,new Size(3,3),10);
        Imgproc.Canny(Blur,imgCanny,20,20);
        Mat kernel=Imgproc.getStructuringElement(Imgproc.MORPH_RECT, new Size(2,2));
        Imgproc.dilate(imgCanny,imgDila,kernel);

        //存储容器
        List<MatOfPoint> contours=new ArrayList<>();
        MatOfPoint2f conPoly=new MatOfPoint2f();

        //寻找轮廓
        Imgproc.findContours(imgDila,contours,new Mat(),Imgproc.RETR_EXTERNAL,Imgproc.CHAIN_APPROX_TC89_KCOS);

        if (contours.size()==0){
            return;
        }
        String obj=new String();
        for(int i=0;i<contours.size();i++){
            double Area=Imgproc.contourArea(contours.get(i));
            if (Area>1000){
                //MatOfPoint to MatOfPoint2f 转
                MatOfPoint2f matOfPoint2f=new MatOfPoint2f();
                contours.get(i).convertTo(matOfPoint2f, CvType.CV_32F);

                //计算周长，多边形逼近
                double peri=Imgproc.arcLength(matOfPoint2f,true);
                Imgproc.approxPolyDP(matOfPoint2f,conPoly,0.03*peri,true);

                int objCor=conPoly.toArray().length;
                if(objCor<=2){ }
                else if (objCor==3){
                    shape_cont[tri]++;
                    obj="Tri";
                }else if (objCor==4){
                    double Angle=My_OpenCV.getAngelOfTwoVector(conPoly);
                    if(80<Angle&&Angle<100){
                       shape_cont[rec]++;
                       obj="rec";
                    }else {
                       shape_cont[pri]++;
                       obj="pri";
                    }
                }
                else if(objCor==10){
                    shape_cont[star]++;
                    obj="star";
                }
                else {
                   shape_cont[round]++;
                   obj="round";
                }
                RotatedRect rotatedRect = Imgproc.minAreaRect(conPoly);
                Imgproc.putText(Input_img,obj,rotatedRect.center,1,1,new Scalar(255,255,255));
            }
        }

    }

    /**
     *
     * @param imgDil 输入图像
     * @param color HSV 阈值
     * @return 对应HSV颜色个数
     */
    public static int Get_ShapeColor(Mat imgDil,String color){
        Imgproc.Canny(imgDil,imgDil,40,20);
        Mat kernel=Imgproc.getStructuringElement(Imgproc.MORPH_RECT, new Size(2,2));
        Imgproc.dilate(imgDil,imgDil,kernel);
        List<MatOfPoint> contours=new ArrayList<>();
        Imgproc.findContours(imgDil,contours,new Mat(),Imgproc.RETR_EXTERNAL,Imgproc.CHAIN_APPROX_TC89_KCOS);
        if(contours.size()==0){
            return 0;
        }
        int cont=0;//颜色计数器
        for(int i=0;i<contours.size();i++) {
            double Area = Imgproc.contourArea(contours.get(i));
            if (Area > 1000) {
                RotatedRect rotatedRect = Imgproc.minAreaRect(new MatOfPoint2f(contours.get(i).toArray()));
                Imgproc.putText(Input_img1,color,rotatedRect.center,1,1,new Scalar(255,255,255));
                cont++;//颜色计数器
            }
        }
        return cont;
    }
}
