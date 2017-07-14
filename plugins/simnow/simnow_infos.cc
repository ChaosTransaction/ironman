//  Copyright (c) 2017-2018 The quotations Authors. All rights reserved.
//  Created on: 2017年7月13日 Author: kerry

#include "simnow/simnow_info.h"
namespace simnow_logic {

SimNowConfig::SimNowConfig() {
  data_ = new Data();
}

SimNowConfig::SimNowConfig(const SimNowConfig& config)
    : data_(config.data_) {
    if (data_ != NULL) {
        data_->AddRef();
    }
}

SimNowConfig& SimNowConfig::operator =(const SimNowConfig& config) {
    if (config.data_ != NULL) {
        config.data_->AddRef();
    }

    if (data_ != NULL) {
        data_->Release();
    }

    data_ = config.data_;
    return (*this);
}


}
