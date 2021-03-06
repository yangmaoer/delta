/*****************************************************************************
 data_query_manager Copyright (c) 2016. All Rights Reserved.

 FileName: http_deal.cpp
 Version: 1.0
 Date: 2016.11.1

 History:
 sky     2016.11.1   1.0     Create
 ******************************************************************************/

#include "http_deal.h"
#include "product_business_deal.h"
#include "base/dtrans.h"
#include "base/trace.h"
#include <iostream>
#include "libiconv/iconv.h"

namespace serverframe
{
    std::string gbk_to_utf8(const char* text)
    {
        iconv_t ih = iconv_open("utf-8", "gbk");

        size_t in_length = strlen(text);
        size_t out_length = 2 * in_length;
        char* in_buffer = new char[in_length + 1];
        char* out_buffer = new char[out_length + 1];
        char* in_buffer2 = in_buffer;
        char* out_buffer2 = out_buffer;
        out_buffer[0] = 0x00;

        strcpy(in_buffer, text);
        iconv(ih, &in_buffer, &in_length, &out_buffer, &out_length);
        *out_buffer = 0x00;
        std::string ret = out_buffer2;
        std::cout << ret.length() << std::endl;
        delete[]in_buffer2;
        delete[]out_buffer2;

        iconv_close(ih);

        return ret;
    }

extern bool g_is_service_available;

inline void send_error_response(struct evbuffer *evb, const char* err_msg) {
    std::string json = "{\"result\":\"N\",\"msg\":\"";
    json += err_msg;
    json += "\"}";
    evbuffer_add_printf(evb, "%s", json.c_str());
    TRACE_ERROR(MODULE_NAME, MANAGER_E_SERVER_NOTIFY_DATA_INVALID,
        "err_msg :%s\n", err_msg);

}

inline void send_success_response(struct evbuffer *evb, base::dictionary &dict) {
    base::dictionary result_dict;
    result_dict.add_object("result", "Y");
    result_dict.add_object("msg", dict);
    evbuffer_add_printf(evb, "%s", base::djson::dict2str(result_dict).c_str());
}

#define RESPONSE_ERROR(msg) \
    send_error_response(evb, msg);

#define RESPONSE_SUCCESS(dict) \
    send_success_response(evb, dict);

http_deal::http_deal()
{
    // TODO Auto-generated constructor stub
    m_nresult = 0;
    m_result_msg = "";
}

http_deal::~http_deal()
{
    // TODO Auto-generated destructor stub
}

int http_deal::service_onoff(evkeyvalq *output_Headers, evkeyvalq &http_query, evbuffer *evb)
{
    const char *input_name = evhttp_find_header (&http_query, "server_switch");

    if (input_name == NULL ) {
        RESPONSE_ERROR(errno_transfer(ERROR_FIELD_SWITCH));
        return MANAGER_E_FAILED;
    }

    std::string str_switch = input_name;
    std::string strrsp = gbk_to_utf8("服务已置成可用状态");
    if ("off" == str_switch) {
        g_is_service_available = false;
        strrsp = gbk_to_utf8("服务已置成不可用状态");
    }
    else {
        g_is_service_available = true;
    }

    base::dictionary outdict;
    outdict["info"] = strrsp;
    RESPONSE_SUCCESS(outdict);

    return NAUT_S_OK;
}

//查询
int http_deal::sql_query(evkeyvalq *output_Headers, base::dictionary *ptr_dict, evbuffer *evb)
{
    base::dictionary &dict = *ptr_dict;
    if (dict["temp"].string_value().length() < MIN_COUNT_SQL)
        RESPONSE_ERROR(errno_transfer(MANAGER_E_FAILED));

    base::dictionary indict, outdict;

    COPY_FIELD_STRING("temp");

    int ret = NAUT_S_OK;

    product_business_deal busi_deal;
    ret = busi_deal.sql_query(indict, outdict);

    RESPONSE_SUCCESS(outdict);

    return NAUT_S_OK;
}

int http_deal::account_fund_query(evkeyvalq *output_Headers, base::dictionary* ptr_dict, evbuffer *evb)
{
    base::dictionary &dict = *ptr_dict;
    base::dictionary indict, outdict;

    COPY_FIELD_STRING("productid");
    COPY_FIELD_STRING("productclass");

    int ret = NAUT_S_OK;

    product_business_deal busi_deal;
    ret = busi_deal.account_fund_query(indict, outdict);

    std::cout << base::djson::dict2str(outdict).c_str() << std::endl;
    RESPONSE_SUCCESS(outdict);

    return NAUT_S_OK;
}

int http_deal::trade_system_config_insert(evkeyvalq *output_Headers, base::dictionary *ptr_dict, evbuffer *evb)
{
    base::dictionary &dict = *ptr_dict;
    base::dictionary outdict;

    int ret = NAUT_S_OK;

    product_business_deal busi_deal;
    ret = busi_deal.trade_system_config_insert(dict, outdict);

    RESPONSE_SUCCESS(outdict);

    return NAUT_S_OK;
}

int http_deal::trade_system_config_update(evkeyvalq *output_Headers, base::dictionary *ptr_dict, evbuffer *evb)
{
    base::dictionary &dict = *ptr_dict;
    base::dictionary outdict;

    int ret = NAUT_S_OK;

    product_business_deal busi_deal;
    ret = busi_deal.trade_system_config_update(dict, outdict);

    RESPONSE_SUCCESS(outdict);

    return NAUT_S_OK;
}

int http_deal::trade_system_config_delete(evkeyvalq *output_Headers, base::dictionary *ptr_dict, evbuffer *evb)
{
    base::dictionary &dict = *ptr_dict;
    base::dictionary outdict;

    int ret = NAUT_S_OK;

    product_business_deal busi_deal;
    ret = busi_deal.trade_system_config_delete(dict, outdict);

    RESPONSE_SUCCESS(outdict);

    return NAUT_S_OK;
}

int http_deal::result()
{
    return m_nresult;
}

std::string http_deal::result_msg()
{
    return m_result_msg;
}

}

