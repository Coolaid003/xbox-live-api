// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include "xsapi-c/profile_c.h"
#include "profile_internal.h"
#include "xbox_live_context_internal_c.h"
#include "xbox_live_context_impl.h"

using namespace xbox::services;
using namespace xbox::services::system;
using namespace xbox::services::social;

void copy_profile(
    _In_ std::shared_ptr<xbox_user_profile_internal> internal,
    _Out_ XblUserProfile* profile
    )
{
    strcpy_s(profile->appDisplayName, internal->app_display_name().data());
    utils::utf8_from_char_t(internal->app_display_picture_resize_uri().to_string().data(), profile->appDisplayPictureResizeUri, XBL_DISPLAY_PIC_URL_RAW_CHAR_SIZE);
    strcpy_s(profile->gameDisplayName, internal->game_display_name().data());
    utils::utf8_from_char_t(internal->game_display_picture_resize_uri().to_string().data(), profile->gameDisplayPictureResizeUri, XBL_DISPLAY_PIC_URL_RAW_CHAR_SIZE);
    strcpy_s(profile->gamerscore, internal->gamerscore().data());
    strcpy_s(profile->gamertag, internal->gamertag().data());
    strcpy_s(profile->xboxUserId, internal->xbox_user_id().data());
}

STDAPI
XblGetUserProfile(
    _In_ xbl_context_handle xboxLiveContext,
    _In_ UTF8CSTR xboxUserId,
    _In_ AsyncBlock* async
    ) XBL_NOEXCEPT
try
{
    RETURN_C_INVALIDARGUMENT_IF(xboxLiveContext == nullptr || xboxUserId == nullptr || async == nullptr);

    struct GetUserProfileContext
    {
        xbl_context_handle xboxLiveContext;
        xsapi_internal_string xboxUserId;
        xbox_live_result<std::shared_ptr<xbox_user_profile_internal>> result;
    };

    auto context = new (xsapi_memory::mem_alloc(sizeof(GetUserProfileContext))) GetUserProfileContext;
    context->xboxUserId = xboxUserId;
    context->xboxLiveContext = xboxLiveContext;

    auto hr = BeginAsync(async, context, nullptr, __FUNCTION__,
        [](AsyncOp op, const AsyncProviderData* data)
    {
        auto context = static_cast<GetUserProfileContext*>(data->context);

        switch (op)
        {
        case AsyncOp_DoWork:
            context->xboxLiveContext->contextImpl->profile_service_impl()->get_user_profile(
                context->xboxUserId,
                data->async->queue, // TODO should this be nested queue
                [data, context](xbox_live_result<std::shared_ptr<xbox_user_profile_internal>> result)
            {
                context->result = std::move(result);
                auto hr = utils::hresult_from_error_code(result.err());
                CompleteAsync(data->async, hr, sizeof(XblUserProfile));
            });
            return E_PENDING;

        case AsyncOp_GetResult:
            copy_profile(context->result.payload(), static_cast<XblUserProfile*>(data->context));
            break;

        case AsyncOp_Cleanup:
            delete data->context;
            break;
        }
        return S_OK;
    });

    if (FAILED(hr))
    {
        return hr;
    }
    return ScheduleAsync(async, 0);
}
CATCH_RETURN()

STDAPI
XblGetUserProfiles(
    _In_ xbl_context_handle xboxLiveContext,
    _In_ UTF8CSTR* xboxUserIds,
    _In_ size_t xboxUserIdsCount,
    _In_ AsyncBlock* async
    ) XBL_NOEXCEPT
try
{
    RETURN_C_INVALIDARGUMENT_IF(xboxLiveContext == nullptr || xboxUserIds == nullptr || async == nullptr);

    struct GetUserProfilesContext
    {
        xbl_context_handle xboxLiveContext;
        xsapi_internal_vector<xsapi_internal_string> xboxUserIds;
        xbox_live_result<xsapi_internal_vector<std::shared_ptr<xbox_user_profile_internal>>> result;
    };

    auto context = new (xsapi_memory::mem_alloc(sizeof(GetUserProfilesContext))) GetUserProfilesContext;
    context->xboxUserIds = utils::string_array_to_internal_string_vector(xboxUserIds, xboxUserIdsCount);
    context->xboxLiveContext = xboxLiveContext;

    auto hr = BeginAsync(async, context, nullptr, __FUNCTION__,
        [](AsyncOp op, const AsyncProviderData* data)
    {
        auto context = static_cast<GetUserProfilesContext*>(data->context);
        XblUserProfile* profile;

        switch (op)
        {
        case AsyncOp_DoWork:
            context->xboxLiveContext->contextImpl->profile_service_impl()->get_user_profiles(
                context->xboxUserIds,
                data->async->queue,
                [data, context](xbox_live_result<xsapi_internal_vector<std::shared_ptr<xbox_user_profile_internal>>> result)
            {
                context->result = std::move(result);
                auto hr = utils::hresult_from_error_code(result.err());
                CompleteAsync(data->async, hr, sizeof(XblUserProfile) * result.payload().size());
            });
            return E_PENDING;

        case AsyncOp_GetResult:
            profile = static_cast<XblUserProfile*>(data->context);
            for (const auto& internalProfile : context->result.payload())
            {
                copy_profile(internalProfile, profile++);
            }
            break;

        case AsyncOp_Cleanup:
            delete data->context;
            break;
        }
        return S_OK;
    });

    if (FAILED(hr))
    {
        return hr;
    }
    return ScheduleAsync(async, 0);
}
CATCH_RETURN()

STDAPI
XblGetUserProfilesForSocialGroup(
    _In_ xbl_context_handle xboxLiveContext,
    _In_ UTF8CSTR socialGroup,
    _In_ AsyncBlock* async
    ) XBL_NOEXCEPT
try
{
    RETURN_C_INVALIDARGUMENT_IF(xboxLiveContext == nullptr || socialGroup == nullptr || async == nullptr);

    struct GetUserProfilesContext
    {
        xbl_context_handle xboxLiveContext;
        xsapi_internal_string socialGroup;
        xbox_live_result<xsapi_internal_vector<std::shared_ptr<xbox_user_profile_internal>>> result;
    };

    auto context = new (xsapi_memory::mem_alloc(sizeof(GetUserProfilesContext))) GetUserProfilesContext;
    context->socialGroup = socialGroup;
    context->xboxLiveContext = xboxLiveContext;

    auto hr = BeginAsync(async, context, nullptr, __FUNCTION__,
        [](AsyncOp op, const AsyncProviderData* data)
    {
        auto context = static_cast<GetUserProfilesContext*>(data->context);
        XblUserProfile* profile;

        switch (op)
        {
        case AsyncOp_DoWork:
            context->xboxLiveContext->contextImpl->profile_service_impl()->get_user_profiles_for_social_group(
                context->socialGroup,
                data->async->queue,
                [data, context](xbox_live_result<xsapi_internal_vector<std::shared_ptr<xbox_user_profile_internal>>> result)
            {
                context->result = std::move(result);
                auto hr = utils::hresult_from_error_code(result.err());
                CompleteAsync(data->async, hr, sizeof(XblUserProfile) * result.payload().size());
            });
            return E_PENDING;

        case AsyncOp_GetResult:
            profile = static_cast<XblUserProfile*>(data->context);
            for (const auto& internalProfile : context->result.payload())
            {
                copy_profile(internalProfile, profile++);
            }
            break;

        case AsyncOp_Cleanup:
            delete data->context;
            break;
        }
        return S_OK;
    });

    if (FAILED(hr))
    {
        return hr;
    }
    return ScheduleAsync(async, 0);
}
CATCH_RETURN()

XBL_API HRESULT XBL_CALLING_CONV
XblGetProfileResultCount(
    _In_ AsyncBlock* async,
    _Out_ size_t* profileCount
    ) XBL_NOEXCEPT
{
    RETURN_C_INVALIDARGUMENT_IF(async == nullptr || profileCount == nullptr);
    
    size_t bufferSize;
    auto hr = GetAsyncResultSize(async, &bufferSize);

    if (SUCCEEDED(hr))
    {
        *profileCount = bufferSize / sizeof(XblUserProfile);
    }
    return hr;
}

XBL_API HRESULT XBL_CALLING_CONV
XblGetProfileResult(
    _In_ AsyncBlock* async,
    _In_ size_t profilesCount,
    _Out_writes_to_(profilesCount, written) XblUserProfile* profiles,
    _Out_opt_ size_t* written
    ) XBL_NOEXCEPT
{
    RETURN_C_INVALIDARGUMENT_IF_NULL(async);

    size_t actualProfilesCount;
    auto hr = XblGetProfileResultCount(async, &actualProfilesCount);
    RETURN_C_INVALIDARGUMENT_IF(actualProfilesCount > profilesCount);

    hr = GetAsyncResult(async, nullptr, profilesCount * sizeof(XblUserProfile), profiles, nullptr);
    if (SUCCEEDED(hr))
    {
        *written = actualProfilesCount;
    }
    return hr;
}