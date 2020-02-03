/*********************************************************************
 * Copyright (C) 2020 Miguel Revilla Rodríguez
 *                    and the OJudge Platform project contributors
 *
 * This file is part of the OJudge Platform
 *
 * Read the LICENSE file for information on license terms
 *********************************************************************/

#ifndef DBMODEL_H
#define DBMODEL_H

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/WtSqlTraits.h>
#include <string>

using namespace Wt;

namespace dbo = Wt::Dbo;

class Category ;
class Problem ;
class Settings ;
class Submission ;
class Testcase ;
class Verdict ;

typedef dbo::collection< dbo::ptr<Category> > Categories ;
typedef dbo::collection< dbo::ptr<Problem> > Problems ;
typedef dbo::collection< dbo::ptr<Testcase> > Testcases ;
typedef dbo::collection< dbo::ptr<Submission> > Submissions ;
typedef dbo::collection< dbo::ptr<Verdict> > Verdicts ;

class Category {
public:
	std::string title ;
	int order ;
	
	dbo::ptr<Category> parent;
	dbo::collection< dbo::ptr<Category> > children;
	dbo::collection< dbo::ptr<Problem> > problems;

	template<class Action>
	void persist(Action& a) {
		dbo::field(a, title, "title") ;
		dbo::field(a, order, "order") ;
		dbo::belongsTo(a, parent, "parent") ;
		dbo::hasMany(a, children, dbo::ManyToOne, "parent") ;
		dbo::hasMany(a, problems, dbo::ManyToMany, "probs_cats") ;
	}
} ;

class Problem {
public:
	long long id ;
	std::string title ;

	dbo::collection< dbo::ptr<Category> > categories;
	dbo::collection< dbo::ptr<Testcase> > testcases;
	dbo::collection< dbo::ptr<Submission> > submissions;

	template<class Action>
	void persist(Action& a) {
		dbo::id(a, id, "id") ;
		dbo::field(a, title, "title");
		dbo::hasMany(a, categories, dbo::ManyToMany, "probs_cats");
		dbo::hasMany(a, testcases, dbo::ManyToOne, "problem");
		dbo::hasMany(a, submissions, dbo::ManyToOne, "problem");
	}
} ;

namespace Wt {
    namespace Dbo {
        template<>
        struct dbo_traits<Problem> : public dbo_default_traits {
            static const char *surrogateIdField() { return 0; }
        };
    }
}

class Testcase {
public:
	dbo::ptr<Problem> problem;
	std::string title;

	template<class Action>
	void persist(Action& a) {
		dbo::field(a, title, "title");
		dbo::belongsTo(a, problem, "problem", Dbo::NotNull) ;
	}
};

class Settings {
public:
	std::string siteTitle ;
	std::string siteLogo ;
	std::string siteColor ;
	std::string customCSS ;

	template<class Action>
	void persist(Action& a) {
		dbo::field(a, siteTitle, "sitetitle");
		dbo::field(a, siteLogo, "sitelogo");
		dbo::field(a, siteColor, "sitecolor");
		dbo::field(a, customCSS, "customcss");
	}
};

class Submission {
public:
	dbo::ptr<Problem> problem;
//	dbo::ptr<User> user;
	dbo::ptr<Testcase> testcase;
	WDateTime datetime;
	dbo::collection< dbo::ptr<Verdict> > verdicts;

	template<class Action>
	void persist(Action& a) {
		dbo::belongsTo(a, problem, "problem", Dbo::NotNull);
//		dbo::belongsTo(a, user, "user", Dbo::NotNull);a
		dbo::belongsTo(a, testcase, "testcase", Dbo::NotNull);
		dbo::field(a, datetime, "datetime");
		dbo::hasMany(a, verdicts, dbo::ManyToOne, "submission") ;
	}
};

class Verdict {
public:
	dbo::ptr<Submission> submission;
	WDateTime datetime;
	int cpuused;
	int memoryused;
	std::string reason;

	template<class Action>
	void persist(Action& a) {
		dbo::belongsTo(a, submission, "submission", Dbo::NotNull);
		dbo::field(a, datetime, "datetime");
		dbo::field(a, cpuused, "cpuused");
		dbo::field(a, memoryused, "memoryused");
		dbo::field(a, reason, "reason");
	}
};

class DBModel {
public:
	DBModel(const std::string &postgresDb);
        ~DBModel();

	dbo::Transaction startTransaction();
	dbo::ptr<Category> addCategory(std::string title, int parent);
	Categories getCategories();

private:
	dbo::Session session;
} ;	

#endif // DBMODEL_H
