#include "TM2P5DCommonInfo.h"

USING_NS_CC;

/**
 * Protected functions
 */

TM2P5DCommonInfo::TM2P5DCommonInfo()
{}

TM2P5DCommonInfo::~TM2P5DCommonInfo()
{}

bool TM2P5DCommonInfo::initWithMapInfo(std::string file)
{
	return true;
}

/**
 * Private functions
 */

/**
 * Public functions
 */

TM2P5DCommonInfo* TM2P5DCommonInfo::createWithMapInfo(std::string file)
{
	TM2P5DCommonInfo* ret = new TM2P5DCommonInfo();
	if(ret->initWithMapInfo(file))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_RELEASE_NULL(ret);
	return nullptr;
}

Rect TM2P5DCommonInfo::getRectOnTextureAtlasForTileType(int type)
{

}
