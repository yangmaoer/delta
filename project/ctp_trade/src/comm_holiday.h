/*****************************************************************************
order_manager Module Copyright (c) 2015. All Rights Reserved.

FileName: common_deal.h
Version: 1.0
Date: 2016.03.26

History: cwm     2016.03.26   1.0     Create
******************************************************************************/

#ifndef TRUNK_MODULES_CTP_SOURCE_COMMON_HOLIDAY_H_
#define TRUNK_MODULES_CTP_SOURCE_COMMON_HOLIDAY_H_

#include "base/util.h"
#include "base/dictionary.h"
#include "base/trace.h"
#include "database/unidbpool.h"
#include "common.h"
#include <string.h>
#include <string>
#include <time.h>

using namespace std;

namespace ctp
{
class comm_holiday
{
public:
    comm_holiday();
    ~comm_holiday();

public:
    bool load_holiday(database::db_instance* dbconn, const std::string &table);
    bool get_latestholiday(time_t* latest_holiday);
    time_t get_next_work_day(time_t tdate);
    time_t get_last_work_day(time_t tdate);
    time_t add_work_days(time_t tdate,int ndays);
    bool isholiday(time_t tdate);
    /*白天盘取当天； 00:00之后 夜盘交易，当天应该按照前一天计算*/
    std::string get_trade_day();
    int get_today_second();

private:
    std::vector<std::string> holidaysvec_;  //节假日列表
};

}



#endif /* TRUNK_MODULES_CTP_SOURCE_COMMON_HOLIDAY_H_ */