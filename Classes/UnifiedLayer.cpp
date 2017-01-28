#include "UnifiedLayer.h"
#include "TM2P5DCommonInfo.h"

USING_NS_CC;

/**
 * protected functions
 */

UnifiedLayer::UnifiedLayer()
:_tiles(nullptr)
{}

UnifiedLayer::~UnifiedLayer()
{
	//release _tiles
	delete[] _tiles;
}

bool UnifiedLayer::initWithLayerIndex(int index)
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

		return true;
	}

	return false;
}

/**
 * private functions
 */

/**
 * public functions
 */

UnifiedLayer* UnifiedLayer::createWithLayerIndex(int index)
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
	auto bit_layer = BitLayer::createWithLayerInfoFile(TM2P5DCommonInfo::getInstance()->getDirectory() + "/" + file);
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

void UnifiedLayer::makeTileSpriets()
{
	int* p = _tiles;
	Size texture_size;
	Sprite* tile;
	auto map_size = TM2P5DCommonInfo::getInstance()->getGridSize();

	for(unsigned int x = 0; x < map_size.width; ++x)
	{
		for(unsigned int y = 0;y < map_size.height; ++y,++p)
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
