#ifndef TILED_MAP_2P5D_H
#define TILED_MAP_2P5D_H

#include "cocos2d.h"
#include <string>

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

public:
	/**
	 * [create description]
	 * @param  file [This must be map_info.dat]
	 * @return      [description]
	 */
	static TiledMap2P5D* create(std::string file);
};

#endif /* defined(TILED_MAP_2P5D_H) */
