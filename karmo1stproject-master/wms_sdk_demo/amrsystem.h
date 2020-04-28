#ifndef AMRSYSTEM_H
#define AMRSYSTEM_H

#include "wms_sdk/include/aic_commu.h"
#include "wms_sdk/include/aicwmssdk.h"
#include "wms_sdk/include/commu.pb.h"
#include "wms_sdk/include/wms_commu.pb.h"
#include "wms_sdk/include/wms_commu_def.h"
#include "string"


using namespace aicrobot;
using namespace commu;
using namespace wms_commu;

class AMRSystem
{
public:
    AMRSystem();

    void init();

    bool sdkLogFunc(const string &msg){}
    void sdkCommuStatusCallback(aicrobot::AICWmsSDK::CommuStatus status){}
    bool packLogCallback(bool send_or_recv, aicrobot::AicCommuType sock_type, const std::string &prefix_msg, bytes_ptr data){}


    void sub_callback(const commu::NotifyData &notify_data);
    void RecvRedirectListCallback(const vector<RedirectServerInfo> server_infos);


};

#endif // AMRSYSTEM_H
