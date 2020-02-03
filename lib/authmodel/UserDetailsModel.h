/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef USERDETAILSMODEL_H
#define USERDETAILSMODEL_H

#include <Wt/WFormModel.h>

using namespace Wt;

class Session;

class UserDetailsModel : public WFormModel {
public:
	// static const Field FavouritePetField
	UserDetailsModel(Session &session);
	void save(const Auth::User &user);

private:
	Session &session_;
};

#endif // USERDETAILSMODEL_H
