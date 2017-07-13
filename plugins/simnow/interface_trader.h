//  Copyright (c) 2017-2018 The SIMNOW Authors. All rights reserved.
//  Created on: 2017年7月14日 Author: kerry

#ifndef SIMNOW_INTERFACE_TRADER_H_
#define SIMNOW_INTERFACE_TRADER_H_
#include "api/simnow/ThostFtdcTraderApi.h"
#include "api/simnow/ThostFtdcUserApiStruct.h"
#include "api/simnow/ThostFtdcUserApiDataType.h"
#include <string>
#include "core/common.h"
namespace simnow_logic {

class SimNowTraderAPI : public CThostFtdcMdSpi {
public:
  SimNowTraderAPI() {}
  ~SimNowTraderAPI();
public:
  virtual void OnFrontConnected();

  virtual void OnFrontDisconnected(int reason);


  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *user_login,
                               CThostFtdcRspInfoField *rsp_info,
                               int request_id, bool is_last);

  virtual void OnRspQryInstrument(CThostFtdcInstrumentField *instrument, 
                                CThostFtdcRspInfoField *rsp_info,
                                int request_id, bool is_last);

public:
  void Init(struct server* srv);
  void Init(const std::string& path,
            const std::string& address);

  void UserLogin();

  void SetUserInfo(const std::string& broker_id, const std::string& investor_id,
                    const std::string& password);
private:
  void CreateFtdcTraderApi(const std::string& path);
  void RegisterFront(const std::string& addresss);
  void ReqQryInstrument();
  void SetTask(void* data, size_t data_length);
private:
  CThostFtdcTraderApi* trader_api_;
  struct server*  srv_;
  std::string broker_id_;
  std::string investor_id_;
  std::string password_;
};

}

#endif
