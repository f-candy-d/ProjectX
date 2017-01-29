#ifndef TILED_MAP_2P5D_H
#define TILED_MAP_2P5D_H

/**
 * Note::
 *
 * The default position of this node is (0,0).
 */

#include "../cocos2d/cocos/cocos2d.h"
#include "UnifiedLayer.h"
#include <string>

typedef std::function<void(cocos2d::Touch*, cocos2d::Event*)> ccTouchCallback;

class TiledMap2P5D : public cocos2d::Node
{
protected:
	TiledMap2P5D();
	virtual ~TiledMap2P5D();
	/**
	 * [initWithFile description]
	 * @param  file [This must be map_info.dat]
	 * @return      [description]
	 */
	bool initWithFile(std::string file);

private:
	cocos2d::Vector<UnifiedLayer*> _layers;
	cocos2d::Rect _visibleRect;
	UnifiedLayer::GridRect _drawnGridRect;

	/**
	 * [addUnifiedLayer description]
	 * @method addUnifiedLayer
	 * @param  index           [description]
	 * @return                 [description]
	 */
	bool addUnifiedLayer(size_t index);

	/**
	 * [resizeDrawnRect description]
	 * @method resizeDrawnRect
	 */
	void resizeDrawnRect();

public:
	/**
	 * [create description]
	 * @param  file [This must be map_info.dat]
	 * @return      [description]
	 */
	static TiledMap2P5D* create(std::string file);

	/**
	 * [getLambdaOnTouchMoved description]
	 * @method getLambdaOnTouchMoved
	 * @return [description]
	 */
	ccTouchCallback getLambdaOnTouchMoved();
};

#endif /* defined(TILED_MAP_2P5D_H) */
