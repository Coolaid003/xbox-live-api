///*********************************************************
///
/// Copyright (c) Microsoft. All rights reserved.
/// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
/// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
/// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
/// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
///
///*********************************************************
#include "pch.h"
#include "social_manager_internal.h"
#include "xsapi/presence.h"

using namespace xbox::services::presence;

NAMESPACE_MICROSOFT_XBOX_SERVICES_SOCIAL_MANAGER_CPP_BEGIN

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType,
    _In_ xsapi_internal_vector(xbox_social_user) usersAffected
    ) :
    m_socialEventType(eventType),
    m_usersAffected(std::move(usersAffected))
{
    for (auto& user : m_usersAffected)
    {
        m_usersAffectedAsStringVec.push_back(user.xbox_user_id());
    }
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType, 
    _In_ xsapi_internal_vector(social_manager_presence_record) presenceRecords
    ) :
    m_socialEventType(eventType),
    m_presenceRecords(std::move(presenceRecords))
{
    for (auto& record : m_presenceRecords)
    {
        stringstream_t str;
        str << record._Xbox_user_id();
        auto strStr = str.str();
        xsapi_internal_string xuid(strStr.c_str());
        m_usersAffectedAsStringVec.push_back(xuid);
    }
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType,
    _In_ device_presence_change_event_args devicePresenceArgs
    ) :
    m_socialEventType(eventType),
    m_devicePresenceArgs(std::move(devicePresenceArgs))
{
    xsapi_internal_string xuidStr(m_devicePresenceArgs.xbox_user_id().c_str());
    m_usersAffectedAsStringVec.push_back(xuidStr);
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType,
    _In_ xbox::services::presence::title_presence_change_event_args titlePresenceArgs
    ) :
    m_socialEventType(eventType),
    m_titlePresenceArgs(std::move(titlePresenceArgs))
{
    m_usersAffectedAsStringVec.push_back(m_titlePresenceArgs.xbox_user_id().c_str());
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType,
    _In_ xsapi_internal_vector(uint64_t) userList
    ) :
    m_socialEventType(eventType),
    m_userList(std::move(userList))
{
    for (auto& user : m_userList)
    {
        stringstream_t str;
        str << user;
        auto strStr = str.str();
        xsapi_internal_string xuid(strStr.c_str());
        m_usersAffectedAsStringVec.push_back(xuid);
    }
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType,
    _In_ xsapi_internal_vector(xsapi_internal_string) userAddList,
    _In_ pplx::task_completion_event<xbox_live_result<void>> tce
    ) :
    m_socialEventType(eventType),
    m_usersAffectedAsStringVec(std::move(userAddList)),
    m_tce(std::move(tce))
{
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type socialEventType,
    _In_ xbox_live_result<void> errorInfo,
    _In_ xsapi_internal_vector(xsapi_internal_string) userList
    ) :
    m_socialEventType(socialEventType),
    m_error(std::move(errorInfo)),
    m_usersAffectedAsStringVec(std::move(userList))
{
}

internal_social_event::internal_social_event(
    _In_ internal_social_event_type eventType,
    _In_ xsapi_internal_vector(xsapi_internal_string) userAddList
    ) :
    m_socialEventType(eventType),
    m_usersAffectedAsStringVec(std::move(userAddList))
{
}

internal_social_event_type
internal_social_event::event_type() const
{
    return m_socialEventType;
}

const xsapi_internal_vector(xbox_social_user)&
internal_social_event::users_affected() const
{
    return m_usersAffected;
}

const xsapi_internal_vector(uint64_t)&
internal_social_event::users_to_remove() const
{
    return m_userList;
}

const xsapi_internal_vector(social_manager_presence_record)&
internal_social_event::presence_records() const
{
    return m_presenceRecords;
}

const device_presence_change_event_args&
internal_social_event::device_presence_args() const
{
    return m_devicePresenceArgs;
}

const title_presence_change_event_args&
internal_social_event::title_presence_args() const
{
    return m_titlePresenceArgs;
}

const xsapi_internal_vector(xsapi_internal_string)&
internal_social_event::users_affected_as_string_vec() const
{
    return m_usersAffectedAsStringVec;
}

const pplx::task_completion_event<xbox_live_result<void>>&
internal_social_event::tce() const
{
    return m_tce;
}

const xbox_live_result<void>&
internal_social_event::error() const
{
    return m_error;
}

const call_buffer_timer_completion_context&
internal_social_event::completion_context() const
{
    return m_completionContext;
}

void
internal_social_event::set_completion_context(
    _In_ const call_buffer_timer_completion_context& compleitionContext
    )
{
    m_completionContext = compleitionContext;
}

NAMESPACE_MICROSOFT_XBOX_SERVICES_SOCIAL_MANAGER_CPP_END