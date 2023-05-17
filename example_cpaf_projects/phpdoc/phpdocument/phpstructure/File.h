#ifndef PHPDOC_FILE_H
#define PHPDOC_FILE_H

#include <boost/filesystem.hpp>

#include "Group.h"


namespace phpdoc {

class File : public Group
{
    PHPDOC_NODE_HDR(File,Group)
public:
    File();
    std::filesystem::path filePath() const;
    File* setFilePath(const std::filesystem::path& filePath);

private:

    virtual std::string do_docStringGet () const override;

    std::filesystem::path m_filePath;

};

} // namespace phpdoc

#endif // PHPDOC_FILE_H
