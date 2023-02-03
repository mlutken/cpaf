#include "ModelData.h"
#include <fstream>
#include <iostream>
#include <QDir>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

using namespace std;

ModelData::ModelData(string json_file_path)
{

    readJsonFile(json_file_path);
    //cout << m_json_doc.toJson().data() << endl;
    //dbgTest2(m_json_doc);

}

void ModelData::readJsonFile(string json_file_path)
{
    ifstream is(json_file_path.c_str());
    string json;
    char sz_line[2048];
    while (!is.eof() ) {
        is.getline(sz_line, 2048);
        json.append(sz_line);
        json.append("\n");
    }

    m_json_doc = QJsonDocument::fromJson(QByteArray(json.c_str()));
}




// --- debug stuff ---

void ModelData::dbgTest1()
{
    cout << "ModelData::dbgTest1: " << QDir::currentPath().toStdString() << endl;
    ifstream is("../psb/data1.json");
    string json;
    char sz_line[2048];
    while (!is.eof() ) {
        is.getline(sz_line, 2048);
        json.append(sz_line);
        json.append("\n");
    }

    QJsonDocument jdoc = QJsonDocument::fromJson( QByteArray(json.c_str()) );

    cout << jdoc.toJson().data() << endl;
    //cout << json << endl;
}

void ModelData::dbgTest2( QJsonDocument jdoc)
{
    cout << "ModelData::dbgTest2: " << QDir::currentPath().toStdString() << endl;

    QJsonObject root = jdoc.object();

//    qWarning() << root;

    cout << root.size() << endl;
    //qWarning() << root.value("patients").toObject().valueAt(0);

    QJsonArray patientArray = root["patients"].toArray();
    for (int patientIndex = 0; patientIndex < patientArray.size(); ++patientIndex) {
        QJsonObject patient = patientArray[patientIndex].toObject();
        qWarning() << "*** " << patient["patient_name"].toString() << " ***";
        qWarning() << "patient_uid:" << patient["patient_uid"].toString();

        QJsonArray studyArray = patient["studies"].toArray();
        for (int studyIndex = 0; studyIndex < studyArray.size(); ++studyIndex) {
            QJsonObject study = studyArray[studyIndex].toObject();
            qWarning() << "  study_uid:" << study["study_uid"].toString();
            qWarning() << "  study_year:" << study["study_year"].toString();

            QJsonArray seriesArray = study["series"].toArray();
            for (int seriesIndex = 0; seriesIndex < seriesArray.size(); ++seriesIndex) {
                QJsonObject series = seriesArray[seriesIndex].toObject();
                qWarning() << "    series_title:" << series["series_title"].toString();
                qWarning() << "    series_uid:" << series["series_uid"].toString();

                qWarning() << ""; // newline!
            }

            qWarning() << ""; // newline!
        }


        qWarning() << ""; // newline!
    }


    //cout << jdoc.toJson().data() << endl;
}

