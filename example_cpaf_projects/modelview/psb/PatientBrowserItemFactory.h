#ifndef PATIENTBROWSERITEMFACTORY_H
#define PATIENTBROWSERITEMFACTORY_H
#include <boost/function.hpp>
#include <map>

class QString;
class PatientBrowserBaseItem;

class PatientBrowserItemFactory
{
public:
    PatientBrowserItemFactory();
    PatientBrowserBaseItem* create (const QString &class_name);
private:
    typedef boost::function< PatientBrowserBaseItem* () > CreatorFunction;
    typedef std::map<QString,CreatorFunction> CreatorFunctionMap;

    CreatorFunctionMap m_factory_map;
};

#endif // PATIENTBROWSERITEMFACTORY_H
