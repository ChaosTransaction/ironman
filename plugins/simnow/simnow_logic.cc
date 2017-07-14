//  Copyright (c) 2017-2018 The IRNMAN Authors. All rights reserved.
//  Created on: 2017年07月13日 Author: kerry

#include "simnow/simnow_logic.h"
#include "logic/logic_comm.h"
#include "basic/native_library.h"
#include "config/config.h"
#include "core/common.h"
#include "protocol/data_packet.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/simnow/simnow_config.xml"
#define CUSTOM_CONFIG_PATH "./plugins/simnow/custom_config.xml"

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

    r = config->LoadConfig(path);
    LOG_MSG2("path : %s", path.c_str());

    simnow_logic::SimNowConfig simnow_config;
    r = ParserCustomXml(CUSTOM_CONFIG_PATH, simnow_config);
    if (!r)
      return false;



    std::string spath;
    std::string sddress = "tcp://180.168.146.187:10011";

    md_api_ = new simnow_logic::SimNowMDAPI();
    md_api_->Init(simnow_config.md_path(), simnow_config.md_address());
    md_api_->SetUserInfo(simnow_config.md_user_id(),
                          simnow_config.md_password(),
                          simnow_config.md_broker_id());

    md_api_->UserLogin();

    /*std::string t_broker_id = "9999";
    std::string t_inverstor_id = "096960";
    std::string t_password = "kerry0903";
    std::string tpath;
    std::string tddress = "tcp://180.168.146.187:10001";
    trader_api_ = new simnow_logic::SimNowTraderAPI();
    trader_api_->Init(tpath, tddress);
    trader_api_->SetUserInfo(t_broker_id, t_inverstor_id, t_password);*/
    return true;

}

void SimNowlogic::InitSrv(struct server* srv) {
  //md_api_->Init(srv);
    trader_api_->Init(srv);
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
    packet::DataInPacket in(
          reinterpret_cast<char*>(const_cast<void*>(msg)), len);
    int16 packet_length = in.Read16();
    int16 operate_code = in.Read16();
    LOG_MSG2("packet_length %d operate_code %d len %d", packet_length, operate_code, len);
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


bool SimNowlogic::ParserCustomXml(const std::string& file, SimNowConfig& config) {
  bool r = false;
  logic::XmlParser xml_parser;
  base_logic::DictionaryValue* md = NULL;
  std::string user_id；
  std::string password;
  std::string broker_id;
  std::string address;
  std::string path;

  r = value->GetDictionary(L"md",&md);
  if (!r) {
    if (value) {delete value; value = NULL;}
    return false;
  }

  r = md->GetString(L"uid", &user_id);
  if (!r) {
    if (value) {delete value; value = NULL;}
    return false;
  }

  r = md->GetString(L"pwd", &password);
  if (!r) {
    if (value) {delete value; value = NULL;}
    return false;
  }

  r = md->GetString(L"broker", &broker_id);
  if (!r) {
    if (value) {delete value; value = NULL;}
    return false;
  }

  r = md->GetString(L"address", &address);
  if (!r) {
    if (value) {delete value; value = NULL;}
    return false;
  }

  r = md->GetString(L"path", &path);
  if (!r) {
    if (value) {delete value; value = NULL;}
    return false;
  }

  config.set_md_user(user_id, password, broker_id, address, path);

  if (value) {delete value; value = NULL;}
  return true;
}

}  // namespace simnow_logic
