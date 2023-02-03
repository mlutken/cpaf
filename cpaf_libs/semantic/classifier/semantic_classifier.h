#ifndef SEMANTIC_CLASSIFIER_H
#define SEMANTIC_CLASSIFIER_H

#include <semantic/config/configuration.h>

namespace cpaf::semantic
{

class semantic_classifier
{
public:
    semantic_classifier();
private:
    configuration   config_;
};

} //end namespace cpaf::semantic
#endif // SEMANTIC_CLASSIFIER_H
