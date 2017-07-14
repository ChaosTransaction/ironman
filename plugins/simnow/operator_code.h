#ifndef SIMNOW_OPERATOR_CODE_H_
#define SIMNOW_OPERATOR_CODE_H_

#pragma once

namespace simnow_logic {

#define MD_TYPE 10000
#define TRADER_TYPE 20000

#define MD_USER_CONNECTED        MD_TYPE + 1
#define MD_USER_LOGIN            MD_TYPE + 2
#define MD_MARKET_DATA           MD_TYPE + 3
#define MD_SCRIBE_MARKET_DATA    MD_TYPE + 4

#define TRADER_USER_LOGIN TRADER_TYPE + 1
#define QRY_INSTRUMENT   TRADER_TYPE + 2
} 

#endif
