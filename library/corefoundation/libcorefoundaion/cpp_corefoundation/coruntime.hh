#pragma once

#include <cstdint>

typedef int64_t coGUID;

#define _coGUIDChar(n) ((coGUID)('a' <= n && n <= 'z' ? n - 'a' + 1 : 0))

#define coGUIDMake(_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_10,_11,_12) (\
/**/    (_coGUIDChar(*(""#_00))) +\
/**/    (_coGUIDChar(*(""#_01))) *27 +\
/**/    (_coGUIDChar(*(""#_02))) *27*27 +\
/**/    (_coGUIDChar(*(""#_03))) *27*27*27 +\
/**/    (_coGUIDChar(*(""#_04))) *27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_05))) *27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_06))) *27*27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_07))) *27*27*27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_08))) *27*27*27*27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_09))) *27*27*27*27*27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_10))) *27*27*27*27*27*27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_11))) *27*27*27*27*27*27*27*27*27*27*27 +\
/**/    (_coGUIDChar(*(""#_12))) *27*27*27*27*27*27*27*27*27*27*27*27)

struct coLanguagePort {
    
    virtual int64_t createComponent(coGUID objectGUID) = 0;
    virtual void retaionComponent(int64_t objectIndex) = 0;
    virtual void releaseComponent(int64_t objectIndex) = 0;
    
    virtual int64_t callComponent
    /*---*/(coGUID objectGUID,
            coGUID methodGUID,
            int64_t objectIndex,
            int64_t param0,
            int64_t param1,
            int64_t param2,
            int64_t param3) = 0;
};
