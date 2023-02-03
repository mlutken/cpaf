#ifndef PHPDOC_COMMENTMULTILINE_H
#define PHPDOC_COMMENTMULTILINE_H
#include "Comment.h"

namespace phpdoc {

class CommentMultiLine : public Comment
{
    PHPDOC_NODE_HDR(CommentMultiLine,Comment)
public:
    virtual std::string do_docStringGet () const;
    virtual void        do_valueSet     (const std::string& val);

    std::string m_val;
};

} // namespace phpdoc

#endif // PHPDOC_COMMENTMULTILINE_H
