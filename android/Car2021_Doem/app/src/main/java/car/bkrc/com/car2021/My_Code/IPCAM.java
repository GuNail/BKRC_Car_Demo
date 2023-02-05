package car.bkrc.com.car2021.My_Code;

import car.bkrc.com.car2021.ActivityView.FirstActivity;
import car.bkrc.com.car2021.FragmentView.LeftFragment;

public class IPCAM {
    public static void Rest_IPCAM(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 31, 0);
    }

    //预设位
    public static void UP_30_IPCAM(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 39, 0);
    }

    public static void Right_45_IPCAM(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 41, 0);
    }

    public static void Right_90_IPCAM(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 45, 0);
    }

    public static void Left_45_IPCAM(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 47, 0);
    }

    public static void Left_90_IPCAM(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 43, 0);
    }

    //微调
    public static void Right(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera,6,1);
    }

    public static void Left(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera,4,1);
    }

    public static void Up(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera,0,1);
    }

    public static void Down(){
        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera,2,1);
    }

}
