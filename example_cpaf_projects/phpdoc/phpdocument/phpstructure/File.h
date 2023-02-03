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
    boost::filesystem::path filePath() const;
    File* setFilePath(const boost::filesystem::path& filePath);

private:

    virtual std::string do_docStringGet () const override;

    boost::filesystem::path m_filePath;

};

} // namespace phpdoc

#endif // PHPDOC_FILE_H
