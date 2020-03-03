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

class Category;
class Problem;
class Description;
class Setting;
class Submission;
class Testcase;
class Verdict;
class Language;
class Contest;

typedef dbo::collection< dbo::ptr<Category> > Categories;
typedef dbo::collection< dbo::ptr<Problem> > Problems;
typedef dbo::collection< dbo::ptr<Description> > Descriptions;
typedef dbo::collection< dbo::ptr<Testcase> > Testcases;
typedef dbo::collection< dbo::ptr<Submission> > Submissions;
typedef dbo::collection< dbo::ptr<Verdict> > Verdicts;
typedef dbo::collection< dbo::ptr<Setting> > Settings;
typedef dbo::collection< dbo::ptr<Language> > Languages;
typedef dbo::collection< dbo::ptr<Contest> > Contests;

namespace Wt {
namespace Dbo {
	template<>
	struct dbo_traits<Problem> : public dbo_default_traits {
		static const char *surrogateIdField() { return 0; }
	};


	template<>
	struct dbo_traits<Description> : public dbo_default_traits {
		typedef ptr<Problem> IdType;

		static IdType invalidId() { return ptr<Problem>{}; }
		static const char *surrogateIdField() { return 0; }
	};
}
}

class Category {
public:
std::string title;
int order;

dbo::ptr<Category> parent;
dbo::collection< dbo::ptr<Category> > children;
dbo::collection< dbo::ptr<Problem> > problems;

template<class Action>
void persist(Action& a) {
	dbo::field(a, title, "title");
	dbo::field(a, order, "order");
	dbo::belongsTo(a, parent, "parent");
	dbo::hasMany(a, children, dbo::ManyToOne, "parent");
	dbo::hasMany(a, problems, dbo::ManyToMany, "probs_cats");
}
};

class Problem {
public:
long long id;
std::string title;

dbo::collection< dbo::ptr<Category> > categories;
dbo::collection< dbo::ptr<Testcase> > testcases;
dbo::collection< dbo::ptr<Submission> > submissions;
dbo::weak_ptr<Description> description;
dbo::collection< dbo::ptr<Contest> > contests;

template<class Action>
void persist(Action& a) {
	dbo::id(a, id, "id");
	dbo::field(a, title, "title");
	dbo::hasOne(a, description);
	dbo::hasMany(a, categories, dbo::ManyToMany, "probs_cats");
	dbo::hasMany(a, testcases, dbo::ManyToOne, "problem");
	dbo::hasMany(a, submissions, dbo::ManyToOne, "problem");
	dbo::hasMany(a, contests, dbo::ManyToMany, "probs_contests");
}
};

class Description {
public:
std::vector<unsigned char> pdfdata;
dbo::ptr<Problem> problem;

template<class Action>
void persist(Action& a) {
	dbo::id(a, problem, "problem", dbo::NotNull|dbo::OnDeleteCascade);
	dbo::field(a, pdfdata, "pdfdata");
}
};

class Language {
public:
std::string name;
std::string compilerVersion;
std::optional< std::vector<unsigned char> > codeSkeleton;
std::optional< std::vector<unsigned char> > compileScript;
std::optional< std::vector<unsigned char> > linkScript;
std::vector<unsigned char> runScript;
dbo::collection< dbo::ptr<Submission> > submissions;
dbo::collection< dbo::ptr<Contest> > contests;

template<class Action>
void persist(Action& a) {
	dbo::field(a, name, "name");
	dbo::field(a, compilerVersion, "compilerVersion");
	dbo::field(a, codeSkeleton, "codeSkeleton");
	dbo::field(a, compileScript, "compileScript");
	dbo::field(a, linkScript, "linkScript");
	dbo::field(a, runScript, "runScript");
	dbo::hasMany(a, submissions, dbo::ManyToOne, "language");
	dbo::hasMany(a, contests, dbo::ManyToMany, "langs_contests");
}
};

class Contest {
public:
std::string title;
std::optional<std::string> subTitle;
std::optional< std::vector<unsigned char> > description;
std::optional< std::vector<unsigned char> > logo;
bool isVirtual;
WDateTime startTime;
WDateTime endTime;
dbo::collection< dbo::ptr<Problem> > problems;
dbo::collection< dbo::ptr<Language> > languages;
dbo::collection< dbo::ptr<Submission> > submissions;

template<class Action>
void persist(Action& a) {
	dbo::field(a, title, "title");
	dbo::field(a, subTitle, "subtitle");
	dbo::field(a, description, "description");
	dbo::field(a, logo, "logo");
	dbo::field(a, isVirtual, "isvirtual");
	dbo::field(a, startTime, "starttime");
	dbo::field(a, endTime, "endtime");
	dbo::hasMany(a, problems, dbo::ManyToMany, "probs_contests");
	dbo::hasMany(a, languages, dbo::ManyToMany, "langs_contests");
	dbo::hasMany(a, submissions, dbo::ManyToOne, "contest");
}
};

class Testcase {
public:
std::string title;
dbo::ptr<Problem> problem;

template<class Action>
void persist(Action& a) {
	dbo::field(a, title, "title");
	dbo::belongsTo(a, problem, "problem", Dbo::NotNull);
}
};

class Setting {
public:
std::string settingName;
std::string settingValue;

template<class Action>
void persist(Action& a) {
	dbo::field(a, settingName, "settingname");
	dbo::field(a, settingValue, "settingvalue");
}
};

class Submission {
public:
dbo::ptr<Problem> problem;
//	dbo::ptr<User> user;
dbo::ptr<Testcase> testcase;
dbo::ptr<Contest> contest;
WDateTime datetime;
dbo::ptr<Language> language;
dbo::collection< dbo::ptr<Verdict> > verdicts;

template<class Action>
void persist(Action& a) {
	dbo::belongsTo(a, problem, "problem", Dbo::NotNull);
//		dbo::belongsTo(a, user, "user", Dbo::NotNull);
	dbo::belongsTo(a, testcase, "testcase", Dbo::NotNull);
	dbo::belongsTo(a, contest, "contest");
	dbo::field(a, datetime, "datetime");
	dbo::belongsTo(a, language, "language", Dbo::NotNull);
	dbo::hasMany(a, verdicts, dbo::ManyToOne, "submission");
}
};

class Verdict {
public:
dbo::ptr<Submission> submission;
WDateTime datetime;
int cpuused;
int memoryused;
std::optional<std::string> reason;

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

Categories getCategories();
dbo::ptr<Category> addCategory(std::string title, int parent);

Problems getProblems();
dbo::ptr<Problem> getProblem(long long id);
dbo::ptr<Problem> addProblem(long long id, std::string title);
void updateDescription(long long problemId, std::vector<unsigned char>& descData);

Settings getSettings();
std::string getSetting(std::string settingName);

Languages getLanguages();

private:
dbo::Session session;
};

#endif // DBMODEL_H
