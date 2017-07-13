//  Copyright (c) 2017-2018 The IRNMAN Authors. All rights reserved.
//  Created on: 2017年07月13日 Author: kerry

#include "simnow/simnow_logic.h"
#include "logic/logic_comm.h"
#include "basic/native_library.h"
#include "config/config.h"
#include "core/common.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/simnow/simnow_config.xml"

namespace simnow_logic{

SimNowlogic *SimNowlogic::instance_ = NULL;

SimNowlogic::SimNowlogic()
:md_api_(NULL)
,trader_api_(NULL){
    if (!Init())
        assert(0);
}

SimNowlogic::~SimNowlogic(){
  if (md_api_){delete md_api_;md_api_ = NULL;}
  if (trader_api_) {delete trader_api_; trader_api_ = NULL;}
}


bool SimNowlogic::Init()
{
    bool r = false;
    std::string path = DEFAULT_CONFIG_PATH;
    config::FileConfig *config = config::FileConfig::GetFileConfig();
    if (config == NULL)
        return false;

    std::string spath;
    std::string sddress = "tcp://180.168.146.187:10011";
    r = config->LoadConfig(path);
    LOG_MSG2("path : %s", path.c_str());

    //md_api_ = new simnow_logic::SimNowMDAPI();
  //  md_api_->Init(spath, sddress);


    std::string tpath;
    std::string tddress = "tcp://180.168.146.187:10001";
    trader_api_ = new simnow_logic::SimNowTraderAPI();
    trader_api_->Init(tpath, tddress);
    return true;
}

void SimNowlogic::InitSrv(struct server* srv) {
  md_api_->Init(srv);
}

SimNowlogic *SimNowlogic::GetInstance()
{
    if (instance_ == NULL)
        instance_ = new SimNowlogic();
    return instance_;
}

void SimNowlogic::FreeInstance()
{
    delete instance_;
    instance_ = NULL;
}

bool SimNowlogic::OnSimNowConnect(struct server *srv, const int socket)
{
    return true;
}

bool SimNowlogic::OnSimNowMessage(struct server *srv, const int socket,
                                  const void *msg, const int len)
{
    bool r = false;
    return r;
}

bool SimNowlogic::OnSimNowClose(struct server *srv, const int socket)
{
    return true;
}

bool SimNowlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
    return true;
}

bool SimNowlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
    bool r = false;
    struct PacketHead *packet = NULL;
    return true;
}


bool SimNowlogic::OnBroadcastClose(struct server *srv, const int socket)
{
    return true;
}

bool SimNowlogic::OnIniTimer(struct server *srv)
{
    return true;
}

bool SimNowlogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time)
{
    return true;
}

}  // namespace simnow_logic
