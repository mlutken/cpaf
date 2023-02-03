#ifndef MODELDATA_H
#define MODELDATA_H

#include <QJsonDocument>

class ModelData
{
public:
    explicit ModelData( std::string json_file_path);
    void readJsonFile( std::string json_file_path);

    // --- debug stuff ---
    static void dbgTest1();
    static void dbgTest2( QJsonDocument jdoc);
private:
    QJsonDocument   m_json_doc;
};

#endif // MODELDATA_H
