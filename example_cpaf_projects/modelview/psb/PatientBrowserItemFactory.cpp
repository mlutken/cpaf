#include "PatientBrowserItemFactory.h"
#include <boost/assign.hpp>
#include <boost/function.hpp>
#include <QString>

#include <PatientBrowserBaseItem.h>
#include <PatientBrowserRootItem.h>
#include <PatientBrowserYearItem.h>
#include <PatientBrowserPatientItem.h>
#include <PatientBrowserStudyItem.h>
#include <PatientBrowserSeriesItem.h>


PatientBrowserItemFactory::PatientBrowserItemFactory()
{
    m_factory_map = boost::assign::map_list_of
            ("RootItem",PatientBrowserRootItem::create)
            ("PatientItem",PatientBrowserPatientItem::create)
            ("SeriesItem",PatientBrowserSeriesItem::create)
            ("StudyItem",PatientBrowserSeriesItem::create)
            ("YearItem",PatientBrowserSeriesItem::create)
        ;
}

PatientBrowserBaseItem *PatientBrowserItemFactory::create(const QString &class_name)
{
    const CreatorFunctionMap::const_iterator itEND = m_factory_map.end();
    CreatorFunctionMap::const_iterator it = m_factory_map.find(class_name);

    PatientBrowserBaseItem *base_item = 0;
    if (it != itEND) base_item = (*it).second();
    return base_item;
}
