// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once 
#include "ServiceConfigurationStatistic_WinRT.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_USERSTATISTICS_BEGIN

/// <summary>
/// Represents the results of a user statistic query.
/// </summary>
public ref class UserStatisticsResult sealed
{
public:
    /// <summary>
    /// The Xbox User ID for the user in a statistic.
    /// </summary>
    DEFINE_PROP_GET_STR_OBJ(XboxUserId, xbox_user_id);

    /// <summary>
    /// A collection of statistics from a service configuration.
    /// </summary>        
    property Windows::Foundation::Collections::IVectorView<ServiceConfigurationStatistic^>^ ServiceConfigurationStatistics
    { 
        Windows::Foundation::Collections::IVectorView<ServiceConfigurationStatistic^>^ get(); 
    }

internal:
    UserStatisticsResult(
        _In_ xbox::services::user_statistics::user_statistics_result cppObj
    );

    property Platform::String^ InputServiceConfigurationId;

private:
    Windows::Foundation::Collections::IVector<ServiceConfigurationStatistic^>^ m_serviceConfigStatistics;
    xbox::services::user_statistics::user_statistics_result m_cppObj;
};

NAMESPACE_MICROSOFT_XBOX_SERVICES_USERSTATISTICS_END