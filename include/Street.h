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

#ifndef __MODEL_STREET_H_
#define __MODEL_STREET_H_

#include <OsmAndCore.h>
#include <MapObject.h>
#include <QString>
#include <stdint.h>

namespace OsmAnd {

    namespace Model {

        class OSMAND_CORE_API Street : public MapObject
        {
        private:
        protected:
        public:
            Street();
            virtual ~Street();

            int64_t _id;
            QString _name;
            QString _latinName;
            int _xTile24;
            int _yTile24;
            double _longitude;
            double _latitude;
            unsigned int _offset;

            struct IntersectedStreet
            {
                QString _name;
                QString _latinName;

                int _xTile24;
                int _yTile24;
                double _longitude;
                double _latitude;
            };
        };

    } // namespace Model

} // namespace OsmAnd

#endif // __MODEL_STREET_H_
