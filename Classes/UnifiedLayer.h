#ifndef UNIFIED_LAYER_H
#define UNIFIED_LAYER_H

#include "../cocos2d/cocos/cocos2d.h"
#include "BitLayer.h"

class UnifiedLayer : public cocos2d::Node
{
public:
	typedef struct gr {
		size_t x;
	 	size_t y;
		size_t width;
		size_t height;

		gr(size_t xp,size_t yp,size_t w,size_t h)
		{
			x = xp;
			y = yp;
			width = w;
			height = h;
		}
	} GridRect;

protected:
	UnifiedLayer();
	virtual ~UnifiedLayer();

	/**
	 * [initWithLayerIndex description]
	 * @param  index [description]
	 * @return       [description]
	 */
	bool initWithLayerIndex(size_t index);

private:
	cocos2d::Vector<BitLayer*> _bitLayers;
	cocos2d::Map<size_t,cocos2d::Sprite*> _tileSprites;
	int* _tiles;
	size_t _atIndex;

	/**
	 * [convertGridPosToLocalPos description]
	 * @method convertGridPosToLocalPos
	 * @param  x                        [description]
	 * @param  y                        [description]
	 * @param  tileSize                     [description]
	 * @return                          [description]
	 */
	cocos2d::Vec2 convertGridPosToLocalPos(unsigned int x,unsigned int y,const cocos2d::Size& tileSize);

	/**
	 * @param  oldTile [description]
	 * @param  x       [description]
	 * @param  y       [description]
	 * @return         [description]
	 */
	cocos2d::Sprite* reInitTileSprite(cocos2d::Sprite* oldTile,size_t x,size_t y);

public:
	/**
	 * [createWithLayerIndex description]
	 * @param  index [description]
	 * @return       [description]
	 */
	static UnifiedLayer* createWithLayerIndex(size_t index);

	/**
	 * [addBitLayer description]
	 * @param file [description]
	 * @param flag  [description]
	 */
	void addBitLayer(std::string file,int flag);

	/**
	 * [makeTileSpriets description]
	 * @method makeTileSpriets
	 * @param  makeGr          [description]
	 */
	void makeTileSpriets(GridRect makeGr);

	/**
	 * [optimizeTileSprites description]
	 * @param netMv [description]
	 */
	void optimizeTileSprites(const cocos2d::Vec2& netMv);
};

#endif /* defined(UNIFIED_LAYER_H) */
