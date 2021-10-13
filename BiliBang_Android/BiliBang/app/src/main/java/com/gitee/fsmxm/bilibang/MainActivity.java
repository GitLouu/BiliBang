package com.gitee.fsmxm.bilibang;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.viewpager2.widget.ViewPager2;

import com.google.gson.Gson;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.ResponseBody;

public class MainActivity extends AppCompatActivity {
    private OkHttpClient okHttpClient;
    private ViewPager2 viewPager;
    private TextView tvDate;
    private int maxPage;
    private ResResult resResult = new ResResult();
    private final String[] timelineUrls = new String[] {
            "https://bangumi.bilibili.com/web_api/timeline_global",
            "https://bangumi.bilibili.com/web_api/timeline_cn"
    };
    private int currentTimeline;
    
    private final Handler handler = new Handler(Looper.myLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            if (msg.what == 0) {
                viewPager.setAdapter(new BangumiPagerAdapter(resResult, MainActivity.this));
                int currentPos = findCurrentPos(resResult);
                MainActivity.this.setTitle(currentPos);
                viewPager.setCurrentItem(currentPos);
            }
        }
    };

    public void setTitle(int currentPos) {
        if (resResult != null && resResult.getResult() != null) {
            Result result = resResult.getResult().get(currentPos);
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        viewPager = findViewById(R.id.vp);
        tvDate = findViewById(R.id.tv_date);
        Toolbar toolbar = findViewById(R.id.toolbar);
        toolbar.setNavigationOnClickListener(v -> MainActivity.this.finish());

        okHttpClient = new OkHttpClient();

        SharedPreferences sp = getSharedPreferences("BiliBang", Context.MODE_PRIVATE);
        currentTimeline = sp.getInt("currentTimeline", 0);

        readTimeline();

        viewPager.registerOnPageChangeCallback(new ViewPager2.OnPageChangeCallback() {
            @Override
            public void onPageSelected(int position) {
                MainActivity.this.setTitle(position);
            }
        });

    }

    private void readTimeline() {
        Request request = new Request.Builder().url(timelineUrls[currentTimeline])
                .build();

        SharedPreferences sp = getSharedPreferences("BiliBang", Context.MODE_PRIVATE);
        sp.edit().putInt("currentTimeline", currentTimeline).apply();

        if (++currentTimeline > timelineUrls.length - 1) {
            currentTimeline = 0;
        }

        Call call = okHttpClient.newCall(request);

        call.enqueue(new Callback() {
            @Override
            public void onFailure(@NotNull Call call, @NotNull IOException e) {
                Toast.makeText(MainActivity.this, "无法访问", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onResponse(@NotNull Call call, @NotNull Response response) throws IOException {
                ResponseBody body = response.body();
                if (body != null) {
                    Gson gson = new Gson();
                    MainActivity.this.resResult = gson.fromJson(body.string(), ResResult.class);
                    MainActivity.this.fillData();
                    handler.sendEmptyMessage(0);
                }
            }
        });
    }

    private int findCurrentPos(ResResult resResult) {
        if (resResult == null) {
            return 0;
        }
        List<Result> results = resResult.getResult();
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

    private void fillData() {
        if (resResult == null) {
            return;
        }
        List<Result> results = resResult.getResult();
        if (results == null) {
            return;
        }
        Bangumi empty = new Bangumi();
        empty.setPub_time("     ");
        empty.setTitle("空空如也");
        empty.setPub_index("今天没有动画哦~");
        for (Result res : results) {
            List<Bangumi> seasons = res.getSeasons();
            if (seasons == null || seasons.isEmpty()) {
                seasons.add(empty);
            }
        }
    }

    public void changeTimeline(View view) {
        readTimeline();
    }
}