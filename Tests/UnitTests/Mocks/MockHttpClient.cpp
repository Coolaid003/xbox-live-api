// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include "MockHttpClient.h"
#include "StockMocks.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_BEGIN

MockHttpClient::MockHttpClient() :
    ResultHR(S_OK)
{
    reinit();
}

void MockHttpClient::reinit()
{
    ResultValue = web::http::http_response(200);
    ResultValue.set_body(
        web::json::value::parse(L"{}")
        );
}


pplx::task<web::http::http_response> 
MockHttpClient::get_request(
    _In_ web::http::http_request request,
    _In_ pplx::cancellation_token token
    )
{
    HRESULT hr = ResultHR;
    return pplx::create_task([this, hr]() -> web::http::http_response
    {
        if (FAILED(hr))
        {
            throw hr;
        }

        return ResultValue;
    });
}

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_END

