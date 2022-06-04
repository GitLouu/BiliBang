package com.gitee.fsmxm.bilibang;

import android.content.Intent;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.resource.bitmap.CircleCrop;
import com.bumptech.glide.load.resource.drawable.DrawableTransitionOptions;
import com.bumptech.glide.request.RequestOptions;

import java.util.List;

public class BangumiListAdapter extends BaseAdapter {

    private final Result result;

    RequestOptions requestOptions = new RequestOptions()
            .placeholder(R.drawable.ic_baseline_cloud_circle_24)
            .error(R.drawable.ic_baseline_cloud_circle_24)
            .fallback(R.drawable.ic_baseline_cloud_circle_24);

    public BangumiListAdapter(Result result) {
        this.result = result;
    }


    @Override
    public int getCount() {
        if (result == null) {
            return 0;
        }
        List<Bangumi> seasons = result.getSeasons();
        return seasons == null ? 0 : seasons.size();
    }

    @Override
    public Object getItem(int position) {
        if (result == null) {
            return null;
        }
        List<Bangumi> seasons = result.getSeasons();
        return seasons == null ? null : seasons.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = LayoutInflater.from(parent.getContext()).inflate(R.layout.listitem, parent, false);
            ViewHolder viewHolder = new ViewHolder();
            viewHolder.time = convertView.findViewById(R.id.tv_time);
            viewHolder.title = convertView.findViewById(R.id.tv_title);
            viewHolder.episode = convertView.findViewById(R.id.tv_episode);
            viewHolder.cover = convertView.findViewById(R.id.iv_cover);
            convertView.setTag(viewHolder);
            convertView.setOnClickListener(v -> {
                Bangumi bangumi = result.getSeasons().get(position);
                String url = bangumi.getUrl();
                if (!"".trim().equals(url)) {
                    Intent intent = new Intent();
                    intent.setData(Uri.parse(url));
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    parent.getContext().startActivity(intent);
                }
            });
        }

        ViewHolder viewHolder = (ViewHolder) convertView.getTag();

        Bangumi bangumi = result.getSeasons().get(position);
        viewHolder.time.setText(bangumi.getPub_time());
        viewHolder.title.setText(bangumi.getTitle());
        String pub_index = bangumi.getPub_index();
        if (pub_index == null) {
            pub_index = bangumi.getDelay_index() + "(" + bangumi.getDelay_reason() + ")";
        }
        viewHolder.episode.setText(pub_index);
        Glide.with(convertView)
                .load(bangumi.getSquare_cover())
                .apply(requestOptions)
                .transition(DrawableTransitionOptions.withCrossFade())
                .transform(new CircleCrop())
                .into(viewHolder.cover);

        return convertView;
    }

    static class ViewHolder {
        TextView time, title, episode;
        ImageView cover;
    }
}
