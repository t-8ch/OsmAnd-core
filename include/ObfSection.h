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

#ifndef __OBF_SECTION_H_
#define __OBF_SECTION_H_

#include <OsmAndCore.h>
#include <QString>
#include <stdint.h>

namespace OsmAnd {

    class ObfReader;

    /**
    Base section in OsmAnd binary file
    */
    struct OSMAND_CORE_API ObfSection
    {
        ObfSection(ObfReader* owner);
        virtual ~ObfSection();

        QString _name;
        uint32_t _length;
        uint32_t _offset;

    protected:
        ObfReader* const _owner;
    };

} // namespace OsmAnd

#endif // __OBF_SECTION_H_