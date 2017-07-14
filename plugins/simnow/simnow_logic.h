//  Copyright (c) 2017-2018 The IRNMAN Authors. All rights reserved.
//  Created on: 2017年07月13日 Author: kerry

#ifndef IRONMAN_SIMNOW_SIMNOW_LOGIC_
#define IRONMAN_SIMNOW_SIMNOW_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "simnow/interface_md.h"
#include "simnow/interface_trader.h"
#include "simnow/simnow_info"

namespace simnow_logic {

class SimNowlogic {
public:
    SimNowlogic();
    virtual ~SimNowlogic();

private:
    static SimNowlogic *instance_;

public:
    static SimNowlogic *GetInstance();
    static void FreeInstance();
public:

    void InitSrv(struct server* srv);

    bool OnSimNowConnect(struct server *srv, const int socket);

    bool OnSimNowMessage(struct server *srv, const int socket, const void *msg,
                         const int len);

    bool OnSimNowClose(struct server *srv, const int socket);

    bool OnBroadcastConnect(struct server *srv, const int socket,
                            const void *data, const int len);

    bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                            const int len);

    bool OnBroadcastClose(struct server *srv, const int socket);

    bool OnIniTimer(struct server *srv);

    bool OnTimeout(struct server *srv, char *id, int opcode, int time);

private:
    bool Init();
    bool ParserCustomXml(const std::string& file, SimNowConfig& config);
private:
    simnow_logic::SimNowMDAPI*  md_api_;
    simnow_logic::SimNowTraderAPI* trader_api_;
};
} // namespace simnow_logic

#endif
