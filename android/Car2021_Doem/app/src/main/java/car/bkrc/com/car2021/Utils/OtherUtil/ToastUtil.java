package car.bkrc.com.car2021.Utils.OtherUtil;

import android.content.Context;
import android.os.Looper;
import android.widget.Toast;

public class ToastUtil {
    private Context context;
    static Toast toast = null;

    public ToastUtil(Context context) {
        this.context = context;
    }

    public void ShowToast(String msg) {
        try{
            if(toast!=null){
                toast.setText(msg);
            }else{
                toast=Toast.makeText(context,msg,Toast.LENGTH_SHORT);
            }
            toast.show();
        }catch (Exception e){
            Looper.prepare();
            Toast.makeText(context,msg,Toast.LENGTH_SHORT).show();
            Looper.loop();
        }
//        toast = Toast.makeText(context, msg, Toast.LENGTH_LONG);
//        toast.setText(msg);
//        toast.show();
    }
}
