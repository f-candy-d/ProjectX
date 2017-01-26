#include "TiledMap2P5D.h"

USING_NS_CC;

/**
 * protected functions
 */
TiledMap2P5D::TiledMap2P5D()
{}

TiledMap2P5D::~TiledMap2P5D()
{}

bool TiledMap2P5D::initWithFile(std::string file)
{
	if(!Node::init())
		return false;

		return true;
}

/**
 * private functions
 */

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
