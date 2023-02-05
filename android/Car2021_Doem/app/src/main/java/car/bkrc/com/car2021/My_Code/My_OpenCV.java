package car.bkrc.com.car2021.My_Code;

import android.graphics.Bitmap;
import android.os.Environment;

import org.opencv.android.Utils;
import org.opencv.core.Core;
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

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import car.bkrc.com.car2021.FragmentView.LeftFragment;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.chief_control_flag;
import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;

public class My_OpenCV {

    /**
    bitmap转Mat
     */
    public static int Get_Scr(Mat img) {
        if (LeftFragment.bitmap == null) {
            toastUtil.ShowToast("没有图片！");
            return -1;
        }
        Utils.bitmapToMat(LeftFragment.bitmap, img);
        Imgproc.cvtColor(img, img, Imgproc.COLOR_BGR2RGB);
        return 1;
    }

    /**
     * 判断有没有图片
     * @return
     */
    public static boolean Get_Scr() {
        if (LeftFragment.bitmap == null) {
            toastUtil.ShowToast("没有图片！");
            return false;
        }
        return true;
    }

    /**
    拍照并把图片保存到手机
     */
    public static void shoop() {
        Mat img = new Mat();
        if (Get_Scr(img) == 1) {
            File file = new File(Environment.getExternalStorageDirectory().getPath() + File.separator + System.currentTimeMillis() + ".jpg");
            Imgcodecs.imwrite(file.getPath(), img);
            toastUtil.ShowToast("图片：" + file.getPath());
        }
    }

    /**
    保存图片到手机 参数img
     */
    public static void save_img(Mat img){
        if (!img.empty()){
            File file = new File(Environment.getExternalStorageDirectory().getPath() + File.separator + System.currentTimeMillis() + ".jpg");
            Imgcodecs.imwrite(file.getPath(), img);
            toastUtil.ShowToast("图片：" + file.getPath());
        }
        else{
            toastUtil.ShowToast("图片保存失败！");
        }
    }

    /**
     * 保存图片到手机 参数Bitmap  img
     * @param img
     */
    public static void save_img(Bitmap img){
        Mat scr=new Mat();
        if(img==null)
            return;
        Utils.bitmapToMat(img,scr);
        Imgproc.cvtColor(scr, scr, Imgproc.COLOR_BGR2RGB);
        if (!scr.empty()){
            File file = new File(Environment.getExternalStorageDirectory().getPath() + File.separator + System.currentTimeMillis() + ".jpg");
            Imgcodecs.imwrite(file.getPath(), scr);
            toastUtil.ShowToast("图片：" + file.getPath());
        }
        else{
            toastUtil.ShowToast("图片保存失败！");
        }
    }
    /**
    HSV图像处理方法
    scr 输入图像 color 输入hsv值
    返回 处理后的黑白图片
     */
    public static Mat stackImgage(Mat scr, int color[]) {
        Mat imgBGR = new Mat();
        Imgproc.GaussianBlur(scr, imgBGR, new Size(7, 7), 0);
        Mat imgHSV = new Mat();
        Imgproc.cvtColor(imgBGR, imgHSV, Imgproc.COLOR_BGR2HSV);
        Mat imgEqua = new Mat();
        Scalar color_l = new Scalar(color[0], color[1], color[2]);
        Scalar color_h = new Scalar(color[3], color[4], color[5]);
        Core.inRange(imgHSV, color_l, color_h, imgEqua);
        Mat imgKer;
        imgKer = Imgproc.getStructuringElement(Imgproc.MORPH_ELLIPSE, new Size(7, 7));
        Imgproc.morphologyEx(imgEqua, imgEqua, Imgproc.MORPH_CLOSE, imgKer);
        Imgproc.morphologyEx(imgEqua, imgEqua, Imgproc.MORPH_OPEN, imgKer);
        return imgEqua;
    }

    /**
     *
     *最大ROI部分裁剪 backGround_Color 默认兼容 白 黄 可根据需求修改
     */
    private static final int[] backGround_Color={0,60,150,180,255,255};

    public static Mat Cut_HSV_RIO(Mat scr,boolean angle) {
        Mat temp = stackImgage(scr, backGround_Color);

        List<MatOfPoint> contours = new ArrayList<>();
        MatOfPoint Max_Contours = new MatOfPoint();
        double Max_area = 0;

        //寻找轮廓，角度修正
        Imgproc.findContours(temp, contours, new Mat(), Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_NONE);
        for (int i = 0; i < contours.size(); i++) {
            double Area = Imgproc.contourArea(contours.get(i));
            if (Area > Max_area) {
                Max_area = Area;
                Max_Contours = contours.get(i);
            }
        }
        MatOfPoint2f approx = new MatOfPoint2f();
        double perimeter = Imgproc.arcLength(new MatOfPoint2f(Max_Contours.toArray()), true);
        Imgproc.approxPolyDP(new MatOfPoint2f(Max_Contours.toArray()), approx, perimeter * 0.02, true);
        if(!angle){
            Rect rect=Imgproc.boundingRect(approx);
            return new Mat(scr,rect);
        }else {
            RotatedRect rotatedRect = Imgproc.minAreaRect(approx);
            return Rotate(scr, rotatedRect);
        }
    }

    /**
     * @param scr
     * @param Color
     * @return
     */
    public static Mat Cut_HSV_RIO(Mat scr,int[] Color,boolean angle) {
        Mat temp = stackImgage(scr,Color);

        List<MatOfPoint> contours = new ArrayList<>();
        MatOfPoint Max_Contours = new MatOfPoint();
        double Max_area = 0;
        MatOfPoint2f approx=new MatOfPoint2f();

        //寻找轮廓，角度修正
        Imgproc.findContours(temp, contours, new Mat(), Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_NONE);

        for(int i=0;i<contours.size();i++) {
            double Area=Imgproc.contourArea(contours.get(i));
            if(Area>Max_area) {
                Max_area=Area;
                Max_Contours=contours.get(i);
                double perimeter = Imgproc.arcLength(new MatOfPoint2f(Max_Contours.toArray()),true);
                Imgproc.approxPolyDP(new MatOfPoint2f(Max_Contours.toArray()),approx,perimeter*0.02,true);
            }
        }
        if(!approx.empty())
        {
            if(!angle){
                Rect rect=Imgproc.boundingRect(approx);
                return new Mat(scr,rect);
            }else {
                RotatedRect rotatedRect = Imgproc.minAreaRect(approx);
                return Rotate(scr, rotatedRect);
            }
        }else{
            return new Mat();
        }

    }

    /**
     *最外接小矩形 角度修正 input  rotatedRect 最外接小矩形
     */
    public static Mat Rotate(Mat input,RotatedRect rotatedRect){
        double angle;
        Size size = rotatedRect.size;
        if(rotatedRect.size.width<=rotatedRect.size.height){
            angle=rotatedRect.angle-90;
            double temp= size.width;
            size.width=size.height;
            size.height=temp;
        }else {
            angle=rotatedRect.angle;
        }
        Mat output=new Mat();
        Mat M=Imgproc.getRotationMatrix2D(rotatedRect.center,angle,1);
        Imgproc.warpAffine(input,output,M,input.size());
        Imgproc.getRectSubPix(output,size,rotatedRect.center,output);
        return output;
    }


    /**
    获取两条相交线的夹角，c 为线的相交点
     */
    public static double getAngelOfTwoVector(MatOfPoint2f pt) {
        Point c=new Point(pt.get(0,0)[0],pt.get(0,0)[1]);
        Point pt1=new Point(pt.get(1,0)[0],pt.get(1,0)[1]);
        Point pt2=new Point(pt.get(3,0)[0],pt.get(3,0)[1]);
        return getAngel(pt1,pt2,c);

    }


    public static double getAngel(Point pt1,Point pt2,Point c){
        double theta = Math.atan2(pt1.y - c.y, pt1.x - c.x) - Math.atan2(pt2.y - c.y, pt2.x - c.x);
        if (theta > Math.PI)
            theta -= 2 * Math.PI;
        if (theta < -Math.PI)
            theta += 2 * Math.PI;
        theta = theta * 180.0 / Math.PI;
        return theta;
    }



}
