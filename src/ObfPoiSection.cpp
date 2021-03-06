#include "ObfPoiSection.h"

#include <ObfReader.h>
#include <google/protobuf/wire_format_lite.h>

#include "Utilities.h"
#include "OBF.pb.h"

OsmAnd::ObfPoiSection::ObfPoiSection( class ObfReader* owner )
    : ObfSection(owner)
{
}

OsmAnd::ObfPoiSection::~ObfPoiSection()
{
}

void OsmAnd::ObfPoiSection::read( ObfReader* reader, ObfPoiSection* section)
{
    auto cis = reader->_codedInputStream.get();

    for(;;)
    {
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return;
        case OBF::OsmAndPoiIndex::kNameFieldNumber:
            {
                std::string name;
                gpb::internal::WireFormatLite::ReadString(cis, &name);
                section->_name = QString::fromStdString(name);
            }
            break;
        case OBF::OsmAndPoiIndex::kBoundariesFieldNumber:
            {
                gpb::uint32 length;
                cis->ReadVarint32(&length);
                auto oldLimit = cis->PushLimit(length);
                readBoundaries(reader, section);
                cis->PopLimit(oldLimit);
            }
            break; 
        case OBF::OsmAndPoiIndex::kCategoriesTableFieldNumber:
            cis->Skip(cis->BytesUntilLimit());
            return;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

void OsmAnd::ObfPoiSection::readCategories( ObfReader* reader, ObfPoiSection* section, QList< std::shared_ptr<OsmAnd::Model::Amenity::Category> >& categories )
{
    auto cis = reader->_codedInputStream.get();

    for(;;)
    {
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return;
        case OBF::OsmAndPoiIndex::kNameFieldNumber:
            {
                std::string name;
                gpb::internal::WireFormatLite::ReadString(cis, &name);
                section->_name = QString::fromStdString(name);
            }
            break;
        case OBF::OsmAndPoiIndex::kBoundariesFieldNumber:
            {
                gpb::uint32 length;
                cis->ReadVarint32(&length);
                auto oldLimit = cis->PushLimit(length);
                readBoundaries(reader, section);
                cis->PopLimit(oldLimit);
            }
            break; 
        case OBF::OsmAndPoiIndex::kCategoriesTableFieldNumber:
            {
                gpb::uint32 length;
                cis->ReadVarint32(&length);
                auto oldLimit = cis->PushLimit(length);
                std::shared_ptr<Model::Amenity::Category> category(new Model::Amenity::Category());
                readCategory(reader, category.get());
                cis->PopLimit(oldLimit);
                categories.push_back(category);
            }
            break;
        case OBF::OsmAndPoiIndex::kNameIndexFieldNumber:
            cis->Skip(cis->BytesUntilLimit());
            return;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

void OsmAnd::ObfPoiSection::readBoundaries( ObfReader* reader, ObfPoiSection* section )
{
    auto cis = reader->_codedInputStream.get();

    for(;;)
    {
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return;
        case OBF::OsmAndTileBox::kLeftFieldNumber:
            cis->ReadVarint32(&section->_left31);
            section->_leftLongitude = Utilities::get31LongitudeX(section->_left31);
            break;
        case OBF::OsmAndTileBox::kRightFieldNumber:
            cis->ReadVarint32(&section->_right31);
            section->_rightLongitude = Utilities::get31LongitudeX(section->_right31);
            break;
        case OBF::OsmAndTileBox::kTopFieldNumber:
            cis->ReadVarint32(&section->_top31);
            section->_topLatitude = Utilities::get31LatitudeY(section->_top31);
            break;
        case OBF::OsmAndTileBox::kBottomFieldNumber:
            cis->ReadVarint32(&section->_bottom31);
            section->_bottomLatitude = Utilities::get31LatitudeY(section->_bottom31);
            break;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

void OsmAnd::ObfPoiSection::readCategory( ObfReader* reader, OsmAnd::Model::Amenity::Category* category )
{
    auto cis = reader->_codedInputStream.get();

    for(;;)
    {
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return;
        case OBF::OsmAndCategoryTable::kCategoryFieldNumber:
            {
                std::string name;
                gpb::internal::WireFormatLite::ReadString(cis, &name);
                category->_name = QString::fromStdString(name);
            }
            break;
        case OBF::OsmAndCategoryTable::kSubcategoriesFieldNumber:
            {
                std::string name;
                gpb::internal::WireFormatLite::ReadString(cis, &name);
                category->_subcategories.push_back(QString::fromStdString(name));
            }
            break;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

void OsmAnd::ObfPoiSection::loadCategories( OsmAnd::ObfReader* reader, OsmAnd::ObfPoiSection* section, QList< std::shared_ptr<OsmAnd::Model::Amenity::Category> >& categories )
{
    auto cis = reader->_codedInputStream.get();
    cis->Seek(section->_offset);
    auto oldLimit = cis->PushLimit(section->_length);
    readCategories(reader, section, categories);
    cis->PopLimit(oldLimit);
}

void OsmAnd::ObfPoiSection::loadAmenities(
    OsmAnd::ObfReader* reader, OsmAnd::ObfPoiSection* section,
    QSet<uint32_t>* desiredCategories /*= nullptr*/,
    QList< std::shared_ptr<OsmAnd::Model::Amenity> >* amenitiesOut /*= nullptr*/,
    IQueryFilter* filter /*= nullptr*/, uint32_t zoomToSkipFilter /*= 3*/,
    std::function<bool (std::shared_ptr<OsmAnd::Model::Amenity>)>* visitor /*= nullptr*/,
    IQueryController* controller /*= nullptr*/ )
{
    auto cis = reader->_codedInputStream.get();
    cis->Seek(section->_offset);
    auto oldLimit = cis->PushLimit(section->_length);
    readAmenities(reader, section, desiredCategories, amenitiesOut, filter, zoomToSkipFilter, visitor, controller);
    cis->PopLimit(oldLimit);
}

void OsmAnd::ObfPoiSection::readAmenities(
    ObfReader* reader, ObfPoiSection* section,
    QSet<uint32_t>* desiredCategories,
    QList< std::shared_ptr<OsmAnd::Model::Amenity> >* amenitiesOut,
    IQueryFilter* filter, uint32_t zoomToSkipFilter,
    std::function<bool (std::shared_ptr<OsmAnd::Model::Amenity>)>* visitor,
    IQueryController* controller)
{
    auto cis = reader->_codedInputStream.get();
    QList< std::shared_ptr<Tile> > tiles;
    for(;;)
    {
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return;
        case OBF::OsmAndPoiIndex::kBoxesFieldNumber:
            {
                auto length = ObfReader::readBigEndianInt(cis);
                auto oldLimit = cis->PushLimit(length);
                readTile(reader, section, tiles, nullptr, desiredCategories, filter, zoomToSkipFilter, controller, nullptr);
                cis->PopLimit(oldLimit);
                if(controller && controller->isAborted())
                    return;
            }
            break;
        case OBF::OsmAndPoiIndex::kPoiDataFieldNumber:
            {
                // Sort tiles byte data offset, to all cache-friendly with I/O system
                qSort(tiles.begin(), tiles.end(), [](const std::shared_ptr<Tile>& l, const std::shared_ptr<Tile>& r) -> int
                {
                    return l->_hash - r->_hash;
                });

                for(auto itTile = tiles.begin(); itTile != tiles.end(); ++itTile)
                {
                    auto tile = *itTile;

                    cis->Seek(section->_offset + tile->_offset);
                    auto length = ObfReader::readBigEndianInt(cis);
                    auto oldLimit = cis->PushLimit(length);
                    readAmenitiesFromTile(reader, section, tile.get(), desiredCategories, amenitiesOut, filter, zoomToSkipFilter, visitor, controller, nullptr);
                    cis->PopLimit(oldLimit);
                    if(controller && controller->isAborted())
                        return;
                }
                cis->Skip(cis->BytesUntilLimit());
            }
            return;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

bool OsmAnd::ObfPoiSection::readTile(
    ObfReader* reader, ObfPoiSection* section,
    QList< std::shared_ptr<Tile> >& tiles,
    Tile* parent,
    QSet<uint32_t>* desiredCategories,
    IQueryFilter* filter, uint32_t zoomToSkipFilter,
    IQueryController* controller,
    QSet< uint64_t >* tilesToSkip)
{
    auto cis = reader->_codedInputStream.get();

    const auto zoomToSkip = (filter && filter->_zoom != std::numeric_limits<uint32_t>::max()) ? filter->_zoom + zoomToSkipFilter : std::numeric_limits<uint32_t>::max();
    QSet< uint64_t > tilesToSkip_;
    if(parent == nullptr && filter && filter->_zoom != std::numeric_limits<uint32_t>::max() && !tilesToSkip)
        tilesToSkip = &tilesToSkip_;

    std::shared_ptr<Tile> tile(new Tile());
    gpb::uint32 lzoom;
    
    for(;;)
    {
        if(controller && controller->isAborted())
            return false;
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            tiles.push_back(tile);
            return true;
        case OBF::OsmAndPoiBox::kZoomFieldNumber:
            {
                cis->ReadVarint32(&lzoom);
                tile->_zoom = lzoom;
                if(parent)
                    tile->_zoom += parent->_zoom;
            }
            break;
        case OBF::OsmAndPoiBox::kLeftFieldNumber:
            {
                auto x = ObfReader::readSInt32(cis);
                if(parent)
                    tile->_x = x + (parent->_x << lzoom);
                else
                    tile->_x = x;
            }
            break;
        case OBF::OsmAndPoiBox::kTopFieldNumber:
            {
                auto y = ObfReader::readSInt32(cis);
                if(parent)
                    tile->_y = y + (parent->_y << lzoom);
                else
                    tile->_y = y;

                // Check that we're inside bounding box, if requested
                if(filter)
                {
                    auto left31 = tile->_x << (31 - tile->_zoom);
                    auto right31 = (tile->_x + 1) << (31 - tile->_zoom);
                    auto top31 = tile->_y << (31 - tile->_zoom);
                    auto bottom31 = (tile->_y + 1) << (31 - tile->_zoom);

                    if (right31 < filter->_bboxLeft31 || left31 > filter->_bboxRight31 || top31 > filter->_bboxBottom31 || bottom31 < filter->_bboxTop31)
                    {
                        // This tile is outside of bounding box
                        cis->Skip(cis->BytesUntilLimit());
                        return false;
                    }
                }
            }
            break;
        case OBF::OsmAndPoiBox::kCategoriesFieldNumber:
            {
                if(!desiredCategories)
                {
                    ObfReader::skipUnknownField(cis, tag);
                    break;
                }
                gpb::uint32 length;
                cis->ReadLittleEndian32(&length);
                auto oldLimit = cis->PushLimit(length);
                const auto containsDesired = checkTileCategories(reader, section, desiredCategories);
                cis->PopLimit(oldLimit);
                if(!containsDesired)
                {
                    cis->Skip(cis->BytesUntilLimit());
                    return false;
                }
            }
            break;
        case OBF::OsmAndPoiBox::kSubBoxesFieldNumber:
            {
                auto length = ObfReader::readBigEndianInt(cis);
                auto oldLimit = cis->PushLimit(length);
                auto tileOmitted = readTile(reader, section, tiles, tile.get(), desiredCategories, filter, zoomToSkipFilter, controller, tilesToSkip);
                cis->PopLimit(oldLimit);

                if(tilesToSkip && tile->_zoom >= zoomToSkip && tileOmitted)
                {
                    auto skipHash = (static_cast<uint64_t>(tile->_x) >> (tile->_zoom - zoomToSkip)) << zoomToSkip;
                    skipHash |= static_cast<uint64_t>(tile->_y) >> (tile->_zoom - zoomToSkip);
                    if(tilesToSkip->contains(skipHash))
                    {
                        cis->Skip(cis->BytesUntilLimit());
                        return true;
                    }
                }
            }
            break;
        case OBF::OsmAndPoiBox::kShiftToDataFieldNumber:
            {
                tile->_offset = ObfReader::readBigEndianInt(cis);
                tile->_hash  = static_cast<uint64_t>(tile->_x) << tile->_zoom;
                tile->_hash |= static_cast<uint64_t>(tile->_y);
                tile->_hash |= tile->_zoom;
                
                // skipTiles - these tiles are going to be ignored, since we need only 1 POI object (x;y)@zoom
                if(tilesToSkip && tile->_zoom >= zoomToSkip)
                {
                    auto skipHash = (static_cast<uint64_t>(tile->_x) >> (tile->_zoom - zoomToSkip)) << zoomToSkip;
                    skipHash |= static_cast<uint64_t>(tile->_y) >> (tile->_zoom - zoomToSkip);
                    tilesToSkip->insert(skipHash);
                }
            }
            break;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

bool OsmAnd::ObfPoiSection::checkTileCategories( ObfReader* reader, ObfPoiSection* section, QSet<uint32_t>* desiredCategories )
{
    auto cis = reader->_codedInputStream.get();
    for(;;)
    {
        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return false;
        case OBF::OsmAndPoiCategories::kCategoriesFieldNumber:
            {
                gpb::uint32 binaryMixedId;
                cis->ReadVarint32(&binaryMixedId);
                const auto catId = binaryMixedId & CategoryIdMask;
                const auto subId = binaryMixedId >> SubcategoryIdShift;

                const uint32_t allSubsId = (catId << 16) | 0xFFFF;
                const uint32_t mixedId = (catId << 16) | subId;
                if(desiredCategories->contains(allSubsId) || desiredCategories->contains(mixedId))
                {
                    cis->Skip(cis->BytesUntilLimit());
                    return true;
                }
            }
            break;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

void OsmAnd::ObfPoiSection::readAmenitiesFromTile(
    ObfReader* reader, ObfPoiSection* section, Tile* tile,
    QSet<uint32_t>* desiredCategories,
    QList< std::shared_ptr<OsmAnd::Model::Amenity> >* amenitiesOut,
    IQueryFilter* filter, uint32_t zoomToSkipFilter,
    std::function<bool (std::shared_ptr<OsmAnd::Model::Amenity>)>* visitor,
    IQueryController* controller,
    QSet< uint64_t >* amenitiesToSkip)
{
    auto cis = reader->_codedInputStream.get();

    const auto zoomToSkip = (filter && filter->_zoom != std::numeric_limits<uint32_t>::max()) ? filter->_zoom + zoomToSkipFilter : std::numeric_limits<uint32_t>::max();
    QSet< uint64_t > amenitiesToSkip_;
    if(filter && filter->_zoom != std::numeric_limits<uint32_t>::max() && !amenitiesToSkip)
        amenitiesToSkip = &amenitiesToSkip_;

    int32_t x = 0;
    int32_t y = 0;
    uint32_t zoom = 0;

    for(;;)
    {
        if(controller && controller->isAborted())
            return;

        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            return;
        case OBF::OsmAndPoiBoxData::kZoomFieldNumber:
            {
                gpb::uint32 value;
                cis->ReadVarint32(&value);
                zoom = value;
            }
            break;
        case OBF::OsmAndPoiBoxData::kXFieldNumber:
            {
                gpb::uint32 value;
                cis->ReadVarint32(&value);
                x = value;
            }
            break;
        case OBF::OsmAndPoiBoxData::kYFieldNumber:
            {
                gpb::uint32 value;
                cis->ReadVarint32(&value);
                y = value;
            }
            break;
        case OBF::OsmAndPoiBoxData::kPoiDataFieldNumber:
            {
                gpb::uint32 length;
                cis->ReadVarint32(&length);
                auto oldLimit = cis->PushLimit(length);
                std::shared_ptr<Model::Amenity> amenity;
                readAmenity(reader, section, x, y, zoom, amenity, desiredCategories, filter, controller);
                cis->PopLimit(oldLimit);
                if(!amenity)
                    break;
                if(amenitiesToSkip)
                {
                    const auto xp = (uint32_t)Utilities::getTileNumberX(zoomToSkip, amenity->_longitude);
                    const auto yp = (uint32_t)Utilities::getTileNumberY(zoomToSkip, amenity->_latitude);
                    const auto hash = (static_cast<uint64_t>(xp) << zoomToSkip) | static_cast<uint64_t>(yp);
                    if(!amenitiesToSkip->contains(hash))
                    {
                        const auto visitorAgrees = visitor ? (*visitor)(amenity) : true;
                        if(visitorAgrees)
                        {
                            amenitiesToSkip->insert(hash);
                            if(amenitiesOut)
                                amenitiesOut->push_back(amenity);
                        }
                    }
                    if(zoomToSkip <= zoom)
                    {
                        cis->Skip(cis->BytesUntilLimit());
                        return;
                    }
                }
                else
                {
                    const auto visitorAgrees = visitor ? (*visitor)(amenity) : true;
                    if(amenitiesOut && visitorAgrees)
                        amenitiesOut->push_back(amenity);
                }
            }
            break;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}

void OsmAnd::ObfPoiSection::readAmenity(
    ObfReader* reader, ObfPoiSection* section, int32_t px, int32_t py, uint32_t pzoom, std::shared_ptr<Model::Amenity>& amenity,
    QSet<uint32_t>* desiredCategories,
    IQueryFilter* filter,
    IQueryController* controller)
{
    auto cis = reader->_codedInputStream.get();
    uint32_t x31;
    uint32_t y31;
    uint32_t catId;
    uint32_t subId;
    for(;;)
    {
        if(controller && controller->isAborted())
            return;

        auto tag = cis->ReadTag();
        switch(gpb::internal::WireFormatLite::GetTagFieldNumber(tag))
        {
        case 0:
            if(amenity->_latinName.isEmpty())
                amenity->_latinName = section->_owner->transliterate(amenity->_name);
            amenity->_x31 = x31;
            amenity->_y31 = y31;
            amenity->_longitude = Utilities::get31LongitudeX(x31);
            amenity->_latitude = Utilities::get31LatitudeY(y31);
            amenity->_categoryId = catId;
            amenity->_subcategoryId = subId;
            return;
        case OBF::OsmAndPoiBoxDataAtom::kDxFieldNumber:
            x31 = (ObfReader::readSInt32(cis) + (px << (24 - pzoom))) << 7;
            break;
        case OBF::OsmAndPoiBoxDataAtom::kDyFieldNumber:
            y31 = (ObfReader::readSInt32(cis) + (py << (24 - pzoom))) << 7;
            if(filter)
            {
                if (x31 < filter->_bboxLeft31 || x31 > filter->_bboxRight31 || y31 > filter->_bboxBottom31 || y31 < filter->_bboxTop31)
                {
                    cis->Skip(cis->BytesUntilLimit());
                    return;
                }
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kCategoriesFieldNumber:
            {
                gpb::uint32 value;
                cis->ReadVarint32(&value);
                catId = value & CategoryIdMask;
                subId = value >> SubcategoryIdShift;

                if(desiredCategories)
                {
                    const uint32_t allSubsId = (catId << 16) | 0xFFFF;
                    const uint32_t mixedId = (catId << 16) | subId;
                    if(desiredCategories->contains(allSubsId) || desiredCategories->contains(mixedId))
                    {
                        cis->Skip(cis->BytesUntilLimit());
                        return;
                    }
                }
                amenity.reset(new Model::Amenity());
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kIdFieldNumber:
            {
                cis->ReadVarint64(reinterpret_cast<gpb::uint64*>(&amenity->_id));
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kNameFieldNumber:
            {
                std::string name;
                gpb::internal::WireFormatLite::ReadString(cis, &name);
                amenity->_name = QString::fromStdString(name);
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kNameEnFieldNumber:
            {
                std::string latinName;
                gpb::internal::WireFormatLite::ReadString(cis, &latinName);
                amenity->_latinName = QString::fromStdString(latinName);
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kOpeningHoursFieldNumber:
            {
                std::string openingHours;
                gpb::internal::WireFormatLite::ReadString(cis, &openingHours);
                amenity->_openingHours = QString::fromStdString(openingHours);
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kSiteFieldNumber:
            {
                std::string site;
                gpb::internal::WireFormatLite::ReadString(cis, &site);
                amenity->_site = QString::fromStdString(site);
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kPhoneFieldNumber:
            {
                std::string phone;
                gpb::internal::WireFormatLite::ReadString(cis, &phone);
                amenity->_phone = QString::fromStdString(phone);
            }
            break;
        case OBF::OsmAndPoiBoxDataAtom::kNoteFieldNumber:
            {
                std::string note;
                gpb::internal::WireFormatLite::ReadString(cis, &note);
                amenity->_description = QString::fromStdString(note);
            }
            break;
        default:
            ObfReader::skipUnknownField(cis, tag);
            break;
        }
    }
}
