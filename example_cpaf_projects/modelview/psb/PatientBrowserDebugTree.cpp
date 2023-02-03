#include "PatientBrowserDebugTree.h"

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

#include <PatientBrowserBaseItem.h>
#include <PatientBrowserRootItem.h>
#include <PatientBrowserYearItem.h>
#include <PatientBrowserPatientItem.h>
#include <PatientBrowserStudyItem.h>
#include <PatientBrowserSeriesItem.h>

using namespace std;

PatientBrowserDebugTree::PatientBrowserDebugTree()
{
}

PatientBrowserBaseItem *PatientBrowserDebugTree::createSimple()
{
    PatientBrowserBaseItem *root;
    PatientBrowserBaseItem *parent;
    PatientBrowserBaseItem *item;


    item = new PatientBrowserRootItem();
    root = item;

    parent = item;
    item = new PatientBrowserPatientItem();
    item->setDbgText("1");
    parent->appendChild(item);

    item = new PatientBrowserPatientItem(parent);
    item->setDbgText("2");

    parent = item;
    item = new PatientBrowserStudyItem(parent);
    item->setDbgText("1");
    item = new PatientBrowserStudyItem(parent);
    item->setDbgText("2");

    return root;
}


void PatientBrowserDebugTree::print(PatientBrowserBaseItem *root, int level)
{
    qWarning() << getIndent(level) << root->dbgText();
    if ( root->hasChildren() ) {
        BOOST_FOREACH( PatientBrowserBaseItem *child, root->children() ) {
            PatientBrowserDebugTree::print(child, level+1);
        }
    }
}

QString PatientBrowserDebugTree::getIndent(int level)
{
    QString indent;
    for ( int i =0; i < level; ++i) {
        indent.append("  ");
    }
    return indent;
}



PatientBrowserBaseItem *PatientBrowserDebugTree::readFromJson(const QString &json_file_path)
{
    PatientBrowserBaseItem *data_root_item = new PatientBrowserRootItem();

    qWarning() << "PatientBrowserDebugTree::dbgTest2: " << QDir::currentPath();

    //ifstream is("../psb/data1.json");
    ifstream is (json_file_path.toUtf8().data() );
    string json;
    char sz_line[2048];
    while (!is.eof() ) {
        is.getline(sz_line, 2048);
        json.append(sz_line);
        json.append("\n");
    }

    QJsonDocument jdoc = QJsonDocument::fromJson( QByteArray(json.c_str()) );

    QJsonObject root = jdoc.object();

//    qWarning() << root;

    qWarning() << root.size() << endl;
    //qWarning() << root.value("patients").toObject().valueAt(0);

    QJsonArray patientArray = root["patients"].toArray();
    for (int patientIndex = 0; patientIndex < patientArray.size(); ++patientIndex) {
        QJsonObject patient = patientArray[patientIndex].toObject();
        qWarning() << "*** " << patient["patient_name"].toString() << " ***";
        qWarning() << "patient_uid:" << patient["patient_uid"].toString();

        PatientBrowserBaseItem *parent = data_root_item;
        PatientBrowserPatientItem *patient_item = new PatientBrowserPatientItem(parent);
        patient_item->setDbgText(patient["patient_name"].toString());


        QJsonArray studyArray = patient["studies"].toArray();
        for (int studyIndex = 0; studyIndex < studyArray.size(); ++studyIndex) {
            QJsonObject study = studyArray[studyIndex].toObject();
            qWarning() << "  study_uid:" << study["study_uid"].toString();
            qWarning() << "  study_year:" << study["study_year"].toString();
            PatientBrowserBaseItem *parent = patient_item;
            PatientBrowserStudyItem *study_item = new PatientBrowserStudyItem(parent);
            study_item->setDbgText(study["study_uid"].toString());

            QJsonArray seriesArray = study["series"].toArray();
            for (int seriesIndex = 0; seriesIndex < seriesArray.size(); ++seriesIndex) {
                QJsonObject series = seriesArray[seriesIndex].toObject();
                qWarning() << "    series_title:" << series["series_title"].toString();
                qWarning() << "    series_uid:" << series["series_uid"].toString();
                qWarning() << ""; // newline!
                PatientBrowserBaseItem *parent = study_item;
                PatientBrowserSeriesItem *series_item = new PatientBrowserSeriesItem(parent);
                series_item->setDbgText(series["series_uid"].toString());

            }

            qWarning() << ""; // newline!
        }


        qWarning() << ""; // newline!
    }


    return data_root_item;
}




//    PatientBrowserPatientItem *patient_item;
//    PatientBrowserYearItem *year_item;
//    PatientBrowserStudyItem *patient_item;
//    PatientBrowserSeriesItem *patient_item;
