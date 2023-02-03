#include "Project.h"
#include <phpstructure/File.h>

namespace phpdoc {

Project::Project()
    : m_files(2,0)
{

}

Project::~Project()
{
    for (File* f : m_files) {
        if (f) delete f;
    }
}

void Project::crawlerFileSet(File* f)
{
    File* oldFile = m_files[CRAWLER_FILE_POS];
    if (oldFile) delete oldFile;
    m_files[CRAWLER_FILE_POS] = f;
}

File* Project::crawlerFile() const
{
    return m_files[CRAWLER_FILE_POS];
}

void Project::templateFileSet(File* f)
{
    File* oldFile = m_files[TEMPLATE_FILE_POS];
    if (oldFile) delete oldFile;
    m_files[TEMPLATE_FILE_POS] = f;
}

File* Project::templateFile() const
{
    return m_files[TEMPLATE_FILE_POS];
}

} // namespace phpdoc
