/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef USER_H
#define USER_H

#include <Wt/Dbo/Types.h>
#include <Wt/WDate.h>
#include <Wt/WGlobal.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

namespace dbo = Wt::Dbo;

class User;
typedef Wt::Auth::Dbo::AuthInfo<User> AuthInfo;

enum class Role {
	Visitor = 0,
	Admin = 1
} ;

class User {
public:
  dbo::weak_ptr<AuthInfo> authInfo;
  Role role;

  template<class Action>
  void persist(Action& a)
  {
    dbo::field(a, role, "role");
    dbo::hasOne(a, authInfo, "user");
  }
};

DBO_EXTERN_TEMPLATES(User);

#endif // USER_H
