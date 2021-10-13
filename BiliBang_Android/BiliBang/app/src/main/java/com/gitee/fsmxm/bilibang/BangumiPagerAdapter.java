package com.gitee.fsmxm.bilibang;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.ListView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class BangumiPagerAdapter extends RecyclerView.Adapter<BangumiPagerAdapter.ViewHolder> {

    private ResResult resResult;
    private Context context;

    public BangumiPagerAdapter(ResResult resResult, Context context) {
        this.resResult = resResult;
        this.context = context;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater
                .from(context).inflate(R.layout.viewpager, parent, false));
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        holder.listView.setAdapter(new BangumiListAdapter(resResult.getResult().get(position)));
    }

    @Override
    public int getItemCount() {
        if (resResult == null) {
            return 0;
        }
        List<Result> result = resResult.getResult();
        return result == null ? 0 : result.size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        private LinearLayout container;
        private ListView listView;
        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            container = itemView.findViewById(R.id.vp_container);
            listView = itemView.findViewById(R.id.lv);
        }

    }
}
