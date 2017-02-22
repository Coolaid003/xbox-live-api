// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include "UnitTestBase.h"
#include "DefineTestMacros.h"
#include "token_error.h"
#include "StockMocks.h"

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_BEGIN

#define TEST_CLASS_OWNER L"jameslao"

class TokenErrorResponseTests : public UnitTestBase
{
public:
    TEST_CLASS(TokenErrorResponseTests);
    DEFINE_TEST_CLASS_SETUP();

    DEFINE_TEST_CASE_WITH_DESC(CreateUserAccount, L"Deserialize XErrorResponse indicating user needs to go through account creation")
};

void TokenErrorResponseTests::CreateUserAccount()
{
    auto json = StockMocks::GetCreateAccountXErrorResponse();
    token_error resp = token_error::deserialize(json);

    VERIFY_ARE_EQUAL(L"0", resp.identity());
    VERIFY_ARE_EQUAL(L"https://start.ui.xboxlive.com/CreateAccount", resp.redirect());
    VERIFY_ARE_EQUAL(2148916233, resp.xerr());
}

NAMESPACE_MICROSOFT_XBOX_SERVICES_SYSTEM_CPP_END
