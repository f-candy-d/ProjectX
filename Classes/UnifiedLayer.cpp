#include "UnifiedLayer.h"
#include "TM2P5DCommonInfo.h"
#include "Constant.h"

USING_NS_CC;

/**
 * protected functions
 */

UnifiedLayer::UnifiedLayer()
:_tiles(nullptr)
,_atIndex(0)
{}

UnifiedLayer::~UnifiedLayer()
{
	//release _tiles
	delete[] _tiles;
}

bool UnifiedLayer::initWithLayerIndex(size_t index)
{
	if(!Node::init())
	{
		return false;
	}

	auto cmn_info = TM2P5DCommonInfo::getInstance();
	if(cmn_info->getIsSuccessedInit())
	{
		auto map_size = cmn_info->getGridSize();
		_tiles = new int[(unsigned int)(map_size.width * map_size.height)]{0};
		_atIndex = index;

		return true;
	}

	return false;
}

/**
 * private functions
 */

Vec2 UnifiedLayer::convertGridPosToLocalPos(unsigned int x,unsigned int y,const Size& tileSize)
{
	return Vec2(tileSize.width * (x + 0.5),tileSize.height * (y + 0.5));
}

/**
 * public functions
 */

UnifiedLayer* UnifiedLayer::createWithLayerIndex(size_t index)
{
	UnifiedLayer* ret = new UnifiedLayer();
	if(ret->initWithLayerIndex(index))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void UnifiedLayer::addBitLayer(std::string file,int flag)
{
	//Make the full path to the 'file'.
	std::string file_path =
	Constant::RESOURCES_DIRECTORY +
	TM2P5DCommonInfo::getInstance()->getDirectory() +
	file;

	auto bit_layer = BitLayer::createWithLayerInfoFile(file_path);
	if(bit_layer != nullptr)
	{
		bit_layer->setBitFlag(flag);
		_bitLayers.pushBack(bit_layer);
		//Add SpriteBatchNode
		if(bit_layer->getSpriteBatchNode() != nullptr)
			this->addChild(bit_layer->getSpriteBatchNode());

		int* p = _tiles;
		auto map_size = TM2P5DCommonInfo::getInstance()->getGridSize();
		for(unsigned int x = 0; x < map_size.width; ++x)
		{
			for(unsigned int y = 0; y < map_size.height; ++y,++p)
			{
				if(bit_layer->isOwnAnyTileAt(x,y))
					*p = *p | flag;
			}
		}
	}
}

void UnifiedLayer::makeTileSpriets(GridRect makeGr)
{
	auto tile_size = TM2P5DCommonInfo::getInstance()->getTileSizePx();
	auto grid_size = TM2P5DCommonInfo::getInstance()->getGridSize();
	int* pt = static_cast<int>(makeGr.x * grid_size.height + makeGr.y) + _tiles;
	int diff = static_cast<int>(grid_size.height) - makeGr.height;
	int nw;
	Size texture_size;
	float p,q;
	BitLayer::TileInfo tile_info;

	int c = 0;

	for(unsigned int x = makeGr.x; x < makeGr.width; ++x)
	{
		for(unsigned int y = makeGr.y; y < makeGr.height; ++y,++pt)
		{
			for(auto bit_layer : _bitLayers)
			{
				if(bit_layer->getBitFlag() & *pt)
				{
					//Make a Rect object.
					texture_size = bit_layer->getSpriteBatchNode()->getTextureAtlas()->getTexture()->getContentSize();
					tile_info = bit_layer->getTileInfoAtGridPos(x,y);
					nw = texture_size.width / tile_size.width;
					q = tile_size.height * static_cast<int>(tile_info.tileType / nw);
					p = tile_size.width * (tile_info.tileType - nw * q);
					auto texture_rect = Rect(p,q,tile_size.width,tile_size.height);
					// log("texture_rect[%f,%f,%f.0,%f.0] type=%d nw=%d x=%d y=%d",p,q,texture_size.width,texture_size.height,tile_info.tileType,nw,x,y);

					//Make a tile sprite.
					auto tile = Sprite::create(
						TM2P5DCommonInfo::getInstance()->getDirectory() + bit_layer->getTileAtlasFile(),texture_rect);
					tile->setPosition(convertGridPosToLocalPos(x,y,tile_size));
					bit_layer->getSpriteBatchNode()->addChild(tile);
					c++;

					break;
				}
			}
		}
		pt += diff;
	}

	log("<<<<<<<<<<<<<<Made %d tiles>>>>>>>>>>>>>makeGr(%zu)",c,makeGr.width*makeGr.height);
}
