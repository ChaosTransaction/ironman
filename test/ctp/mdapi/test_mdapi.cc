//  Copyright (c) 2015-2016 The kid Authors. All rights reserved.
//   Created on: 2016.12.23 Author: kerry

#include "gtest/gtest.h"
#include "file/file_path.h"
#include "file/file_util.h"
#include "basic/radom_in.h"
#include "logic/logic_comm.h"
#include "basic/basictypes.h"
#include "log/mig_log.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"
#include "http/http_header.h"
#include <string>
#include "api/simnow/ThostFtdcMdApi.h"
#include "api/simnow/ThostFtdcUserApiStruct.h"
#include "api/simnow/ThostFtdcUserApiDataType.h"
class MdApiTest: public testing::Test {
};

#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/time.h>
#include <errno.h>
#include "rdkafka.h"


//參照chrome的Lock實現
class SimNowMdAPI : public CThostFtdcMdSpi{
public:
    SimNowMdAPI();
    ~SimNowMdAPI();
public:
    virtual void OnFrontConnected();
   
    virtual void OnFrontDisconnected(int nReason);

    virtual void OnHeartBeatWarning(int nTimeLapse);

    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, 
                                CThostFtdcRspInfoField *pRspInfo, 
                                int nRequestID, bool bIsLast);

    virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, 
                                 CThostFtdcRspInfoField *pRspInfo, 
                                 int nRequestID, bool bIsLast);

    int Join();
    
    void RegisterFront(std::string& addresss);

    void RegisterNameServer(std::string& addresss);

    void RegisterSpi(CThostFtdcMdSpi* spi);

    void UserLogin(CThostFtdcReqUserLoginField* user_login);

    void Init();

    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
public:
    void CreateFtdcMdApi(std::string& pszFlowPath);

private:
    CThostFtdcMdApi* md_api_;
};

SimNowMdAPI::SimNowMdAPI() {
}

SimNowMdAPI::~SimNowMdAPI() {
   // md_api_->Join();
    md_api_->RegisterSpi(NULL);
    md_api_->Release();
    ////md_api_->Join();
    md_api_ = NULL;
}

void SimNowMdAPI::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, 
                                     CThostFtdcRspInfoField *pRspInfo, 
                                     int nRequestID, bool bIsLast) {
    LOG_DEBUG2("nRequestID %d  InstrumentID %s", nRequestID, pSpecificInstrument->InstrumentID);
}

void SimNowMdAPI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
    LOG_DEBUG2("TradingDay:%s InstrumentID:%s ExchangeID: %s ExchangeInstID %s\n LastPrice %f PreSettlementPrice %f PreClosePrice %f\nPreOpenInterest %f OpenPrice %f HighestPrice %f LowestPrice %f\nVolume %d Turnover %s OpenInterest %d\n  ClosePrice %f  SettlementPrice %f  UpperLimitPrice %f LowerLimitPrice %f \n  UpdateTime %lld UpdateMillisec %lld  AveragePrice %f  ActionDay %s",
               pDepthMarketData->TradingDay, pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID, 
               pDepthMarketData->ExchangeInstID,pDepthMarketData->LastPrice,pDepthMarketData->LastPrice,
               pDepthMarketData->PreSettlementPrice,pDepthMarketData->PreClosePrice,pDepthMarketData->PreOpenInterest,
               pDepthMarketData->OpenPrice,pDepthMarketData->HighestPrice,pDepthMarketData->LowestPrice,
               pDepthMarketData->Volume,pDepthMarketData->Turnover,pDepthMarketData->OpenInterest,pDepthMarketData->ClosePrice,
               pDepthMarketData->SettlementPrice,pDepthMarketData->UpperLimitPrice,pDepthMarketData->LowerLimitPrice,pDepthMarketData->UpdateTime,
               pDepthMarketData->UpdateMillisec,pDepthMarketData->AveragePrice,pDepthMarketData->ActionDay
              );
}

void SimNowMdAPI::UserLogin(CThostFtdcReqUserLoginField* user_login) {
    int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
    md_api_->ReqUserLogin(user_login, request_id);
}

void SimNowMdAPI::CreateFtdcMdApi(std::string& path) {
    md_api_ = CThostFtdcMdApi::CreateFtdcMdApi(path.c_str());
    md_api_->RegisterSpi(this);
}

void SimNowMdAPI::RegisterFront(std::string& addresss) {
    md_api_->RegisterFront((char*)(addresss.c_str()));
}

void SimNowMdAPI::Init() {
    md_api_->Init();
}

void SimNowMdAPI::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
    LOG_DEBUG2("RequestID %d ErrorID: %d ErrorMsg: %s",nRequestID,
               pRspInfo->ErrorID,pRspInfo->ErrorMsg);
    std::string s_file = "1.txt";
    file::FilePath file_path(s_file);
    file::WriteFile(file_path, pRspInfo->ErrorMsg, strlen(pRspInfo->ErrorMsg));
}

void SimNowMdAPI::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, 
                                 CThostFtdcRspInfoField *pRspInfo, 
                                 int nRequestID, bool bIsLast) {
    LOG_DEBUG2("OnRspUserLogout nRequestID %d\n", nRequestID);
}

void SimNowMdAPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, 
                                CThostFtdcRspInfoField *pRspInfo, 
                                int nRequestID, bool bIsLast) {
    LOG_DEBUG2("OnRspUserLogin nRequestID %d\n", nRequestID);
    char*  pp_instrument_id[2] = {"RB1804","IF1505"};
    md_api_->SubscribeMarketData(pp_instrument_id,1);
}

void SimNowMdAPI::OnHeartBeatWarning(int nTimeLapse) {
    LOG_DEBUG2("last recv message %d\n", nTimeLapse);
}

void SimNowMdAPI::OnFrontConnected() {
    LOG_DEBUG("connect successs!\n");
    LOG_DEBUG2("%s",md_api_->GetTradingDay());
    std::string user_id = "096960";
    std::string password = "kerry0903";
    std::string broker_id = "9999";
    CThostFtdcReqUserLoginField req_user_login;
    memset(&req_user_login,0,sizeof(req_user_login));
    strncpy(req_user_login.UserID,user_id.c_str(),user_id.length());
    strncpy(req_user_login.Password, password.c_str(), password.length());
    strncpy(req_user_login.BrokerID, broker_id.c_str(), broker_id.length());
    UserLogin(&req_user_login);
}

void SimNowMdAPI::OnFrontDisconnected(int nReason) {
    switch (nReason) {
        case 0x1001: {
            LOG_DEBUG("网络读失败\n");
            break;
        }
        case 0x1002: {
            LOG_DEBUG("网络写失败\n");
            break;
        }
        case 0x2001: {
            LOG_DEBUG("接收心跳超时\n");
            break;
        }

        case 0x2002:{
            LOG_DEBUG("发送心跳失败\n");
            break;
        }

        case 0x2003:{
            LOG_DEBUG("收到错误报文\n");
            break;
        }

        default:{
            LOG_DEBUG("未知\n");
            break;
        }
    }
}


TEST(MdApiTest, Basic){
    SimNowMdAPI* sim_now = new SimNowMdAPI();
    std::string path = "";
    sim_now->CreateFtdcMdApi(path);
    std::string addresss = "tcp://180.168.146.187:10011";
    sim_now->RegisterFront(addresss);
    sim_now->Init();
   /* 
    std::string user_id = "096960";
    std::string password = "kerry0903";
    std::string broker_id = "9999";
    CThostFtdcReqUserLoginField req_user_login;
    memset(&req_user_login,0,sizeof(req_user_login));
    strncpy(req_user_login.UserID,user_id.c_str(),user_id.length());
    strncpy(req_user_login.Password, password.c_str(), password.length());
    strncpy(req_user_login.BrokerID, broker_id.c_str(), broker_id.length());
    sim_now->UserLogin(&req_user_login);*/
    sleep(100);
    if (sim_now) {delete sim_now; sim_now = NULL;}
}
