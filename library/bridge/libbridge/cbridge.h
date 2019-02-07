#pragma once

#include "cbridgecore_p.h"
#include <string>

# ifdef ANDROID
#   include "candroidbridge_p.h"
# endif

template<class T> struct CQValue;

template<> struct CQValue<std::string> {
    static CQType create(const std::string &value) {
        return CQCreateString(value.c_str(), (int32_t)value.size());
    }
};