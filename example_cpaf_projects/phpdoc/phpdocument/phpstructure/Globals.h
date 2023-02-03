#ifndef PHPDOC_GLOBALS_H
#define PHPDOC_GLOBALS_H

#include <loki/Singleton.h>
#include <phpstructure/NodeFactory.h>

namespace phpdoc {

class Globals
{
public:
    Globals();
    ~Globals();

    NodeFactory&    fac() { return m_fac; }
private:

    NodeFactory m_fac;
};


typedef Loki::SingletonHolder<
    Globals,
    Loki::CreateUsingNew,
    Loki::NoDestroy
> G;

inline Globals& g()
{
    return G::Instance();
}

} // namespace phpdoc

#endif // PHPDOC_GLOBALS_H
