﻿//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#pragma once
#include "xsapi/system.h"
#include "token_result.h"
#include "Ecdsa.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_BEGIN

class service_token_service
{
public:
    virtual pplx::task<xbox_live_result<token_result>>
    get_s_token_from_service(
        _In_ std::shared_ptr<ecdsa> proofKey,
        _In_ std::shared_ptr<auth_config> authenticationConfiguration,
        _In_ std::shared_ptr<xbox_live_context_settings> xboxLiveContextSettings
        ) = 0;
};

class service_token_service_impl : public service_token_service
{
public:
    virtual pplx::task<xbox_live_result<token_result>>
    get_s_token_from_service(
        _In_ std::shared_ptr<ecdsa> proofKey,
        _In_ std::shared_ptr<auth_config> authenticationConfiguration,
        _In_ std::shared_ptr<xbox_live_context_settings> xboxLiveContextSettings
        ) override;
};

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_END
