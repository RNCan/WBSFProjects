//******************************************************************************
//  Project:		Weather-based simulation framework (WBSF)
//	Programmer:     R�mi Saint-Amant
//
//  It under the terms of the GNU General Public License as published by
//     the Free Software Foundation
//  It is provided "as is" without express or implied warranty.
//******************************************************************************
#pragma once

//#pragma warning(disable: 4275 4251)

#define ACCEPT_USE_OF_DEPRECATED_PROJ_API_H

#if defined(_WIN32) || defined(BUILDING_FROM_VCPKG)
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#else
#include "/usr/local/include/gdal_priv.h"
#include "/usr/local/include/ogr_spatialref.h"
#endif
