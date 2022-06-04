package com.gitee.fsmxm.bilibang;

import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.viewpager2.widget.ViewPager2;

import com.google.gson.Gson;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.ResponseBody;

public class MainActivity extends AppCompatActivity {
    private static final int SUCCESS = 0;
    private OkHttpClient okHttpClient;
    private ViewPager2 viewPager;
    private TextView tvDate;
    private int maxPage;
    private final List<ResponseResult> responseResults = Collections.synchronizedList(new ArrayList<>(2));
    private ResponseResult responseResult;
    private final String[] timelineUrls = new String[]{
            "https://bangumi.bilibili.com/web_api/timeline_global",
            "https://bangumi.bilibili.com/web_api/timeline_cn"
    };

    static final Message MSG_CANNOT_ACCESS = Message.obtain();
    static final Message MSG_404_ACCESS = Message.obtain();
    static final Message MSG_ERROR = Message.obtain();

    static {
        MSG_CANNOT_ACCESS.what = -2;
        MSG_CANNOT_ACCESS.obj = "无法访问";
        MSG_ERROR.what = -1;
        MSG_ERROR.obj = "出错";
        MSG_404_ACCESS.what = 404;
        MSG_404_ACCESS.obj = "无法访问";
    }

    private Handler handler;

    @RequiresApi(api = Build.VERSION_CODES.N)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        viewPager = findViewById(R.id.vp);
        tvDate = findViewById(R.id.tv_date);
        tvDate.setText("读取中...");
        Toolbar toolbar = findViewById(R.id.toolbar);
        toolbar.setNavigationOnClickListener(v -> finish());

        viewPager.registerOnPageChangeCallback(new ViewPager2.OnPageChangeCallback() {
            @Override
            public void onPageSelected(int position) {
                setTitle(position);
            }
        });

        new Thread(() -> {
            Looper.prepare();
            handler = new Handler(Looper.myLooper()) {
                @RequiresApi(api = Build.VERSION_CODES.R)
                @Override
                public void handleMessage(@NonNull Message msg) {
                    if (msg.what == SUCCESS) {
                        MainActivity.this.runOnUiThread(() -> {
                            viewPager.setAdapter(new BangumiPagerAdapter(responseResult, MainActivity.this));
                            int currentPos = findCurrentPos(responseResult);
                            setTitle(currentPos);
                            viewPager.setCurrentItem(currentPos);
                        });
                    } else {
                        MainActivity.this.runOnUiThread(() -> tvDate.setText("出错"));
                        Toast.makeText(MainActivity.this, msg.what + ": " + msg.obj, Toast.LENGTH_SHORT).show();
                    }
                }
            };
            Looper.loop();
        }).start();

        okHttpClient = new OkHttpClient();
        readTimeline();

    }

    public void setTitle(int currentPos) {
        if (responseResult != null && responseResult.getResult() != null) {
            Result result = responseResult.getResult().get(currentPos);
            if (result != null) {
                StringBuilder title = new StringBuilder();
                if (currentPos != 0) {
                    title.append("< ");
                }
                title.append(result.getDate())
                        .append("(")
                        .append(getWeek(result.getDay_of_week()))
                        .append(")");
                if (currentPos != maxPage) {
                    title.append(" >");
                }
                tvDate.setText(title.toString());
            }
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    private void readTimeline() {
        new Thread(() -> {
            responseResults.clear();
            CountDownLatch latch = new CountDownLatch(timelineUrls.length);
            for (String url : timelineUrls) {
                readTimeline(latch, url);
            }
            try {
                latch.await();
                handleData();
                handler.sendEmptyMessage(SUCCESS);
            } catch (InterruptedException e) {
                handler.sendMessage(Message.obtain(MSG_ERROR));
            }
            responseResults.clear();
        }).start();
    }

    private void readTimeline(CountDownLatch latch, String url) {
        Request request = new Request.Builder().url(url)
                .build();

        Call call = okHttpClient.newCall(request);

        call.enqueue(new Callback() {
            @Override
            public void onFailure(@NotNull Call call, @NotNull IOException e) {
                handler.sendMessage(Message.obtain(MSG_CANNOT_ACCESS));
            }

            @Override
            public void onResponse(@NotNull Call call, @NotNull Response response) throws IOException {
                ResponseBody body = response.body();
                if (response.code() == 200 && body != null) {
                    Gson gson = new Gson();
                    responseResults.add(gson.fromJson(body.string(), ResponseResult.class));
                } else {
                    handler.sendMessage(Message.obtain(MSG_404_ACCESS));
                }
                latch.countDown();
            }
        });
    }

    private int findCurrentPos(ResponseResult responseResult) {
        if (responseResult == null) {
            return 0;
        }
        List<Result> results = responseResult.getResult();
        if (results == null || results.isEmpty()) {
            return 0;
        }
        maxPage = results.size() - 1;
        for (int i = 0; i < results.size(); i++) {
            if (results.get(i).getIs_today().equals(1)) {
                return i;
            }
        }
        return 0;
    }

    private String getWeek(int dayOfWeek) {
        switch (dayOfWeek) {
            case 1:
                return "周一";
            case 2:
                return "周二";
            case 3:
                return "周三";
            case 4:
                return "周四";
            case 5:
                return "周五";
            case 6:
                return "周六";
            case 7:
                return "周日";
            default:
                return "";
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    private void handleData() {
        for (ResponseResult rr : responseResults) {
            if (responseResult == null) {
                responseResult = rr;
            } else {
                List<Result> results = rr.getResult();
                if (results == null || results.isEmpty()) {
                    continue;
                }
                List<Result> rs = responseResult.getResult();
                for (int i = 0; i < rs.size(); i++) {
                    List<Bangumi> seasons = rs.get(i).getSeasons();
                    seasons.addAll(results.get(i).getSeasons());
                    seasons.sort(Comparator.comparing(Bangumi::getPub_time));
                }
            }
        }
    }
}