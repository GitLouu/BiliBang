package com.gitee.fsmxm.bilibang;

import java.util.List;

/**
 * 接口相应结果
 */
public class ResponseResult {
    private Integer code;
    private String message;
    private List<Result> result;

    public Integer getCode() {
        return code;
    }

    public void setCode(Integer code) {
        this.code = code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public List<Result> getResult() {
        return result;
    }

    public void setResult(List<Result> result) {
        this.result = result;
    }
}
