//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include"simnow/interface_trader.h"
#include "basic/basic_util.h"
#include "basic/radom_in.h"
#include "logic/logic_comm.h"
#include "protocol/data_packet.h"
namespace simnow_logic{

SimNowTraderAPI::~SimNowTraderAPI() {
  trader_api_->RegisterSpi(NULL);
  trader_api_->Release();
  trader_api_ = NULL;
}

void SimNowTraderAPI::Init(const std::string& path,
                       const std::string& address) {

  CreateFtdcTraderApi(path);
  trader_api_->SubscribePrivateTopic(THOST_TERT_RESUME);
  trader_api_->SubscribePublicTopic(THOST_TERT_RESUME);
  RegisterFront(address);
  trader_api_->Init();
}

void SimNowTraderAPI::Init(struct server* srv){
  srv_ = srv;
}

void SimNowTraderAPI::SetUserInfo(const std::string& broker_id,
                              const std::string& investor_id,
                              const std::string& password) {
  broker_id_ = broker_id;
  investor_id_ = investor_id;
  password_ = password;
}

void SimNowTraderAPI::CreateFtdcTraderApi(const std::string& path) {
  trader_api_ = 
        CThostFtdcTraderApi::CreateFtdcTraderApi(path.c_str());
  trader_api_->RegisterSpi(this);
}


void SimNowTraderAPI::RegisterFront(const std::string& address){
  trader_api_->RegisterFront((char*)(address.c_str()));
}


void SimNowTraderAPI::ReqQryInstrument() {
  struct CThostFtdcQryInstrumentField qry_instrument;
  int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
  memset(&qry_instrument, 0, sizeof(qry_instrument));
  trader_api_->ReqQryInstrument(&qry_instrument,request_id);
}

void SimNowTraderAPI::UserLogin() {
  CThostFtdcReqUserLoginField req_user_login;
  int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
  memset(&req_user_login,0,sizeof(req_user_login));
  strncpy(req_user_login.UserID,   investor_id_.c_str(), investor_id_.length());
  strncpy(req_user_login.Password, password_.c_str(), password_.length());
  strncpy(req_user_login.BrokerID, broker_id_.c_str(), broker_id_.length());
  trader_api_->ReqUserLogin(&req_user_login, request_id);
}

void SimNowTraderAPI::OnFrontConnected(){ //连接成功
  LOG_DEBUG("connect successs!\n");
  LOG_DEBUG2("%s",trader_api_->GetTradingDay());
  UserLogin();
}

void SimNowTraderAPI::OnFrontDisconnected(int reason) {
    LOG_DEBUG("disconnected");
}


void SimNowTraderAPI::OnRspUserLogin(CThostFtdcRspUserLoginField *rsp_user_login,
                                CThostFtdcRspInfoField *rsp_info,
                                int request_id, bool is_last) {
    LOG_DEBUG2("==========> request_id %d", request_id);
    ReqQryInstrument();
    //SetTask(TRADER_USER_LOGIN, request_id, (void*)(rsp_user_login),
      //     sizeof(CThostFtdcRspUserLoginField)); 
}


void SimNowTraderAPI::OnRspQryInstrument(CThostFtdcInstrumentField *instrument,
                              CThostFtdcRspInfoField *rsp_info,
                              int request_id, bool is_last) {
    
    //LOG_DEBUG2("==========> request_id %d", request_id);
    /*char* input = instrument->InstrumentName;
    char* output = NULL;
    size_t outlen = 0;
    base::BasicUtil::CharracterSetConv("UTF-8", input, strlen(input),
                                      "gbk", &output, &outlen);

    LOG_DEBUG2("QryInstrument nRequestID:%d InstrumentID:%s ExchangeID:%s InstrumentName:%s  ExchangeInstID:%s ProductID:%s",
                request_id,instrument->InstrumentID,instrument->ExchangeID,
                output,instrument->ExchangeInstID,
                instrument->ProductID);
    if (output) {delete output; output = NULL;}*/
    SetTask(QRY_INSTRUMENT, request_id, (void*)(instrument),
                sizeof(struct CThostFtdcQryInstrumentField));
}



void SimNowTraderAPI::SetTask(int16 code, int32 request_id, void* data, size_t data_length) {
    //sleep(10);
    int32 packet_length = data_length + sizeof(int16) + sizeof(int16) + sizeof(int32);
    packet::DataOutPacket out(true, packet_length);
    out.Write16(packet_length);
    out.Write16(code);
    out.Write32(request_id);
    out.WriteData(const_cast<const char*>(reinterpret_cast<char *>(data)),data_length);
    srv_->set_event_task(srv_,reinterpret_cast<void *>(const_cast<char *>(out.GetData())),
                        packet_length);
}

}
