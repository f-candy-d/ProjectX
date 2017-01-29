#include "TiledMap2P5D.h"
#include "TM2P5DCommonInfo.h"

USING_NS_CC;

/**
 * Constant values
 */
const size_t MAX_UNIFIED_LAYERS_NUM = 5;
const size_t GRID_CLEARANCE = 1;
const float DEFAULT_MAP_POS_X = 0.0;
const float DEFAULT_MAP_POS_Y = 0.0;

/**
 * protected functions
 */
TiledMap2P5D::TiledMap2P5D()
:_visibleRect(0,0,0,0)
,_drawnGridRect(0,0,0,0)
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

	//Default visible Rect
	auto win_size = Director::getInstance()->getWinSize();
	_visibleRect = Rect(DEFAULT_MAP_POS_X,DEFAULT_MAP_POS_Y,win_size.width,win_size.height);
		log("winSize(%f,%f)",win_size.width,win_size.height);

	//Set default position
	this->setPosition(DEFAULT_MAP_POS_X,DEFAULT_MAP_POS_Y);

	//Setting for recycling tile sprites.
	resizeDrawnRect();

	//Reserve memorys for UnifiedLayer objects.
	_layers.reserve(MAX_UNIFIED_LAYERS_NUM);
	//Make unified layers
	for (size_t i = 0 , n = TM2P5DCommonInfo::getInstance()->getNumOfUnifiedLayers(); i < n; ++i)
	{
		if(!addUnifiedLayer(i))
			return false;
	}


	return true;
}

/**
 * private functions
 */

bool TiledMap2P5D::addUnifiedLayer(size_t index)
{
	log("addUnifiedLayer");
	if (index <= MAX_UNIFIED_LAYERS_NUM) {
		auto unified_layer = UnifiedLayer::createWithLayerIndex(index);
		if(unified_layer == nullptr)
			return false;

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
		unified_layer->makeTileSpriets(_drawnGridRect);
		//add
		this->addChild(unified_layer);

		return true;
	}

	return false;
}

void TiledMap2P5D::resizeDrawnRect()
{
	const auto& vis_origin = _visibleRect.origin;
	const auto& vis_size = _visibleRect.size;
	const auto& tile_size = TM2P5DCommonInfo::getInstance()->getTileSizePx();
	int dx,dy;
	size_t dw,dh;
	auto pq = this->getPosition();

	dx = static_cast<int>((vis_origin.x - pq.x) / tile_size.width) - GRID_CLEARANCE;
	dy = static_cast<int>((vis_origin.y - pq.y) / tile_size.height) - GRID_CLEARANCE;
	dx = std::max(0,dx);
	dy = std::max(0,dy);
	dw = static_cast<int>(((vis_origin.x + vis_size.width) - (pq.x + dx)) / tile_size.width + 1) + GRID_CLEARANCE;
	dh = static_cast<int>(((vis_origin.y + vis_size.height) - (pq.y + dy)) / tile_size.height + 1) + GRID_CLEARANCE;

	//set values
	_drawnGridRect.x = static_cast<size_t>(dx);
	_drawnGridRect.width = dw;
	_drawnGridRect.y = static_cast<size_t>(dy);
	_drawnGridRect.height = dh;

	log("drawnGridRect(%zu,%zu,%zu,%zu)",
	_drawnGridRect.x,_drawnGridRect.y,_drawnGridRect.width,_drawnGridRect.height);
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

ccTouchCallback TiledMap2P5D::getLambdaOnTouchMoved()
{
	return [this](Touch* touch,Event* event)
	{
		//On the parent node is touched and moved...
		Vec2 delta = touch->getDelta();
		Vec2 position = this->getPosition();
		Vec2 new_position = position + delta;
		this->setPosition(new_position);
	};
}
