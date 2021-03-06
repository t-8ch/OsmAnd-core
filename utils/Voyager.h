/**
* @file
*
* @section LICENSE
*
* OsmAnd - Android navigation software based on OSM maps.
* Copyright (C) 2010-2013  OsmAnd Authors listed in AUTHORS file
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __VOYAGER_H_
#define __VOYAGER_H_

#include <memory>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include "CoreUtils.h"
#include "RoutingConfiguration.h"

namespace OsmAnd
{
    namespace Voyager
    {
        struct OSMAND_CORE_UTILS_API Configuration
        {
            Configuration();
            
            bool verbose;
            QList< std::shared_ptr<QFile> > obfs;
            double startLatitude;
            double startLongitude;
            double endLatitude;
            double endLongitude;

            RoutingConfiguration routingConfig;
        };
        OSMAND_CORE_UTILS_API bool OSMAND_CORE_UTILS_CALL parseCommandLineArguments(const QStringList& cmdLineArgs, Configuration& cfg, QString& error);
        OSMAND_CORE_UTILS_API void OSMAND_CORE_UTILS_CALL dumpToStdOut(const Configuration& cfg);
        OSMAND_CORE_UTILS_API QString OSMAND_CORE_UTILS_CALL dumpToString(const Configuration& cfg);
    } // namespace Voyager

} // namespace OsmAnd 

#endif // __VOYAGER_H_
