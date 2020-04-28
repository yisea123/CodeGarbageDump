#include "amrsystem.h"

AMRSystem::AMRSystem()
{
    init();

}

void AMRSystem::init(){


    vector<tuple<string, int>> ips;

    ips.push_back(tuple<string, int>("172.16.8.100", 10031));

    // 设置sdk输出回调

    AICWmsSDK::instance()->setLogFunc(sdkLogFunc);

    // 设置sdk通信状态回调

    AICWmsSDK::instance()->setCommuStatusCallback(sdkCommuStatusCallback);

    // 设置数据包输出回调

    AICWmsSDK::instance()->setPackPrintFunc(packLogCallback);

    // 设置订阅回调

    AICWmsSDK::instance()->setSubRecvFunc(sub_callback);

    // 重定向服务器列表回调

    AICWmsSDK::instance()->setRecvRedirectServeListFunc(RecvRedirectListCallback);

    // 启动和调度系统的通信服务

    AICWmsSDK::instance()->run(ips);

    //生成wms任务

    wms_commu::DoTaskRequest do_task_req;

    wms_commu::WmsTaskData *wms_task_data = do_task_req.mutable_wms_task();

    wms_commu::WmsTaskInfo wms_task_info;

    wms_task_info.set_task_id("task_id");

    wms_task_info.set_execute_by_order(true);

    wms_task_info.set_can_skip(true);

    wms_task_info.set_goto_pack_station(true);

    //任务:储物信息要符合PC界面编辑的实际储物
    std::string nos[5] = {"SCU011", "SCS011", "SCQ011", "SCO011", "SCM011"};

    std::vector<std::string> no_vec;
    for (int i = 0; i < 5; i++)
    { //生成5个子任务

        CarryTaskInfo carry_task;

        carry_task.set_storage_no(nos[i]);
        string carry_task_data;
        carry_task.SerializeToString(&carry_task_data);
        SubTaskData *sub_task_data = wms_task_info.add_sub_task_data();
        sub_task_data->set_type(1);
        sub_task_data->set_data(carry_task_data);
    }
    wms_task_data->set_source(1);
    wms_task_data->set_task_type(1);
    string *task_data = wms_task_data->add_task_data();

    wms_task_info.SerializeToString(task_data);
    //生成wms任务结束

    //生成回调匿名函数
    auto req_callback = [](const commu::ResponseData &response) {
        cout << "err_no:" << response.header().err_no() << " err_msg:" << response.header().err_msg() << endl;
        int err_no = response.header().err_no();
        if (err_no == static_cast<int>(wms::ReqErrCode::kErr))
        {
            return;
        }

        if (err_no == static_cast<int>(wms::ReqErrCode::kNotMainSchedule))
        { //不是主调度
            AICWmsSDK::instance()->requestScheduleServerList();
            return;
        }

        DoTaskResponse res;
        res.ParseFromString(response.body());
        cout << "wms_task_id:" << res.wms_task_id() << " robot_id:" << res.robot_id() << " robot_no:" << res.robot_no() << endl;
    };

    //请求执行任务
    AICWmsSDK::instance()->requestDoTask(do_task_req, req_callback);
}

void AMRSystem::sub_callback(const commu::NotifyData &notify_data)
{

    switch (static_cast<wms::Subscriber>(notify_data.header().type()))
    {
    case wms::Subscriber::kTaskProgress:
    {
        wms_commu::TaskProgressNotify notify;
        notify.ParseFromString(notify_data.body());
        cout << "------" << endl;
        cout << "wms_task_id:" << notify.wms_task_id() << endl;
        cout << "storage_no:" << notify.storage_no() << endl;
        cout << "status:" << notify.status() << endl;
        cout << "------" << endl;
    }
    break;
    default:
        break;
    }
}

void AMRSystem::RecvRedirectListCallback(const vector<RedirectServerInfo> server_infos){

    cout<<endl<<"**redirect server list:**"<<endl;
    for(RedirectServerInfo info : server_infos){
        cout<<"ip:"<<info.server_ip<<" port:"<<info.req_port<<endl;
    }
    cout<<endl;
}
