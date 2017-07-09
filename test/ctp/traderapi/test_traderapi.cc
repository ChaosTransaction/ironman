//  Copyright (c) 2015-2016 The kid Authors. All rights reserved.
//   Created on: 2016.12.23 Author: kerry

#include "gtest/gtest.h"
#include "file/file_path.h"
#include "file/file_util.h"
#include "basic/radom_in.h"
#include "basic/basic_util.h"
#include "logic/logic_comm.h"
#include "basic/basictypes.h"
#include "log/mig_log.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"
#include "http/http_header.h"
#include <string>
#include "api/simnow/ThostFtdcTraderApi.h"
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
class SimNowTraderAPI : public CThostFtdcTraderSpi{
public:
    SimNowTraderAPI();
    ~SimNowTraderAPI();
    void CreateFtdcTraderApi(const std::string& path);
    void RegisterFront(const std::string& address);
    void Init();
    void RegisterSpi(CThostFtdcTraderSpi *pSpi);
    void UserLogin(CThostFtdcReqUserLoginField* user_login);
    void ReqQryExchange(const std::string& exchange_id);
    void ReqQryTradingAccount();
    int ReqQryInstrument();
    void SubscribePrivateTopic(int type);
    void SubscribePublicTopic(int type);
public:
    void OnFrontConnected();
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, 
                   CThostFtdcRspInfoField *pRspInfo, 
                   int nRequestID, bool bIsLast);
    void OnRspQryExchange(CThostFtdcExchangeField *pExchange, 
                          CThostFtdcRspInfoField *pRspInfo, 
                          int nRequestID, bool bIsLast);

    void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, 
                                CThostFtdcRspInfoField *pRspInfo, 
                                int nRequestID, bool bIsLast);

    void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, 
                            int nRequestID, bool bIsLast);

public:
    CThostFtdcTraderApi*  trader_api_;
};

SimNowTraderAPI::SimNowTraderAPI() {

}

SimNowTraderAPI::~SimNowTraderAPI() {
    trader_api_->RegisterSpi(NULL);
    trader_api_->Release();
    trader_api_ = NULL;
}

void SimNowTraderAPI::SubscribePrivateTopic(int type) {
    THOST_TE_RESUME_TYPE resume_type;
    switch (type) {
        case 0: {
            resume_type = THOST_TERT_RESTART;
            break;
        }
        case 1: {
            resume_type = THOST_TERT_RESUME;
            break;
        }

        case 2: {
            resume_type = THOST_TERT_QUICK;
            break;
        }
    }

    trader_api_->SubscribePrivateTopic(resume_type);
}

void SimNowTraderAPI::SubscribePublicTopic(int type) {
    THOST_TE_RESUME_TYPE resume_type;
    switch (type) {
        case 0: {
            resume_type = THOST_TERT_RESTART;
            break;
        }
        case 1: {
            resume_type = THOST_TERT_RESUME;
            break;
        }

        case 2: {
            resume_type = THOST_TERT_QUICK;
            break;
        }
    }

    trader_api_->SubscribePublicTopic(resume_type);
}

void SimNowTraderAPI::RegisterSpi(CThostFtdcTraderSpi *pSpi) {
    trader_api_->RegisterSpi(pSpi);
}

void SimNowTraderAPI::CreateFtdcTraderApi(const std::string& path) {
    trader_api_ = CThostFtdcTraderApi::CreateFtdcTraderApi(path.c_str());
    trader_api_->RegisterSpi(this);
}

void SimNowTraderAPI::UserLogin(CThostFtdcReqUserLoginField* user_login) {
    int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
    trader_api_->ReqUserLogin(user_login, request_id);
}

void SimNowTraderAPI::Init() {
    trader_api_->Init();
}

void SimNowTraderAPI::RegisterFront(const std::string& address) {
    trader_api_->RegisterFront((char*)address.c_str());
}


void SimNowTraderAPI::OnFrontConnected() {
    LOG_DEBUG("connect successs!\n");
    LOG_DEBUG2("%s",trader_api_->GetTradingDay());
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


void SimNowTraderAPI::ReqQryExchange(const std::string& exchange_id){
    struct CThostFtdcQryExchangeField qry_exchange;
    int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
    memset(&qry_exchange,0,sizeof(qry_exchange));
    //strncpy(qry_exchange.ExchangeID,exchange_id.c_str(),exchange_id.length());
    trader_api_->ReqQryExchange(&qry_exchange,request_id);
}

void SimNowTraderAPI::ReqQryTradingAccount() {
    std::string broker_id = "9999";
    std::string investor_id = "096960";
    struct CThostFtdcQryTradingAccountField qry_trading_account;
    int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
    memset(&qry_trading_account,0,sizeof(qry_trading_account));
    strncpy(qry_trading_account.BrokerID, broker_id.c_str(), broker_id.length());
    strncpy(qry_trading_account.InvestorID, investor_id.c_str(), investor_id.length());
    trader_api_->ReqQryTradingAccount(&qry_trading_account,request_id);
}

int SimNowTraderAPI::ReqQryInstrument() {
    struct CThostFtdcQryInstrumentField qry_instrument;
    int request_id = base::SysRadom::GetInstance()->GetRandomIntID();
    memset(&qry_instrument, 0, sizeof(qry_instrument));
    return trader_api_->ReqQryInstrument(&qry_instrument,request_id);
}

void SimNowTraderAPI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, 
                                CThostFtdcRspInfoField *pRspInfo, 
                                int nRequestID, bool bIsLast) {
    LOG_DEBUG2("OnRspUserLogin nRequestID %d\n", nRequestID);
    std::string exchange_id = "SHFE";
    //ReqQryExchange(exchange_id);
    //ReqQryTradingAccount();
    ReqQryInstrument();
}

void SimNowTraderAPI::OnRspQryExchange(CThostFtdcExchangeField *pExchange,
                                       CThostFtdcRspInfoField *pRspInfo, 
                                       int nRequestID, bool bIsLast) {
    LOG_DEBUG2("ExchangeID %s ExchangeName %s TThostFtdcExchangePropertyType %s",
               pExchange->ExchangeID,pExchange->ExchangeName,
              pExchange->ExchangeProperty);                                       
}


void SimNowTraderAPI::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, 
                                CThostFtdcRspInfoField *pRspInfo, 
                                int nRequestID, bool bIsLast) {
    LOG_DEBUG2("TradingAccount nRequestID %d", nRequestID);
}

void SimNowTraderAPI::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, 
                                         CThostFtdcRspInfoField *pRspInfo, 
                                         int nRequestID, bool bIsLast) {
    
    char* input = pInstrument->InstrumentName;
    char* output = NULL;
    size_t outlen = 0;
    base::BasicUtil::CharracterSetConv("UTF-8", input, strlen(input),
                                       "gbk", &output, &outlen);
    LOG_DEBUG2("QryInstrument nRequestID:%d InstrumentID:%s ExchangeID:%s InstrumentName:%s  ExchangeInstID:%s ProductID:%s", 
               nRequestID,pInstrument->InstrumentID,pInstrument->ExchangeID,
                output,pInstrument->ExchangeInstID,
                pInstrument->ProductID);
}

TEST(TraderApiTest, Basic){
    SimNowTraderAPI* sim_now = new SimNowTraderAPI();
    std::string path = "";
    sim_now->CreateFtdcTraderApi(path);
    sim_now->SubscribePrivateTopic(1);
    sim_now->SubscribePublicTopic(1);
    std::string adress = "tcp://180.168.146.187:10001";
    sim_now->RegisterFront(adress);
    sim_now->Init();
    sleep(10); 
    if (sim_now) {delete sim_now; sim_now = NULL;}
}
