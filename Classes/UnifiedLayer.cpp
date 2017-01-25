#include "UnifiedLayer.h"

USING_NS_CC;

/**
 * protected functions
 */

UnifiedLayer::UnifiedLayer()
:_mapSize(0,0)
,_directory("")
,_tiles(nullptr)
{}

UnifiedLayer::~UnifiedLayer()
{
	//release _tiles
	delete[] _tiles;
}

bool UnifiedLayer::initWithMapInfo(std::string directory,cocos2d::Size map_size)
{
	if(!Node::init())
	{
		return false;
	}

	_mapSize = Size(map_size);
	_directory = std::string(directory);
	_tiles = new int[(unsigned int)(map_size.width * map_size.height)]{0};

	return true;
}

/**
 * private functions
 */

/**
 * public functions
 */

UnifiedLayer* UnifiedLayer::createWithMapInfo(std::string directory,cocos2d::Size map_size)
{
	UnifiedLayer* ret = new UnifiedLayer();
	if(ret->initWithMapInfo(directory, map_size))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void UnifiedLayer::addBitLayer(std::string file,int flag)
{
	auto bit_layer = BitLayer::createWithLayerInfoFile(_directory + file);
	if(bit_layer != nullptr)
	{
		bit_layer->setBitFlag(flag);
		_bitLayers.pushBack(bit_layer);
		//Add SpriteBatchNode
		if(bit_layer->getSpriteBatchNode() != nullptr)
			this->addChild(bit_layer->getSpriteBatchNode());

		int* p = _tiles;
		for(unsigned int x = 0; x < _mapSize.width; ++x)
		{
			for(unsigned int y = 0; y < _mapSize.height; ++y,++p)
			{
				if(bit_layer->isOwnAnyTileAt(x,y))
					*p = *p | flag;
			}
		}
	}
}

void UnifiedLayer::makeTileSpriets()
{
	int* p = _tiles;
	Size texture_size;
	Sprite* tile;

	for(unsigned int x = 0; x < _mapSize.width; ++x)
	{
		for(unsigned int y = 0;y < _mapSize.height; ++y,++p)
		{
			for(auto bit_layer : _bitLayers)
			{
				if(bit_layer->getBitFlag() | *p)
				{
					texture_size = bit_layer->getSpriteBatchNode()->getTextureAtlas()->getTexture()->getContentSize();

					break;
				}
			}
		}
	}
}
