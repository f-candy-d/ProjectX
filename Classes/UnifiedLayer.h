#ifndef UNIFIED_LAYER_H
#define UNIFIED_LAYER_H

#include "../cocos2d/cocos/cocos2d.h"
#include "BitLayer.h"

class UnifiedLayer : public cocos2d::Node
{
protected:
	UnifiedLayer();
	virtual ~UnifiedLayer();

	/**
	 * [initWithLayerIndex description]
	 * @param  index [description]
	 * @return       [description]
	 */
	bool initWithLayerIndex(int index);

private:
	cocos2d::Vector<BitLayer*> _bitLayers;
	int* _tiles;

	/**
	 * [convertGridPosToLocalPos description]
	 * @method convertGridPosToLocalPos
	 * @param  x                        [description]
	 * @param  y                        [description]
	 * @param  tileSize                     [description]
	 * @return                          [description]
	 */
	cocos2d::Vec2 convertGridPosToLocalPos(unsigned int x,unsigned int y,const cocos2d::Size& tileSize);

public:
	/**
	 * [createWithLayerIndex description]
	 * @param  index [description]
	 * @return       [description]
	 */
	static UnifiedLayer* createWithLayerIndex(int index);

	/**
	 * [addBitLayer description]
	 * @param file [description]
	 * @param flag  [description]
	 */
	void addBitLayer(std::string file,int flag);

	/**
	 * [makeTileSpriets description]
	 */
	void makeTileSpriets();
};

#endif /* defined(UNIFIED_LAYER_H) */
