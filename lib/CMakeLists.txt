MACRO (FILE_TO_STRING infile outfile var)
ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
                   COMMAND ${CMAKE_COMMAND}
                   ARGS -Dinfile:FILEPATH=xml/${infile}
                        -Doutfile:FILEPATH=${outfile}
                        -Dvar:STRING=${var}
                        -P filetostring.cmake
                   MAIN_DEPENDENCY
                        xml/${infile}
)
ENDMACRO (FILE_TO_STRING)

FILE_TO_STRING(form_templates.xml form_templates_xml.cpp form_templates_xml)
FILE_TO_STRING(messages.xml messages_xml.cpp messages_xml)

SET (ojudge_lib
form_templates_xml.cpp
messages_xml.cpp
datastore/CategoryStore.cpp
datastore/DataStore.cpp
datastore/LanguageStore.cpp
datastore/ProblemStore.cpp
dbmodel/DBModel.cpp
viewmodels/CategoryModel.cpp
viewmodels/LanguageModel.cpp
viewmodels/ProblemModel.cpp
viewmodels/ViewModels.cpp
widgets/OJCodeEditorWidget.cpp
widgets/OJColorPicker.cpp
widgets/OJProblemViewerWidget.cpp
widgets/OJRatingSetWidget.cpp
widgets/OJRatingViewWidget.cpp
AboutWidget.cpp
AdminWidget.cpp
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
PdfResource.cpp
ProblemsWidget.cpp
ProblemWidget.cpp
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

TARGET_LINK_LIBRARIES (ojudge
        ${Boost_LIBRARIES}
        Wt::Wt
        Wt::HTTP
        Wt::Dbo
        Wt::DboPostgres
)
