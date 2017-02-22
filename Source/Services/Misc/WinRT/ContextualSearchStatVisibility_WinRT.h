﻿// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include "pch.h"
#include "xsapi/contextual_search_service.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_CONTEXTUAL_SEARCH_BEGIN

/// <summary>Enumerates the visibility of a contextual search statistic.</summary>
public enum class ContextualSearchStatVisibility
{
    /// <summary>Unknown visibility.</summary>
    Unknown = xbox::services::contextual_search::contextual_search_stat_visibility::unknown,

    /// <summary>This is important, as third party applications like Twitch will only be able to view OPEN stats.</summary>
    PublicVisibility = xbox::services::contextual_search::contextual_search_stat_visibility::public_visibility,

    /// <summary>If a stat is private, only the title itself, or a companion application that has special privileges, can access this information.</summary>
    PrivateVisibility = xbox::services::contextual_search::contextual_search_stat_visibility::private_visibility
};

NAMESPACE_MICROSOFT_XBOX_SERVICES_CONTEXTUAL_SEARCH_END
