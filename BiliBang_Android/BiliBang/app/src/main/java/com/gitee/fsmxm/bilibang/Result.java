package com.gitee.fsmxm.bilibang;

import java.util.List;

public class Result {
    private String date;
    private String date_ts;
    private Integer day_of_week;
    private Integer is_today;
    private List<Bangumi> seasons;

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getDate_ts() {
        return date_ts;
    }

    public void setDate_ts(String date_ts) {
        this.date_ts = date_ts;
    }

    public Integer getDay_of_week() {
        return day_of_week;
    }

    public void setDay_of_week(Integer day_of_week) {
        this.day_of_week = day_of_week;
    }

    public Integer getIs_today() {
        return is_today;
    }

    public void setIs_today(Integer is_today) {
        this.is_today = is_today;
    }

    public List<Bangumi> getSeasons() {
        return seasons;
    }

    public void setSeasons(List<Bangumi> seasons) {
        this.seasons = seasons;
    }
}
