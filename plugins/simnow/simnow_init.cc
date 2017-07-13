//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "simnow/simnow_init.h"
#include "simnow/simnow_logic.h"
#include "core/common.h"
#include "core/plugins.h"

struct simnowplugin {
    char *id;
    char *name;
    char *version;
    char *provider;
};

static void *OnSimNowStart(struct server *srv) {
    signal(SIGPIPE, SIG_IGN);
    struct simnowplugin *simnow =
        (struct simnowplugin *)calloc(1, sizeof(struct simnowplugin));
    simnow->id = "simnow";
    simnow->name = "simnow";
    simnow->version = "1.0.0";
    simnow->provider = "kerry";
    if (!simnow_logic::SimNowlogic::GetInstance())
          assert(0);
    simnow_logic::SimNowlogic::GetInstance()->InitSrv(srv);
    return simnow;
}

static handler_t OnSimNowShutdown(struct server *srv, void *pd) {
    simnow_logic::SimNowlogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnSimNowConnect(struct server *srv, int fd, void *data,
                                 int len) {
    simnow_logic::SimNowlogic::GetInstance()->OnSimNowConnect(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnSimNowMessage(struct server *srv, int fd, void *data,
                                 int len) {
    bool r = simnow_logic::SimNowlogic::GetInstance()->OnSimNowMessage(srv, fd, data,
             len);
    if (r)
        return HANDLER_GO_ON;
    else
        return HANDLER_FINISHED;
}

static handler_t OnSimNowClose(struct server *srv, int fd) {
    simnow_logic::SimNowlogic::GetInstance()->OnSimNowClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
    simnow_logic::SimNowlogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
    simnow_logic::SimNowlogic::GetInstance()->OnBroadcastClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
    simnow_logic::SimNowlogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
    simnow_logic::SimNowlogic::GetInstance()->OnIniTimer(srv);
    return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
    simnow_logic::SimNowlogic::GetInstance()->OnTimeout(srv, id, opcode, time);
    return HANDLER_GO_ON;
}

int simnow_plugin_init(struct plugin *pl) {
    pl->init = OnSimNowStart;
    pl->clean_up = OnSimNowShutdown;
    pl->connection = OnSimNowConnect;
    pl->connection_close = OnSimNowClose;
    pl->connection_close_srv = OnBroadcastClose;
    pl->connection_srv = OnBroadcastConnect;
    pl->handler_init_time = OnIniTimer;
    pl->handler_read = OnSimNowMessage;
    pl->handler_read_srv = OnBroadcastMessage;
    pl->handler_read_other = OnUnknow;
    pl->time_msg = OnTimeOut;
    pl->data = NULL;
    return 0;
}
