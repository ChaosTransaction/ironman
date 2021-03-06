//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include"simnow/interface_md.h"
#include "basic/radom_in.h"
#include "logic/logic_comm.h"
#include "protocol/data_packet.h"
namespace simnow_logic{

SimNowMDAPI::~SimNowMDAPI() {
  md_api_->RegisterSpi(NULL);
  md_api_->Release();
  md_api_ = NULL;
}

void SimNowMDAPI::Init(const std::string& path,
                       const std::string& address) {

  CreateFtdcMdApi(path);
  RegisterFront(address);
  md_api_->Init();
}

void SimNowMDAPI::Init(struct server* srv){
  srv_ = srv;
}

void SimNowMDAPI::SetUserInfo(const std::string& user_id, const std::string& password,
                            const std::string& broker_id) {
   user_id_ = user_id;
   password_ = password;
   broker_id_ = broker_id;
}

void SimNowMDAPI::CreateFtdcMdApi(const std::string& path) {
  md_api_ = CThostFtdcMdApi::CreateFtdcMdApi(path.c_str());
  md_api_->RegisterSpi(this);
}


void SimNowMDAPI::RegisterFront(const std::string& address){
  md_api_->RegisterFront((char*)(address.c_str()));
}


void SimNowMDAPI::UserLogin() {
  CThostFtdcReqUserLoginField req_user_login;
  int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
  memset(&req_user_login,0,sizeof(req_user_login));
  strncpy(req_user_login.UserID,   user_id_.c_str(), user_id_.length());
  strncpy(req_user_login.Password, password_.c_str(), password_.length());
  strncpy(req_user_login.BrokerID, broker_id_.c_str(), broker_id_.length());
  md_api_->ReqUserLogin(&req_user_login, request_id);
}

void SimNowMDAPI::SubScribeMarketData(char** instrument_id, int32 num) {
    md_api_->SubscribeMarketData(instrument_id, num);
}


void SimNowMDAPI::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *specific_instrument,CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last) {
  LOG_DEBUG2("nRequestID %d  InstrumentID %s", request_id, specific_instrument->InstrumentID);
  SetTask(MD_SCRIBE_MARKET_DATA,request_id,(void*)(specific_instrument),
        sizeof(CThostFtdcSpecificInstrumentField));
}

void SimNowMDAPI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
  /*int32 packet_length = sizeof(CThostFtdcDepthMarketDataField) + sizeof(int16);
  packet::DataOutPacket out(false, packet_length);
  out.Write16(packet_length);
  out.WriteData((void*)(pDepthMarketData),sizeof(CThostFtdcDepthMarketDataField));
  srv_->set_event_task(srv,reinterpret_cast<void *>(const_cast<char *>(out.GetData())),
                        packet_length);*/
  
  //SetTask(MD_MARKET_DATA, 0, (void*)(pDepthMarketData),sizeof(CThostFtdcDepthMarketDataField));
}



void SimNowMDAPI::OnRspUserLogin(CThostFtdcRspUserLoginField *user_login,
                             CThostFtdcRspInfoField *rsp_info,
                             int request_id, bool is_last) {
  LOG_DEBUG2("OnRspUserLogin successs nRequestID %d\n", request_id);
  SetTask(MD_USER_LOGIN,request_id,(void*)(user_login),sizeof(CThostFtdcRspUserLoginField));
}

void SimNowMDAPI::OnFrontConnected(){ //连接成功
  LOG_DEBUG("connect successs!\n");
  LOG_DEBUG2("%s",md_api_->GetTradingDay());
  SetTask(MD_USER_CONNECTED,0,NULL,0);
}

void SimNowMDAPI::OnFrontDisconnected(int reason) {
    LOG_DEBUG("disconnected");
}

void SimNowMDAPI::SetTask(int16 code, int32 request_id, void* data, size_t data_length) {
    //sleep(10);
    int32 packet_length = data_length + sizeof(int16) + sizeof(int16) + sizeof(int32);
    packet::DataOutPacket out(true, packet_length);
    out.Write16(packet_length);
    out.Write16(code);
    out.Write32(request_id);
    if (data_length == 0)
        out.WriteData(const_cast<const char*>(reinterpret_cast<char *>(data)),data_length);
    srv_->set_event_task(srv_,reinterpret_cast<void *>(const_cast<char *>(out.GetData())),
                        packet_length);
}

}
