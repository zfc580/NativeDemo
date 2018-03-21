package com.choufucai.nativedemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = MainActivity.class.getSimpleName();
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Button mGetBtn, mSetBtn, mCallbackBtn;
    TextView mTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mGetBtn = findViewById(R.id.jni_demo_getparams);
        mSetBtn = findViewById(R.id.jni_demo_setparams);
        mCallbackBtn = findViewById(R.id.jni_demo_setcallback);
        mTextView = findViewById(R.id.sample_text);

        mGetBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mTextView.setText(stringFromJNI());
            }
        });

        mSetBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Bean bean = new Bean(10, "zfc");
                setParams(bean);
                int result = plus(10, 12);
                Log.i(TAG, "bean = " + bean + ", result = " + result);
            }
        });

        mCallbackBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setJniCallback(new JniCallback() {
                    @Override
                    public void onJniCalled(String str) {
                        //从Jni调回Java的函数
                        Log.i("MainActivity", "onJniCalled str = "+str);
                    }
                });
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int plus(int x, int y);
    public native void setParams(Bean bean);
    public native void setJniCallback(JniCallback callback);

}
