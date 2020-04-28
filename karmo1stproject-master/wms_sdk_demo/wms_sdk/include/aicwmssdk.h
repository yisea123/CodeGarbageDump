#ifndef AICWMSSDK_H
#define AICWMSSDK_H

#include <iostream>
#include <functional>
#include <mutex>
#include <vector>
#include "commu.pb.h"
#include "wms_commu.pb.h"
#include "aic_commu.h"
#include "wms_commu_def.h"

using namespace std;




namespace aicrobot{

//重定向服务器信息
typedef struct {
    string  server_ip;  //重定向服务器ip
    int     req_port;   //重定向服务器请求端口
}RedirectServerInfo;

//接收数据回调函数类型
using ReqRecvFunc = std::function<void(const commu::ResponseData&)>;        //请求回调函数
using SubRecvFunc = std::function<void(const commu::NotifyData&)>;          //订阅回调函数
using RecvRedirectListFunc = std::function<void(const vector<RedirectServerInfo> server_infos)>;    //重定向服务器列表更新回调函数

class AICWmsSDK
{
public:

//和调度通信状态
enum class CommuStatus{
    kConnected                  = 1,    //连接上调度系统服务器
    kDisconnected,                      //与调度系统服务器断开连接
    kRequestTimeout,                    //发送调度系统请求超时
    kNoScheduleServer,                  //没有可用的调度系统
//    kAllRedirectServerConnectFail,      //所有重定向服务器连接失败
};

public:
     /**
     * @brief instance  获取sdk单例对象
     * @return
     */
    static AICWmsSDK* instance();

    /**
     * @brief run           启动和调度系统的通信服务
     * @param redirect_servers     重定向服务器数组,tuple第一个值为服务器的ip，第二个为端口
     * @return
     */
    bool    run(const vector<tuple<string, int> > &redirect_servers);


    /**
     * @brief requestDoTask     请求执行任务
     * @param req               请求的数据
     * @param callback          请求结果回调
     * @return
     */
    bool    requestDoTask(const wms_commu::DoTaskRequest& req,ReqRecvFunc callback);

    /**
     * @brief cancelTask    请求取消任务
     * @param req           请求的数据
     * @param callback      请求结果回调
     * @return
     */
    bool    cancelTask(const wms_commu::CancelTaskRequest& req,ReqRecvFunc callback);

    /**
     * @brief setSubRecvFunc    设置订阅结果回调函数
     * @param callback  订阅回调函数
     */
    void    setSubRecvFunc(SubRecvFunc callback);

    /**
     * @brief setCommuStatusCallback    设置sdk通信状态回调函数
     */
    void    setCommuStatusCallback(function<void(CommuStatus)> callback);

    /**
     * @brief setPackPrintFunc  设置封包数据打印函数
     * @param callback  打印回调函数
     */
    void    setPackPrintFunc(PrintPackCall callback);

    /**
     * @brief setLogFunc    设置打印输出回调函数
     * @param callback      打印输出回调函数
     */
    void    setLogFunc(LogCall callback);

    /**
     * @brief setRecvRedirectServeListFunc  设置重定向服务器列表更新回调函数
     * @param callback
     */
    void    setRecvRedirectServeListFunc(RecvRedirectListFunc callback);

    bool requestScheduleServerList();

private:
    AICWmsSDK();

    void reconnect();
    bool connectToRedirectServer();
    bool connectToRedirectServer(string ip,int req_port);
    bool connectToScheduleServer(string ip, int req_port, int sub_port);
    bool requestRedirectServerList();


    void redirectServerSocketStatusCB(AicCommuStatus status, const std::string &msg);
    void scheduleServerSocketStatusCB(AicCommuStatus status, const std::string &msg);
    void subsRecvFunc(const std::string *p_sub, bytes_ptr data_in, bytes_ptr data_out);
    string makeSubscriber(wms::Subscriber subscriber);

private:
    static mutex       instance_mutex_;
    static AICWmsSDK   *instance_;

    vector<RedirectServerInfo> redirect_servers_;      //重定向服务器数组
    int cur_redirect_index_ = -1;    //当前尝试连接的重定向服务器在vector中的索引
    AICRequester       redirect_server_req_     = nullptr; //重定向服务器socket
    AICRequester       requester_               = nullptr; //请求socket
    AICSubscriber      subscriber_              = nullptr; //订阅socket
    LogCall         log_callback_               = nullptr; //日志回调
    PrintPackCall   pack_callback_              = nullptr; //数据包回调
    SubRecvFunc     sub_callback_               = nullptr; //订阅数据回调
    RecvRedirectListFunc    redirect_list_callback_ = nullptr;  //重定向服务器列表更新回调函数
    function<void(CommuStatus)> commu_status_callback_  = nullptr;  //sdk通信状态回调
    bool started    = false;    //通信是否已启动
};

}
#endif // AICWMSSDK_H
