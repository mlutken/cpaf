#ifndef PHPDOC_COMMENTSINGLELINE_H
#define PHPDOC_COMMENTSINGLELINE_H

#include "Leaf.h"

namespace phpdoc {

class CommentSingleLine : public Leaf
{
    PHPDOC_NODE_HDR(CommentSingleLine,Leaf)
public:
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& val);

    std::string m_val;
};

} // namespace phpdoc

#endif // PHPDOC_COMMENTSINGLELINE_H
