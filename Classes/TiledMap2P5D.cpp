#include "TiledMap2P5D.h"
#include "TM2P5DCommonInfo.h"

USING_NS_CC;

/**
 * Constant values
 */
const size_t MAX_UNIFIED_LAYERS_NUM = 5;

/**
 * protected functions
 */
TiledMap2P5D::TiledMap2P5D()
{}

TiledMap2P5D::~TiledMap2P5D()
{}

bool TiledMap2P5D::initWithFile(std::string file)
{
	log("initWithFile");
	if(!Node::init())
		return false;

	//Initialize TM2P5DCommonInfo class.
	if(!TM2P5DCommonInfo::getInstance()->initWithFile(file))
	{
		log("Initializing TM2P5DCommonInfo class failed.");
		return false;
	}

	log("reserve");
	//Reserve memorys for UnifiedLayer objects.
	_layers.reserve(MAX_UNIFIED_LAYERS_NUM);
	log("start loop");
	//Make unified layers
	for (size_t i = 0 , n = TM2P5DCommonInfo::getInstance()->getNumOfUnifiedLayers(); i < n; i++)
	{
		addUnifiedLayer(i);
	}

	return true;
}

/**
 * private functions
 */

void TiledMap2P5D::addUnifiedLayer(size_t index)
{
	log("addUnifiedLayer");
	if (index <= MAX_UNIFIED_LAYERS_NUM) {
		auto unified_layer = UnifiedLayer::createWithLayerIndex(index);
		_layers.insert(index,unified_layer);
		auto bit_layer_names = TM2P5DCommonInfo::getInstance()->getBitLayerNamesInUnifiedLayer(index);

		int tag;
		int sht = 0;
		for(const auto& name : bit_layer_names)
		{
			//bit mask
			log("name -> [%s]",name.c_str());
			tag = 1 << sht;
			unified_layer->addBitLayer(name,tag);
			sht++;
			if(sht > 32)
			{
				log("Error::Threre is too many bit layers in the unified layer (index %d)",
					static_cast<int>(index));
				break;
			}
		}

		//make tile sprites
		unified_layer->makeTileSpriets();
		//add
		this->addChild(unified_layer);
	}
}

/**
 * public functions
 */

TiledMap2P5D* TiledMap2P5D::create(std::string file)
{
	TiledMap2P5D* ret = new TiledMap2P5D();
	if(ret->initWithFile(file))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_RELEASE_NULL(ret);
	return nullptr;
}
