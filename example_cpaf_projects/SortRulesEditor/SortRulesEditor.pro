#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T11:08:04
#
#-------------------------------------------------
CONFIG += c++11

QT       += core gui widgets script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SortRulesEditor
TEMPLATE = app


SOURCES += main.cc\
    MainWindow.cc \
    SimpleComboBoxDelegate.cc \
    StringCompareOperators.cc \
    SerializationUtils.cc \
    DisplaySetSorting.cc \
    SynonymsManager.cc \
    ScriptableHP_SortRulesEditorModel.cc \
    HPJavascriptHookEditor.cc \
    JavascriptEdit.cc \
    ScriptableHPGui_WidgetBase.cc \
    ScriptableHPGui_DisplaysetHanging.cc \
    utils.cc \
    ScriptableHPGui_SortRulesEditor.cc \
    ScriptableHPGui_TemporaryTabsForDevelopment.cc \
    ScriptableHPGui_SeriesSelection.cc \
    ScriptableHPGui_DisplaySetGeneration.cc \
    ScriptableHPGui_PerInstanceDisplaySetEditor.cc \
    ScriptableHPGui_EditorManager.cc \
    ScriptableHPGui_PresentationGroupAssignment.cc \
    ScriptableHPGui_DisplaySetTilingEditor.cc


HEADERS  += MainWindow.h \
    SimpleComboBoxDelegate.h \
    evbase/StringCompareOperators.h \
    evbase/SerializationUtils.h \
    vdbclient/DisplaySetSorting.h \
    vdbclient/SynonymsManager.h \
    qtutility/JavascriptEdit.h \
    ScriptableHP_SortRulesEditorModel.h \
    HPJavascriptHookEditor.h \
    ScriptableHPGui_WidgetBase.h \
    ScriptableHPGui_DisplaysetHanging.h \
    utils.h \
    ScriptableHPGui_SortRulesEditor.h \
    ScriptableHPGui_TemporaryTabsForDevelopment.h \
    ScriptableHPGui_SeriesSelection.h \
    ScriptableHPGui_DisplaySetGeneration.h \
    ScriptableHPGui_PerInstanceDisplaySetEditor.h \
    ScriptableHPGui_EditorManager.h \
    ScriptableHPGui_PresentationGroupAssignment.h \
    ScriptableHPGui_DisplaySetTilingEditor.h
