SET (ojudge_lib
authmodel/Session.cpp
authmodel/User.cpp
authmodel/UserDetailsModel.cpp
dbmodel/DBModel.cpp
viewmodels/CategoryModel.cpp
viewmodels/ViewModels.cpp
AboutWidget.cpp
AdminWidget.cpp
AdminCategoryWidget.cpp
AuthWidget.cpp
ContactWidget.cpp
ContestsWidget.cpp
ContributeWidget.cpp
DashboardWidget.cpp
FactsWidget.cpp
FooterWidget.cpp
HomeWidget.cpp
LanguagesWidget.cpp
LoginWidget.cpp
ProblemsWidget.cpp
ProfileWidget.cpp
RankingsWidget.cpp
RegistrationView.cpp
SponsorsWidget.cpp
SubmissionsWidget.cpp
TeamWidget.cpp
TutorialWidget.cpp
ojudgeApp.cpp
)

add_library (ojudge SHARED ${ojudge_lib})

set_target_properties (
        ojudge
        PROPERTIES
        SOVERSION ${OJUDGE_VERSION_MAJOR}
        VERSION ${OJUDGE_VERSION_MAJOR}.${OJUDGE_VERSION_MINOR}.${OJUDGE_VERSION_RELEASE}
)

