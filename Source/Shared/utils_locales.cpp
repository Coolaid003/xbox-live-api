﻿// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "pch.h"
#include <mutex>
#include "utils.h"
#if XSAPI_A
#include "a/java_interop.h"
#include "a/jni_utils.h"
#endif

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_BEGIN

static string_t s_locales = _T("en-US");

#if XSAPI_A
std::unordered_map<string_t, string_t> serviceLocales = 
{ 
    { "es_AR",    "es-AR" },
    { "AR",         "es-AR" },
    { "en_AU",     "en-AU" },
    { "AU",         "en-AU" },
    { "de_AT",     "de-AT" },
    { "AT",         "de-AT" },
    { "fr_BE",     "fr-BE" },
    { "nl_BE",     "nl-BE" },
    { "BE",         "fr-BE" },
    { "pt_BR",     "pt-BR" },
    { "BR",         "pt-BR" },
    { "en_CA",     "en-CA" },
    { "fr_CA",     "fr-CA" },
    { "CA",         "en-CA" },
    { "en_CZ",     "en-CZ" },
    { "CZ",         "en-CZ" },
    { "da_DK",     "da-DK" },
    { "DK",         "da-DK" },
    { "fi_FI",     "fi-FI" },
    { "FI",         "fi-FI" },
    { "fr_FR",     "fr-FR" },
    { "FR",         "fr-FR" },
    { "de_DE",    "de-DE" },
    { "DE",         "de-DE" },
    { "en_GR",     "en-GR" },
    { "GR",         "en-GR" },
    { "en_HK",     "en-HK" },
    { "zh_HK",     "zh-HK" },
    { "HK",         "en-HK" },
    { "en_HU",     "en-HU" },
    { "HU",         "en-HU" },
    { "en_IN",     "en-IN" },
    { "IN",         "en-IN" },
    { "en_GB",     "en-GB" },
    { "GB",         "en-GB" },
    { "en_IL",     "en-IL" },
    { "IL",         "en-IL" },
    { "it_IT",     "it-IT" },
    { "IT",         "it-IT" },
    { "ja_JP",     "ja-JP" },
    { "JP",         "ja-JP" },
    { "zh_CN",     "zh-CN" },
    { "CN",         "zh-CN" },
    { "es_MX",     "es-MX" },
    { "MX",         "es-MX" },
    { "es_CL",     "es-CL" },
    { "CL",         "es-CL" },
    { "es_CO",     "es-CO" },
    { "CO",         "es-CO" },
    { "nl_NL",     "nl-NL" },
    { "NL",         "nl-NL" },
    { "en_NZ",     "en-NZ" },
    { "NZ",         "en-NZ" },
    { "nb_NO",     "nb-NO" },
    { "NO",         "nb-NO" },
    { "pl_PL",     "pl-PL" },
    { "PL",         "pl-PL" },
    { "pt_PT",     "pt-PT" },
    { "PT",         "pt-PT" },
    { "ru_RU",    "ru-RU" },
    { "RU",        "ru-RU" },
    { "en_SA",     "en-SA" },
    { "SA",         "en-SA" },
    { "en_SG",     "en-SG" },
    { "zh_SG",     "zh-SG" },
    { "SG",         "en-SG" },
    { "en_SK",     "en-SK" },
    { "SK",         "en-SK" },
    { "en_ZA",     "en-ZA" },
    { "ZA",         "en-ZA" },
    { "ko_KR",     "ko-KR" },
    { "KR",         "ko-KR" },
    { "es_ES",     "es-ES" },
    { "es",         "es-ES" },
    { "de_CH",     "de-CH" },
    { "fr_CH",     "fr-CH" },
    { "CH",         "fr-CH" },
    { "zh_TW",     "zh-TW" },
    { "TW",         "zh-TW" },
    { "en_AE",     "en-AE" },
    { "AE",         "en-AE" },
    { "en_US",     "en-US" },
    { "US",         "en-US" },
    { "sv_SE",     "sv-SE" },
    { "SE",         "sv-SE" },
    { "tr_Tr",     "tr-TR" },
    { "Tr",         "tr-TR" },
    { "en_IE",   "en-IE" },
    { "IE",      "en-IE" },
    { "es_US",      "es-US" }
};
#endif

#ifdef _WIN32
// Locale api for desktop and xbox
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP | TV_API | XBOX_UWP

std::vector<string_t> utils::get_locale_list()
{
    std::vector<string_t> localeList;

    TCHAR localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };

    if (GetUserDefaultLocaleName(localeName, ARRAYSIZE(localeName)))
    {
        localeList.push_back(localeName);
    }
    else
    {
        localeList.push_back(_T("en-US"));
    }

    return localeList;
}

#else // #else WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP

std::vector<string_t> utils::get_locale_list()
{
    std::vector<string_t> localeList;

    try
    {
        auto resourceContext = Windows::ApplicationModel::Resources::Core::ResourceContext::GetForCurrentView();

        auto languages = resourceContext->Languages;
        for (auto language : languages)
        {
            localeList.push_back(language->Data());
        }
    }
    catch (...)
    {
        LOG_ERROR("Failed to get system locale, fall back to en-US");
        localeList.push_back(_T("en-US"));
    }

    return localeList;
}

#endif //WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP

#elif XSAPI_A // #else ifdef WIN32

std::vector<string_t> utils::get_locale_list()
{
    auto javaInterop = java_interop::get_java_interop_singleton();
    std::vector<string_t> localeList;
    auto javaVM = javaInterop->get_java_vm();
    if (javaVM == nullptr)
    {
        LOG_ERROR("java interop not initialized properly");
        return localeList;
    }
    auto marketActivityClass = javaInterop->get_market_activity_class();
    if (javaVM != nullptr && marketActivityClass != nullptr)
    {
        JNIEnv* jniEnv;
        JNI_ATTACH_THREAD(javaVM, jniEnv);
        jmethodID getLocaleMethod = jniEnv->GetStaticMethodID(marketActivityClass, "getLocale", "()Ljava/lang/String;");
        if (getLocaleMethod != nullptr)
        {
            jstring localeJString = (jstring)jniEnv->CallStaticObjectMethod(marketActivityClass, getLocaleMethod);
            string_t localeString = jniEnv->GetStringUTFChars(localeJString, nullptr);
            auto findResult = serviceLocales.find(localeString);
            if (findResult != serviceLocales.end())
            {
                localeList.push_back(findResult->second);
            }
            else
            {
                localeList.push_back(_T("en-US"));
            }
            return localeList;
        }
    }

    localeList.push_back(_T("en-US"));
    return localeList;
}

#endif // ifdef WIN32

void utils::generate_locales()
{
    std::vector<string_t> localeList = get_locale_list();
    std::vector<string_t> localeFallbackList;

    for (auto& locale : localeList)
    {
        // Build up fallback list, for instance, if the lang is "sd-Arab-PK"
        // We add "sd-Arab" and "sd" as well 
        // So that if an user's language preference is "fr-ml", "zh-hans", "en-us"
        // fallback chain is going to be:
        // fr-ml -> fr -> zh-hans -> zh -> en-us -> en
        localeFallbackList.push_back(locale);
        size_t nPos = locale.rfind(_T("-"));
        while (nPos != string_t::npos)
        {
            localeFallbackList.push_back(locale.substr(0, nPos));
            nPos = locale.rfind(_T("-"), nPos - 1);
        }
    }
    s_locales.clear();
    for (auto& locale : localeFallbackList)
    {
        s_locales += locale;
        s_locales += _T(',');
    }
    // erase the last ','
    s_locales.pop_back();
}

static std::mutex s_locale_lock;
static bool s_custom_locale_override = false;

const string_t& utils::get_locales()
{
    std::lock_guard<std::mutex> guard(s_locale_lock);
    if (s_custom_locale_override)
    {
        return s_locales;
    }
    // For WinRT app, locale can only be get from STA, thus we generate locale in UI dispacther assignment.
    // For desktop and xbox, we generate locale on the first call.
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP | TV_API | XBOX_UWP
    static bool localeGenerated = false;
    if (!localeGenerated)
    {
        generate_locales();
        localeGenerated = true;
    }
#endif

    return s_locales;
}


void utils::set_locales(
    _In_ const string_t& locale
    )
{
    s_locales = locale;
    s_custom_locale_override = true;
}

NAMESPACE_MICROSOFT_XBOX_SERVICES_CPP_END