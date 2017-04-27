%module pyossim

%{
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryBase.h>
%}

%include "ossim/base/ossimConstants.h"

/**
 * @class ossimOverviewBuilderFactoryBase
 * @brief The base class for factory overview builder factories.
 */
%include "ossim/imaging/ossimOverviewBuilderFactoryBase.h"
