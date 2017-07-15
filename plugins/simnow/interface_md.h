//  Copyright (c) 2017-2018 The SIMNOW Authors. All rights reserved.
//  Created on: 2017年7月13日 Author: kerry

#ifndef SIMNOW_INTERFACE_MD_H_
#define SIMNOW_INTERFACE_MD_H_
#pragma once

#include "api/simnow/ThostFtdcMdApi.h"
#include "api/simnow/ThostFtdcUserApiStruct.h"
#include "api/simnow/ThostFtdcUserApiDataType.h"
#include <string>
#include "core/common.h"
#include "basic/basictypes.h"
#include "simnow/operator_code.h"

namespace simnow_logic {

class SimNowMDAPI : public CThostFtdcMdSpi {
public:
  SimNowMDAPI() {}
  ~SimNowMDAPI();
public:
  virtual void OnFrontConnected();

  virtual void OnFrontDisconnected(int reason);

  //virtual void OnHeartBeatWarning(int time_lapse);

  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *user_login,
                               CThostFtdcRspInfoField *rsp_info,
                               int request_id, bool is_last);


  /*virtual void OnRspUserLogout(CThostFtdcUserLogoutField *user_logout,
                              CThostFtdcRspInfoField *rsp_info,
                              int request_id, bool is_last);*/

  //virtual void OnRspError(CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last);

  virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *specific_instrument,
                            CThostFtdcRspInfoField *rsp_info, int request_id, bool is_last);

  virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *depth_marketdata);

public:
  void Init(struct server* srv);
  void Init(const std::string& path,
            const std::string& address);

  void UserLogin();

  void SetUserInfo(const std::string& user_id, const std::string& password,
                const std::string& broker_id);

  void SubScribeMarketData(char** instrument_id, int32 num);

private:
  void CreateFtdcMdApi(const std::string& path);
  void RegisterFront(const std::string& addresss);
  void SetTask(int16 code, int32 request_id, void* data, size_t data_length);
private:
  CThostFtdcMdApi* md_api_;
  struct server*  srv_;
  std::string user_id_;
  std::string password_;
  std::string broker_id_;
};

}

#endif
