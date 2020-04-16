/*********************************************************************
* Copyright (C) 2020 Miguel Revilla Rodríguez
*                    and the OJudge Platform project contributors
*
* This file is part of the OJudge Platform
*
* Read the LICENSE file for information on license terms
*********************************************************************/

#ifndef OJAUTHMODEL_H
#define OJAUTHMODEL_H

#include <Wt/Auth/AuthModel.h>
#include <Wt/Auth/AbstractUserDatabase.h>

class OJAuthModel : public Wt::Auth::AuthModel {
public:
OJAuthModel(const Wt::Auth::AuthService& baseAuth, Wt::Auth::AbstractUserDatabase& users);
bool login(Wt::Auth::Login& login) override;
bool loginUser(Wt::Auth::Login& login, Wt::Auth::User& user, Wt::Auth::LoginState state = Wt::Auth::LoginState::Strong) override;
};

#endif // OJAUTHMODEL_H
