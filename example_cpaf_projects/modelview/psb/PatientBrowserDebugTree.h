#ifndef PATIENTBROWSERDEBUGTREE_H
#define PATIENTBROWSERDEBUGTREE_H

class QString;
class PatientBrowserBaseItem;


class PatientBrowserDebugTree
{
public:
    PatientBrowserDebugTree();
    static PatientBrowserBaseItem *createSimple();
    static PatientBrowserBaseItem *readFromJson( const QString& json_file_path );

    static void print(PatientBrowserBaseItem *root, int level);

private:
    static QString getIndent(int level);

};

#endif // PATIENTBROWSERDEBUGTREE_H
