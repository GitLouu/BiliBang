package com.gitee.fsmxm.bilibang;

public class Bangumi {
    private String cover;
    private Integer delay;
    private Integer delay_id;
    private String delay_index;
    private String delay_reason;
    private String ep_id;
    private Integer favorites;
    private Integer follow;
    private Integer is_published;
    private String pub_index;
    private String pub_time;
    private Long pub_ts;
    private Integer season_id;
    private Integer season_status;
    private String square_cover;
    private String title;
    private String url;

    public static final Bangumi Empty = new Bangumi();
    static {
        Empty.setPub_time("     ");
        Empty.setTitle("空空如也");
        Empty.setPub_index("今天没有动画哦~");
    }

    public String getCover() {
        return cover;
    }

    public void setCover(String cover) {
        this.cover = cover;
    }

    public Integer getDelay() {
        return delay;
    }

    public void setDelay(Integer delay) {
        this.delay = delay;
    }

    public Integer getDelay_id() {
        return delay_id;
    }

    public void setDelay_id(Integer delay_id) {
        this.delay_id = delay_id;
    }

    public String getDelay_index() {
        return delay_index;
    }

    public void setDelay_index(String delay_index) {
        this.delay_index = delay_index;
    }

    public String getDelay_reason() {
        return delay_reason;
    }

    public void setDelay_reason(String delay_reason) {
        this.delay_reason = delay_reason;
    }

    public String getEp_id() {
        return ep_id;
    }

    public void setEp_id(String ep_id) {
        this.ep_id = ep_id;
    }

    public Integer getFavorites() {
        return favorites;
    }

    public void setFavorites(Integer favorites) {
        this.favorites = favorites;
    }

    public Integer getFollow() {
        return follow;
    }

    public void setFollow(Integer follow) {
        this.follow = follow;
    }

    public Integer getIs_published() {
        return is_published;
    }

    public void setIs_published(Integer is_published) {
        this.is_published = is_published;
    }

    public String getPub_index() {
        return pub_index;
    }

    public void setPub_index(String pub_index) {
        this.pub_index = pub_index;
    }

    public String getPub_time() {
        return pub_time;
    }

    public void setPub_time(String pub_time) {
        this.pub_time = pub_time;
    }

    public Long getPub_ts() {
        return pub_ts;
    }

    public void setPub_ts(Long pub_ts) {
        this.pub_ts = pub_ts;
    }

    public Integer getSeason_id() {
        return season_id;
    }

    public void setSeason_id(Integer season_id) {
        this.season_id = season_id;
    }

    public Integer getSeason_status() {
        return season_status;
    }

    public void setSeason_status(Integer season_status) {
        this.season_status = season_status;
    }

    public String getSquare_cover() {
        return square_cover;
    }

    public void setSquare_cover(String square_cover) {
        this.square_cover = square_cover;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }
}
