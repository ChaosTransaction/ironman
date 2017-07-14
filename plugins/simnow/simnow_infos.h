//  Copyright (c) 2017-2018 The quotations Authors. All rights reserved.
//  Created on: 2017年7月13日 Author: kerry

#ifndef SIMNOW_SIMNOW_INFOS_H_
#define SIMNOW_SIMNOW_INFOS_H_

namespace simnow_logic {

struct MDUserInfo {
  std::string user_id_;
  std::string password_;
  std::string broker_id_;
  std::string address_;
  std::string path_;
};

class SimNowConfig{
public:
  SimNowConfig();
  SimNowConfig(const SimNowConfig& config);

  SimNowConfig& operator = (const SimNowConfig& config);

  ~SimNowConfig(){
    if (data_ != NULL)
      data_->Release();
  }

  void set_md_user(const std::string& user_id, const std::string& password,
                    const std::string& broker_id, const std::string& address,
                  const std::string& path) {
        data_->md_userinfo_.user_id_ = user_id;
        data_->md_userinfo_.password_ = password;
        data_->md_userinfo_.broker_id_ = broker_id;
        data_->md_userinfo_.address_ = address;
        data_->md_userinfo_.path = path;
  }

  std::string& md_user_id() const {
    return data_->md_userinfo_.user_id_;
  }

  std::string& md_password() const {
    return data_->md_userinfo_.password_;
  }

  std::string& md_broker_id() const {
    return data_->md_userinfo_.broker_id_;
  }

  std::string& md_address() const {
    return data_->md_userinfo_.address_;
  }

  std::string& md_path() const {
    return data_->md_userinfo_.path_;
  }

private:
  class Data {
  public:
    Data()
      : refcount_(1) {}
    struct MDUserInfo md_userinfo_;
    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }

    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_) delete this;
    }

  private:
    int refcount_;
  };
  Data* data_;
};
}
#endif
