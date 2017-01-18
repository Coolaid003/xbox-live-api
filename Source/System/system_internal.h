//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#pragma once

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_BEGIN

class xbox_live_mutex
{
public:
    xbox_live_mutex();
    xbox_live_mutex(_In_ const xbox_live_mutex& other);
    xbox_live_mutex operator=(_In_ const xbox_live_mutex& other);
    std::mutex& get();
private:
    std::mutex m_xboxLiveMutex;
};


NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_END