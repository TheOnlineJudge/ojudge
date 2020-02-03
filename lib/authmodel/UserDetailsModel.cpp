/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#include "UserDetailsModel.h"
#include "User.h"
#include "Session.h"

// const WFormModel::Field UserDetailsModel::FavouritePetField = "favourite-pet";

UserDetailsModel::UserDetailsModel(Session &session) : WFormModel(), session_(session) {
	// addField(FavouritePetField, WString::tr("favourite-pet-info"));
}

void UserDetailsModel::save(const Auth::User &authUser) {
	Dbo::ptr<User> user = session_.user(authUser) ;
	// uset.modify()->favouritePet = valueText(FavouritePetField).toUTF8();
}
