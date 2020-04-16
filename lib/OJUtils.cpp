/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include "OJUtils.h"

namespace OJUtils {
std::string bin_to_hex(const std::string bin) {
        std::string result;
        char tmpbyte[3];

        for(unsigned char c: bin) {
                sprintf(tmpbyte,"%02x",c);
                result += tmpbyte;
        }

        return result;
}
};
