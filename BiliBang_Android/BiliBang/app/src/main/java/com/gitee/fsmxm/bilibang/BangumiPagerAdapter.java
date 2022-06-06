package com.gitee.fsmxm.bilibang;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class BangumiPagerAdapter extends RecyclerView.Adapter<BangumiPagerAdapter.ViewHolder> {

    private final ResponseResult responseResult;
    private final Context context;

    public BangumiPagerAdapter(ResponseResult responseResult, Context context) {
        this.responseResult = responseResult;
        this.context = context;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater
                .from(context).inflate(R.layout.viewpager, parent, false));
    }

    @RequiresApi(api = Build.VERSION_CODES.S)
    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        holder.listView.setAdapter(new BangumiListAdapter(responseResult.getResult().get(position)));
        holder.listView.setOnItemClickListener((parent, view, position1, id) -> {
            Object itemAtPosition = parent.getItemAtPosition(position1);
            if (!(itemAtPosition instanceof Bangumi)) {
                return;
            }
            Bangumi bangumi = (Bangumi) itemAtPosition;
            PackageManager packageManager = view.getContext().getPackageManager();
            Intent intent = new Intent();
            try {
                PackageInfo packageInfo = packageManager.getPackageInfo("tv.danmaku.bili", 0);
                intent.setPackage(packageInfo.packageName);
            } catch (PackageManager.NameNotFoundException e) {
                Log.i("BangumiPagerAdapter", "找不到package " + e.getLocalizedMessage());
            }
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setAction(Intent.ACTION_VIEW);
            intent.setData(Uri.parse(bangumi.getUrl()));
            view.getContext().startActivity(intent);
        });
    }

    @Override
    public int getItemCount() {
        if (responseResult == null) {
            return 0;
        }
        List<Result> result = responseResult.getResult();
        return result == null ? 0 : result.size();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        private final ListView listView;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            listView = itemView.findViewById(R.id.lv);
        }

    }
}
