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
#include "xsapi/tournaments.h"
#include "Tournament_WinRT.h"
#include "TournamentRequest_WinRT.h"
#include "TournamentRequestResult_WinRT.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_TOURNAMENTS_BEGIN

/// <summary>
/// Represents an endpoint that you can use to access the Tournament service.
/// </summary>
public ref class TournamentService sealed
{
public:
    /// <summary>
    /// Returns a list of tournaments for the current title.
    /// </summary>
    /// <param name="request">A tournament request object that retrieves tournaments based on the configuration of the request.</param>
    /// <returns>A list of tournaments if they exist.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// This method calls GET /tournaments
    /// </remarks>
    Windows::Foundation::IAsyncOperation<TournamentRequestResult^>^  GetTournamentsAsync(
        _In_ TournamentRequest^ request
        );

    /// <summary>
    /// Returns a specific tournament object.
    /// The tournament ID and the organizer ID together uniquely identify a tournament.
    /// </summary>
    /// <param name="organizerId">The ID of the tournament organizer.</param>
    /// <param name="tournamentId">The ID of the tournament.</param>
    /// <returns>The specific tournament if they exist.</returns>
    /// <remarks>
    /// Returns a concurrency::task<T> object that represents the state of the asynchronous operation.
    /// This method calls GET /tournaments/{organizer}/{id}.
    /// </remarks>
    Windows::Foundation::IAsyncOperation<Tournament^>^ GetTournamentDetailsAsync(
        _In_ Platform::String^ organizerId,
        _In_ Platform::String^ tournamentId
        );

internal:
    TournamentService(
        _In_ xbox::services::tournaments::tournament_service cppObj
        );

    const xbox::services::tournaments::tournament_service& GetCppObj() const;
private:
    xbox::services::tournaments::tournament_service m_cppObj;
};

NAMESPACE_MICROSOFT_XBOX_SERVICES_TOURNAMENTS_END
