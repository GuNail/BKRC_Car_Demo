package car.bkrc.com.car2021.My_Code;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.Gravity;
import android.widget.Toast;

import car.bkrc.com.car2021.ActivityView.LoginActivity;


public class ToastUtils {
    private static Toast toast = null;
    private static Handler mainHandler = null;
    private static Context context= LoginActivity.context;
    /**
     * 显示Toast信息（子线程也可以调用）
     */
    public static void showText(int resId) {
        show(null, resId);
    }

    /**
     * 显示Toast信息（子线程也可以调用）
     */
    public static void showText(String msg) {
        show( msg, 0);
    }

    /**
     * 通过handler（构造方法用MainLooper），实现子线程也能Toast
     *
     * @param
     * @param msg
     * @param resId
     */
    private static void show(String msg, int resId) {
        if (mainHandler == null) {
            synchronized (ToastUtils.class) {
                if (mainHandler == null) {
                    mainHandler = new Handler(context.getMainLooper()) {
                        @Override
                        public void handleMessage(Message msg) {
                            super.handleMessage(msg);
                            if (msg.obj != null) {
                                showString(context, String.valueOf(msg.obj), msg.arg2);
                            } else {
                                showInt(context, msg.arg1, msg.arg2);
                            }
                        }
                    };
                }
            }
        }
        Message message = mainHandler.obtainMessage(1, resId, Toast.LENGTH_SHORT, msg);
        mainHandler.sendMessage(message);
    }

    private static void showInt(Context context, int resId, int time) {
        if (toast == null) {
            toast = Toast.makeText(context, "", time);
        }
        toast.setText(resId);
        toast.setGravity(Gravity.CENTER, 0, 0);
        toast.show();
    }

    private static void showString(Context context, String msg, int time) {
        if (toast == null) {
            toast = Toast.makeText(context, "", time);
        }
        toast.setText(msg);
        toast.setGravity(Gravity.CENTER, 0, 0);
        toast.show();
    }
}

