/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#include <Wt/Utils.h>
#include "Joomla10HashFunction.h"
#include "OJUtils.h"

using namespace Wt;

std::string Joomla10HashFunction::name() const {
	return "joomla10";
}

std::string Joomla10HashFunction::compute(const std::string& msg, const std::string& salt) const {

	return OJUtils::bin_to_hex(Utils::md5(msg + salt));
}
