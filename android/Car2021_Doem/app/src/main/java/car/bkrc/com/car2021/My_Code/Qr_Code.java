package car.bkrc.com.car2021.My_Code;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Message;

import com.google.zxing.BinaryBitmap;
import com.google.zxing.ChecksumException;
import com.google.zxing.DecodeHintType;
import com.google.zxing.FormatException;
import com.google.zxing.NotFoundException;
import com.google.zxing.Result;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.QRCodeReader;

import java.util.HashMap;
import java.util.Map;

import car.bkrc.com.car2021.DataProcessingModule.QR_Recognition;
import car.bkrc.com.car2021.FragmentView.LeftFragment;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;
import static car.bkrc.com.car2021.My_Code.ToastUtils.showText;

public class Qr_Code {

    public static String result_qr;
    public static int Error_Cont=0;//错误次数
    private static Bitmap qrBitmap;
    volatile private static boolean seate = false;

    // 二维码任务
    public static void Task(int msg) {
        switch (msg) {
            case 10:
                if (!seate) {
                    seate = true;
                    qrBitmap = LeftFragment.bitmap;
                    if (My_OpenCV.Get_Scr()) {
                        showText("开始识别二维码！");
                        QRRecon();
                    } else {
                        showText("没有连接到摄像头，请连接到摄像头后再试！");
                    }
                    seate = false;
                } else {
                    showText("正在识别！");
                }
                break;

            case 20:
                if (result_qr != null)
                    showText(result_qr);
                break;

            case 30:
                showText("未检测到二维码！");
                Error_Cont++;
                break;

            default:
                break;
        }
    }

    /**
     * 二维码扫描
     */
    synchronized private static void QRRecon(){
        Result result;
        QR_Recognition rSource = new QR_Recognition(bitmap2Gray(qrBitmap));
        try {
            BinaryBitmap binaryBitmap = new BinaryBitmap(new HybridBinarizer(rSource));
            Map<DecodeHintType, String> hint = new HashMap<DecodeHintType, String>();
            hint.put(DecodeHintType.CHARACTER_SET, "utf-8");
            QRCodeReader reader = new QRCodeReader();
            result = reader.decode(binaryBitmap, hint);
            if (result.toString() != null) {
                result_qr = result.toString();
                Task(20);
            }
        } catch (NotFoundException e) {
            e.printStackTrace();
            Task(30);
        } catch (ChecksumException e) {
            e.printStackTrace();
        } catch (FormatException e) {
            e.printStackTrace();
        }
    }

    /**
     * 图像灰度化
     * @param bmSrc
     * @return
     */
    public static Bitmap bitmap2Gray(Bitmap bmSrc) {
        // 得到图片的长和宽
        if (bmSrc == null)
            return null;
        int width = bmSrc.getWidth();
        int height = bmSrc.getHeight();
        // 创建目标灰度图像
        Bitmap bmpGray = null;
        bmpGray = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
        // 创建画布
        Canvas c = new Canvas(bmpGray);
        Paint paint = new Paint();
        ColorMatrix cm = new ColorMatrix();
        cm.setSaturation(0);
        ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm);
        paint.setColorFilter(f);
        c.drawBitmap(bmSrc, 0, 0, paint);
        return bmpGray;
    }
}
