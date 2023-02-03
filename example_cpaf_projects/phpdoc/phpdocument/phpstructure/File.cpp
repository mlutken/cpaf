#include "File.h"

using namespace std;

namespace phpdoc {

PHPDOC_NODE_CPP(File,Group)

File::File()
{
}

boost::filesystem::path File::filePath() const
{
    return m_filePath;
}

File* File::setFilePath(const boost::filesystem::path& filePath)
{
    m_filePath = filePath;
    return this;
}

std::string File::do_docStringGet() const
{
    string s("<?php\n");
    for (Node* pNode : children()) {
        s += pNode->docString();
    }
    s.append("?>\n");
    return s;
}


} // namespace phpdoc
