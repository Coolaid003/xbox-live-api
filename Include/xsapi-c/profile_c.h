// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

/// <summary> 
/// Represents a user's Xbox Live profile.
/// </summary>
typedef struct XBL_XBOX_USER_PROFILE
{
    /// <summary>
    /// The user's display name to be used in application UI.  This value is privacy gated and could
    /// be a user's real name or their Gamertag.
    /// </summary>
    PCSTR appDisplayName;

    /// <summary>
    /// Encoded Uri for the user's display picture to be used in application UI.
    /// The Uri is a resizable Uri. It can be used to specify one of the following sizes and formats by appending &apos;&amp;format={format}&amp;w={width}&amp;h={height}:
    /// Format: png
    /// Width   Height
    /// 64      64
    /// 208     208
    /// 424     424
    /// </summary>
    PCSTR appDisplayPictureResizeUri;

    /// <summary>
    /// The user's display name to be used in game UI.  This value is privacy gated and could
    /// be a user's real name or their Gamertag.
    /// </summary>
    PCSTR gameDisplayName;

    /// <summary>
    /// Encoded Uri for the user's display picture to be used in games.
    /// The Uri is a resizable Uri. It can be used to specify one of the following sizes and formats by appending &apos;&amp;format={format}&amp;w={width}&amp;h={height}:
    /// Format: png
    /// Width   Height
    /// 64      64
    /// 208     208
    /// 424     424
    /// </summary>
    PCSTR gameDisplayPictureResizeUri;

    /// <summary>
    /// The user's Gamerscore.
    /// </summary>
    PCSTR gamerscore;

    /// <summary>
    /// The user's Gamertag.
    /// </summary>
    PCSTR gamertag;

    /// <summary>
    /// The user's Xbox user ID.
    /// </summary>
    PCSTR xboxUserId;
} XBL_XBOX_USER_PROFILE;

/// <summary>
/// Callback function for XblGetUserProfile. result and profile are valid only until the callback returns.
/// </summary>
typedef void(*XBL_GET_USER_PROFILE_COMPLETION_ROUTINE)(
    _In_ XBL_RESULT result,
    _In_ CONST XBL_XBOX_USER_PROFILE *profile,
    _In_opt_ void* context
    );

/// <summary>
/// Gets a user profile for a specific Xbox user.
/// </summary>
/// <param name="xboxLiveContext">An xbox live context handle created with XblXboxLiveContextCreateHandle.</param>
/// <param name="xboxUserId">The Xbox User ID of the user to get the profile for.</param>
/// <param name="queue">The async queue to associate this call with.</param>
/// <param name="callbackContext">Context passed back to callback.</param>
/// <param name="callback">A client callback function that will be called when the async operation is complete.</param>
/// <returns>
/// Result code for this API operation. Possible values are XBL_RESULT_OK and XBL_RESULT_E_INVALIDARG.
/// The result of the asynchronous operation is returned via the callback parameters.
/// </returns>
/// <remarks>Calls V2 GET /users/batch/profile/settings</remarks>
XBL_API XBL_RESULT XBL_CALLING_CONV
XblGetUserProfile(
    _In_ XBL_XBOX_LIVE_CONTEXT_HANDLE xboxLiveContext,
    _In_ PCSTR xboxUserId,
    _In_ XBL_ASYNC_QUEUE queue,
    _In_opt_ void* callbackContext,
    _In_ XBL_GET_USER_PROFILE_COMPLETION_ROUTINE callback
    ) XBL_NOEXCEPT;

/// <summary>
/// Callback function for XblGetUserProfile. result and profiles are valid only until the callback returns.
/// </summary>
typedef void(*XBL_GET_USER_PROFILES_COMPLETION_ROUTINE)(
    _In_ XBL_RESULT result,
    _In_ CONST XBL_XBOX_USER_PROFILE *profiles,
    _In_ uint32_t profilesCount,
    _In_opt_ void* context
    );

/// <summary>
/// Gets one or more user profiles for a collection of specified Xbox users.
/// </summary>
/// <param name="xboxLiveContext">An xbox live context handle created with XblXboxLiveContextCreateHandle.</param>
/// <param name="xboxUserIds">C-style Array of Xbox User IDs of the users to get profiles for.</param>
/// <param name="xboxUserIdsCount">The number of Xbox User IDs in the array.</param>
/// <param name="queue">The async queue to associate this call with.</param>
/// <param name="callbackContext">Context passed back to callback.</param>
/// <param name="callback">A client callback function that will be called when the async operation is complete.</param>
/// <returns>
/// <returns>
/// Result code for this API operation. Possible values are XBL_RESULT_OK and XBL_RESULT_E_INVALIDARG.
/// The result of the asynchronous operation is returned via the callback parameters.
/// </returns>
/// <remarks>Calls V2 GET /users/batch/profile/settings</remarks>
XBL_API XBL_RESULT XBL_CALLING_CONV
XblGetUserProfiles(
    _In_ XBL_XBOX_LIVE_CONTEXT_HANDLE xboxLiveContext,
    _In_ PCSTR *xboxUserIds,
    _In_ uint32_t xboxUserIdsCount,
    _In_ XBL_ASYNC_QUEUE queue,
    _In_opt_ void* callbackContext,
    _In_ XBL_GET_USER_PROFILES_COMPLETION_ROUTINE callback
    ) XBL_NOEXCEPT;

/// <summary>
/// Gets a user profile for a specific Xbox user.
/// </summary>
/// <param name="xboxLiveContext">An xbox live context handle created with XblXboxLiveContextCreateHandle.</param>
/// <param name="socialGroup">The name of the social group of users to search. Options are "Favorites" and "People".</param>
/// <param name="queue">The async queue to associate this call with.</param>
/// <param name="callbackContext">Context passed back to callback.</param>
/// <param name="callback">A client callback function that will be called when the async operation is complete.</param>
/// <returns>
/// Result code for this API operation. Possible values are XBL_RESULT_OK and XBL_RESULT_E_INVALIDARG.
/// The result of the asynchronous operation is returned via the callback parameters.
/// </returns>
/// <remarks>Calls V2 GET /users/{userId}/profile/settings/people/{socialGroup}</remarks>
XBL_API XBL_RESULT XBL_CALLING_CONV
XblGetUserProfilesForSocialGroup(
    _In_ XBL_XBOX_LIVE_CONTEXT_HANDLE xboxLiveContext,
    _In_ PCSTR socialGroup,
    _In_ XBL_ASYNC_QUEUE queue,
    _In_opt_ void* callbackContext,
    _In_ XBL_GET_USER_PROFILES_COMPLETION_ROUTINE callback
    ) XBL_NOEXCEPT;

#if defined(__cplusplus)
} // end extern "C"
#endif // defined(__cplusplus)