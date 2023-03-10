package car.bkrc.com.car2021.ViewAdapter;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Message;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import car.bkrc.com.car2021.My_Code.IPCAM;
import car.bkrc.com.car2021.Utils.CameraUtile.XcApplication;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.ChecksumException;
import com.google.zxing.DecodeHintType;
import com.google.zxing.FormatException;
import com.google.zxing.NotFoundException;
import com.google.zxing.Result;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.QRCodeReader;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import car.bkrc.com.car2021.Utils.OtherUtil.RadiusUtil;
import car.bkrc.com.car2021.ActivityView.FirstActivity;
import car.bkrc.com.car2021.R;
import car.bkrc.com.car2021.FragmentView.LeftFragment;
import car.bkrc.com.car2021.DataProcessingModule.QR_Recognition;

import static car.bkrc.com.car2021.ActivityView.FirstActivity.toastUtil;

public class OtherAdapter extends RecyclerView.Adapter<OtherAdapter.ViewHolder> {

    private List<Other_Landmark> mOtherLandmarkList;
    Context context = null;
    static class ViewHolder extends RecyclerView.ViewHolder {
        View InfrareView;
        ImageView OtherImage;
        TextView OtherName;

        public ViewHolder(View view) {
            super(view);
            InfrareView = view;
            OtherImage = (ImageView) view.findViewById(R.id.landmark_image);
            OtherName = (TextView) view.findViewById(R.id.landmark_name);
        }
    }

    public OtherAdapter(List<Other_Landmark> InfrareLandmarkList, Context context) {
        mOtherLandmarkList = InfrareLandmarkList;
        this.context = context;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.other_item, parent, false);
        final ViewHolder holder = new ViewHolder(view);
        holder.OtherName.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int position = holder.getAdapterPosition();
                Other_Landmark otherLandmark = mOtherLandmarkList.get(position);
                Other_select(otherLandmark);
            }
        });
        holder.OtherImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                int position = holder.getAdapterPosition();
                Other_Landmark otherLandmark = mOtherLandmarkList.get(position);
                Other_select(otherLandmark);
            }
        });

        return holder;
    }

    private Bitmap bitmap;
    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        Other_Landmark InfrareLandmark = mOtherLandmarkList.get(position);
        bitmap = BitmapFactory.decodeResource(context.getResources(),InfrareLandmark.getImageId(),null);
        bitmap = RadiusUtil.roundBitmapByXfermode(bitmap, bitmap.getWidth(), bitmap.getHeight(), 10);
        holder.OtherImage.setImageBitmap(bitmap);
        holder.OtherName.setText(InfrareLandmark.getName());
    }

    @Override
    public int getItemCount() {
        return mOtherLandmarkList.size();
    }


    private void Other_select(Other_Landmark InfrareLandmark) {
        switch (InfrareLandmark.getName()) {
            case "???????????????":
                position_Dialog();
                break;
            case "???????????????":
                QR_Dialog();
                break;
            case "???????????????":
                buzzerController();
                break;
            case "???????????????":
                lightController();
                break;
            default:
                break;
        }

    }


    private String result_qr;
    private Bitmap qrBitmap;
    private boolean qrRecState = false;
    // ????????????????????????
    @SuppressLint("HandlerLeak")
    Handler qrHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 10:
                    qrRecState = true;
                    qrBitmap = LeftFragment.bitmap;
                    if (qrBitmap != null) {
                        QRRecon();
                    } else toastUtil.ShowToast("????????????????????????????????????????????????????????????");
                    break;
                case 20:
                    toastUtil.ShowToast(result_qr);
                    break;
                case 30:
                    toastUtil.ShowToast("????????????????????????");
                    break;
                default:
                    break;
            }
        }
    };

    private void QRRecon(){
        new Thread(() -> {
            Result result;
            QR_Recognition rSource = new QR_Recognition(
                    bitmap2Gray(qrBitmap));
            try {
                BinaryBitmap binaryBitmap = new BinaryBitmap(
                        new HybridBinarizer(rSource));
                Map<DecodeHintType, String> hint = new HashMap<DecodeHintType, String>();
                hint.put(DecodeHintType.CHARACTER_SET, "utf-8");
                QRCodeReader reader = new QRCodeReader();
                result = reader.decode(binaryBitmap, hint);
                if (result.toString() != null) {
                    result_qr = result.toString();
                    qrHandler.sendEmptyMessage(20);
                }
                System.out.println("????????????");
            } catch (NotFoundException e) {
                e.printStackTrace();
                qrHandler.sendEmptyMessage(30);
            } catch (ChecksumException e) {
                e.printStackTrace();
            } catch (FormatException e) {
                e.printStackTrace();
            }
            qrRecState = false;
        }).start();
    }

    /**
     * ???????????????
     * @param bmSrc
     * @return
     */
    public Bitmap bitmap2Gray(Bitmap bmSrc) {
        // ????????????????????????
        if (bmSrc == null)
            return null;
        int width = bmSrc.getWidth();
        int height = bmSrc.getHeight();
        // ????????????????????????
        Bitmap bmpGray = null;
        bmpGray = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
        // ????????????
        Canvas c = new Canvas(bmpGray);
        Paint paint = new Paint();
        ColorMatrix cm = new ColorMatrix();
        cm.setSaturation(0);
        ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm);
        paint.setColorFilter(f);
        c.drawBitmap(bmSrc, 0, 0, paint);
        return bmpGray;
    }


    private int state_camera = 0;

    private void position_Dialog()  //??????????????????
    {
        final AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("??????????????????????????????");
        String[] set_item = {"??????????????? ???", "??????????????? ???", "??????????????? ???", "??????????????? ???", "??????????????? ???", "??????????????? ???", "??????????????? ???", "??????????????? ???","???????????????"};
        builder.setSingleChoiceItems(set_item, -1, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // TODO ???????????????????????????
                state_camera = which + 5;
                camerastate_control();
            }
        });
        builder.create().show();
    }



    private void QR_Dialog()  //????????????????????????
    {
        final AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("???????????????");
        String[] set_item = {"???????????????????????????", "?????????????????????"};
        builder.setSingleChoiceItems(set_item, -1,
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        // TODO Auto-generated method stub
                        if (which == 0) {
                            // ?????????????????????
                            if (!qrRecState){
                                qrHandler.sendEmptyMessage(10);
                            }
                        } else if (which == 1) {
                            // ?????????????????????
                            agv_QR_Dialog();
                        }
                    }
                });
        builder.create().show();
    }

    private void agv_QR_Dialog()  // ?????????????????????????????????????????????
    {
        final AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("????????????????????????????????????");
        String[] set_item = {"????????????", "????????????"};
        builder.setSingleChoiceItems(set_item, -1,
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        // TODO Auto-generated method stub
                        if (which == 0) {
                            FirstActivity.Connect_Transport.qr_rec(1);
                        } else if (which == 1) {
                            FirstActivity.Connect_Transport.qr_rec(2);
                        }
                    }
                });
        builder.create().show();
    }

    // ???????????????????????????????????????
    private void camerastate_control() {
        XcApplication.executorServicetor.execute(new Runnable() {
            public void run() {
                switch (state_camera) {
                    //??????????????????
                    case 1:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 0, 1);  //??????
                        break;
                    case 2:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 2, 1);  //??????
                        break;
                    case 3:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 4, 1);  //??????
                        break;
                    case 4:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 6, 1);  //??????
                        break;
                    // / 5-8   ???????????????1???4
                    case 5:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 30, 0);
                        break;
                    case 6:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 32, 0);
                        break;
                    case 7:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 34, 0);
                        break;
                    case 8:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 36, 0);
                        break;

                    //???????????????1-4, 9-12
                    case 9:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 33, 0);
                        break;
                    case 10:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 35, 0);
                        break;
                    case 11:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 37, 0);
                        break;
                    case 12:
                        LeftFragment.cameraCommandUtil.postHttp(FirstActivity.IPCamera, 39, 0);
                        break;
                    case 13:
                        IPCAM.Rest_IPCAM();
                        break;
                    default:
                        break;
                }
                state_camera = 0;
            }
        });
    }

    // ?????????
    private void buzzerController() {
        AlertDialog.Builder build = new AlertDialog.Builder(context);
        build.setTitle("???????????????");
        String[] im = {"??????", "??????"};
        build.setSingleChoiceItems(im, -1,
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        // TODO Auto-generated method stub
                        if (which == 0) {
                            // ???????????????
                            FirstActivity.Connect_Transport.buzzer(1);
                        } else if (which == 1) {
                            // ???????????????
                            FirstActivity.Connect_Transport.buzzer(0);
                        }
                    }
                });
        build.create().show();
    }

    // ??????????????????
    private void lightController() {
        AlertDialog.Builder lt_builder = new AlertDialog.Builder(context);
        lt_builder.setTitle("???????????????");
        String[] item = {"??????", "??????", "??????", "????????????"};
        lt_builder.setSingleChoiceItems(item, -1,
                new DialogInterface.OnClickListener() {

                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        // TODO Auto-generated method stub
                        if (which == 0) {
                            FirstActivity.Connect_Transport.light(1, 0);
                        } else if (which == 1) {
                            FirstActivity.Connect_Transport.light(0, 1);
                        } else if (which == 2) {
                            FirstActivity.Connect_Transport.light(0, 0);
                        } else if (which == 3) {
                            FirstActivity.Connect_Transport.light(1, 1);
                        }
                    }
                });
        lt_builder.create().show();
    }

}