package car.bkrc.com.car2021.My_Code;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.ContextThemeWrapper;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import java.io.InputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.List;

import car.bkrc.com.car2021.ActivityView.FirstActivity;
import car.bkrc.com.car2021.ActivityView.LoginActivity;

import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

public class Trffic_Flag {
    private static final String[] Flag_Text = {"", "直行", "左转", "右转", "掉头", "禁止直行", "禁止通行"};
    volatile public static boolean seate = false;
    public static int Error_Cont = 0;
    public static int Flag_num = 0;
    private static Context mcontext = LoginActivity.context;


    synchronized public static void Task(int msg) {
        switch (msg) {
            case 10:
                if (!seate) {
                    seate = true;
                    if (My_OpenCV.Get_Scr()) {
                        showText("TFT交通标志物识别！");
                        Trffic_Flag_Recon();
                    } else {
                        showText("没有连接到摄像头，请连接到摄像头后再试！");
                    }
                    seate=false;
                } else {
                    showText("正在识别！");
                }
                break;

            case 20:
                showText(Flag_Text[Flag_num] + ":0x0" + Flag_num);
                break;

            case 30:
                showText("未找到交通标志！");
                Error_Cont++;
                break;
        }
    }

    /**
     * 交通标志识别
     */
    private static void Trffic_Flag_Recon() {
        int[] hsv = {0, 100, 90, 180, 255, 255};
        Mat scr = new Mat();
        My_OpenCV.Get_Scr(scr);
        scr = My_OpenCV.Cut_HSV_RIO(scr, hsv, false);
        Flag_num = Template_matching(scr);
        if (Flag_num != 0) {
           Task(20);
        } else {
            Task(30);
        }
    }

        /**
         * 模板匹配
         * @param input 输入图像
         * @return 输入图像对应的序号
         */
    public static int Template_matching(Mat input) {
        for (int i = 1; i < Flag_Text.length; i++) {
            Mat temp = Open_Assets("img/" + Flag_Text[i] + ".jpg");
            Mat reslut = new Mat();
            if ((input.cols() * input.rows()) > (temp.cols() * temp.rows())) {
                return 0;
            }
            Imgproc.matchTemplate(temp,input , reslut, Imgproc.TM_CCOEFF_NORMED);
            Core.MinMaxLocResult minMaxLocResult = Core.minMaxLoc(reslut);
            if (minMaxLocResult.maxVal > 0.7) {
                return i;
            }
        }
        return 0;
    }

        /**
         * 读取 assets目录的图片
         * @param name 文件名/位置
         * @return 图片
         */
    private static Mat Open_Assets(String name) {
        Bitmap bitmap = null;
        try {
            InputStream inputStream = mcontext.getResources().getAssets().open(name);
            bitmap = BitmapFactory.decodeStream(inputStream);
            inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        Mat output = new Mat();
        if (bitmap == null)
            return output;
        Utils.bitmapToMat(bitmap, output);
        Imgproc.cvtColor(output, output, Imgproc.COLOR_BGR2RGB);
        return output;
    }
}
