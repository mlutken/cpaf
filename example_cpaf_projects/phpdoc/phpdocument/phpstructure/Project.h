#ifndef PHPDOC_PROJECT_H
#define PHPDOC_PROJECT_H

#include <vector>

namespace phpdoc {

class File;

class Project
{
public:
    static const unsigned int CRAWLER_FILE_POS     = 0;
    static const unsigned int TEMPLATE_FILE_POS    = 1;

    typedef std::vector<File*>	FileVecT;

    Project();
    ~Project();


    void        crawlerFileSet  (File* f);
    File*       crawlerFile     () const;
    void        templateFileSet (File* f);
    File*       templateFile    () const;


private:
    FileVecT m_files;
};



} // namespace phpdoc

#endif // PHPDOC_PROJECT_H
