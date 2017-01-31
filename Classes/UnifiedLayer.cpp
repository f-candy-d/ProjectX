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

Sprite* UnifiedLayer::reInitTileSprite(cocos2d::Sprite* oldTile,size_t x,size_t y)
{
	if(oldTile == nullptr)
	{
		//Create new sprite.
		auto tile_size = TM2P5DCommonInfo::getInstance()->getTileSizePx();
		auto grid_size = TM2P5DCommonInfo::getInstance()->getGridSize();
		int nw;
		Size texture_size;
		float p,q;
		BitLayer::TileInfo tile_info;
		for(const auto& bit_layer : _bitLayers)
		{
			size_t index = x * grid_size.height + y;
			if(bit_layer->getBitFlag() & _tiles[index])
			{
				//Make a Rect object.
				texture_size = bit_layer->getSpriteBatchNode()->getTextureAtlas()->getTexture()->getContentSize();
				tile_info = bit_layer->getTileInfoAtGridPos(x,y);
				nw = texture_size.width / tile_size.width;
				q = tile_size.height * static_cast<int>(tile_info.tileType / nw);
				p = tile_size.width * (tile_info.tileType - nw * q);
				auto texture_rect = Rect(p,q,tile_size.width,tile_size.height);

				//Make a tile sprite.
				auto tile =
					Sprite::create(TM2P5DCommonInfo::getInstance()->getDirectory() + bit_layer->getTileAtlasFile(),texture_rect);
				tile->setPosition(convertGridPosToLocalPos(x,y,tile_size));
				tile->setTag(bit_layer->getBitFlag());
				bit_layer->getSpriteBatchNode()->addChild(tile);

				return std::move(oldTile);
			}
		}
	}
	else
	{
		//Reuse old sprite object.
		oldTile->getBatchNode()->removeChild(oldTile,true);
		
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
					auto tile =
						Sprite::create(TM2P5DCommonInfo::getInstance()->getDirectory() + bit_layer->getTileAtlasFile(),texture_rect);
					tile->setPosition(convertGridPosToLocalPos(x,y,tile_size));
					bit_layer->getSpriteBatchNode()->addChild(tile);

					//Add new sprite into Map.
					size_t hash = bit_layer->makeHashXYD(x,y);
					tile->setTag(bit_layer->getBitFlag());
					_tileSprites.insert(hash,tile);

					break;
				}
			}
		}
		pt += diff;
	}
}

void UnifiedLayer::optimizeTileSprites(Vec2 netMv)
{
	Sprite* tmpSp = nullptr;
	size_t new_p;

	if(netMv.x != 0)
	{
		//If netMv.x > 0, The drawnGridRect moved to right of the screen from left of that.
		//Otherwise it moved to opposite side of the screen.

		//Remove old tile sprite.
		for(size_t x =
				(netMv.x > 0)
				? _drawnGridRect.x - netMv.x
				: _drawnGridRect.x + _drawnGridRect.width -1 - netMv.x,
			i = 0;
			i < std::fabs(netMv.x);
			++i,++x)
		{
			for(size_t y = _drawnGridRect.y; y < drawnGridRect.height; ++y)
			{
				hash = makeHashXYD(x,y);
				if((auto itr = _tileSprites.find(makeHashXYD(x,y)) != _tileSprites.end()))
				{
					//Get Sprite* from the iterator.
					tmpSp = itr->second;
					//Remove an old tile from the hash-map.
					_tileSprites.erase(itr);
				}

				//Re-initialize tile sprite.
				new_p =
					(netMv.x > 0)
					? _drawnGridRect.x + _drawnGridRect.width - netMv.x
					: _drawnGridRect.x + i;

				tmpSp = reInitTileSprite(tmpSp,new_p,y);
				if(tmpSp != nullptr)
				{
					//Restore a tile sprite to the hash-map.
					_tileSprites.insert(makeHashXYD(new_p,y),tmpSp);
				}

				tmpSp = nullptr;
			}
		}
	}

	if(netMv.y != 0)
	{
		//If netMv.y > 0, The drawnGridRect moved to right of the screen from left of that.
		//Otherwise it moved to opposite side of the screen.

		//Remove old tile sprite.
		for(size_t y =
				(netMv.y > 0)
				? _drawnGridRect.y - netMv.y
				: _drawnGridRect.y + _drawnGridRect.width -1 - netMv.y,
			i = 0;
			i < std::fabs(netMv.y);
			++i,++y)
		{
			for(size_t x = _drawnGridRect.x; x < drawnGridRect.height; ++x)
			{
				hash = makeHashXYD(x,y);
				if((auto itr = _tileSprites.find(makeHashXYD(x,y)) != _tileSprites.end()))
				{
					//Get Sprite* from the iterator.
					tmpSp = itr->second;
					//Remove an old tile from the hash-map.
					_tileSprites.erase(itr);
				}

				//Re-initialize tile sprite.
				new_p =
					(netMv.y > 0)
					? _drawnGridRect.y + _drawnGridRect.width - netMv.y
					: _drawnGridRect.y + i;

				tmpSp = reInitTileSprite(tmpSp,x,new_p);
				if(tmpSp != nullptr)
				{
					//Restore a tile sprite to the hash-map.
					_tileSprites.insert(makeHashXYD(y,new_p),tmpSp);
				}

				tmpSp = nullptr;
			}
		}
	}
}
