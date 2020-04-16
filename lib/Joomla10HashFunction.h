/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef JOOMLA10HASHFUNCTION_H
#define JOOMLA10HASHFUNCTION_H

#include <Wt/Auth/HashFunction.h>

class Joomla10HashFunction : public Wt::Auth::HashFunction {
public:
virtual std::string name() const override;
virtual std::string compute(const std::string& msg, const std::string& salt) const override;
};

#endif // JOOMLA10HASHFUNCTION_H
