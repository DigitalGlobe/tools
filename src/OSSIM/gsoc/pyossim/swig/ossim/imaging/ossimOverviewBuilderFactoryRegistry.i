%module pyossim

%{
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>
#include <ossim/imaging/ossimOverviewBuilderBase.h>
#include <ossim/base/ossimFactoryListInterface.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
%}

%template(ossimFactoryListInterface_ossimOverviewBuilderFactoryBase_ossimOverviewBuilderBase) ossimFactoryListInterface< ossimOverviewBuilderFactoryBase,ossimOverviewBuilderBase >;

%rename(__set__) ossimOverviewBuilderFactoryRegistry::operator=;

%include "ossim/base/ossimConstants.h"

/**
 * @class ossimOverviewBuilderFactoryRegistry
 * @brief The factory registry for overview builders.
 *
 * Typical usage:
 * std::vector<ossimString> typeList;
 * ossimOverviewBuilderFactoryRegistry::instance()->getTypeNameList(typeList);
 * ossimOverviewBuilderBase* builder =
 * ossimOverviewBuilderFactoryRegistry::instance()->createBuilder(
 * someStringFromTypeList);
 */
%include "ossim/imaging/ossimOverviewBuilderFactoryRegistry.h"
