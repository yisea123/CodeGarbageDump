#ifndef WMS_COMMU_DEF_H
#define WMS_COMMU_DEF_H

namespace wms{

//请求包类型
enum class RequestPackType{
  kDoTaskRequest = 1,           /*执行任务请求*/
  kCancelTaskRequest,           /*取消任务请求*/
};


//请求错误码
enum class ReqErrCode{
    kOk = 1,              //请求成功
    kErr,                 //请求失败
    kCancel,              //请求取消
    kParamErr,            //请求参数有误
    kInvalidReqErr,       //无效请求
    kProtoBufErr,         //protobuf错误
    kNoRobotErr,          //无可用机器人
    kWmsTaskRepeat,       //wms任务重复
    kNotMainSchedule,     //不是主调度
};

//数据订阅类型
enum class Subscriber{
    kTaskProgress=1,      //任务进度
    kNotMainSchedule,     //已不是主调度
};


}
#endif // WMS_COMMU_DEF_H
