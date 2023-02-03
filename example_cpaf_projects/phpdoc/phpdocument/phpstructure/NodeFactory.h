#ifndef PHPDOC_NODEFACTORY_H
#define PHPDOC_NODEFACTORY_H

#include <string>
#include <map>
#include <functional>
#include <boost/function.hpp>

namespace phpdoc {

class Node;

class NodeFactory
{
public:

    NodeFactory();
    typedef boost::function< Node* () > NodeCreatorFunT;

    void    registerClass   (const std::string& className, const NodeCreatorFunT& creatorFun);
    Node*   create          (const std::string& className) const;

    unsigned int size() const { return m_nodeCreateMap.size(); }

private:
    typedef std::map<std::string, NodeCreatorFunT>  MapT;

    MapT m_nodeCreateMap;
};

} // namespace phpdoc

#endif // PHPDOC_NODEFACTORY_H
