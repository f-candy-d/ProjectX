#ifndef UNIFIED_LAYER_H
#define UNIFIED_LAYER_H

#include "cocos2d.h"
#include "BitLayer.h"

class UnifiedLayer : public cocos2d::Node
{
protected:
	UnifiedLayer();
	virtual ~UnifiedLayer();
	/**
	 * Init this class using this function.
	 * @param  directory [The path to the directory where the files of the map are there]
	 * @return           [true or false]
	 */
	bool initWithMapInfo(std::string directory,cocos2d::Size map_size);

private:
	cocos2d::Vector<BitLayer*> _bitLayers;
	cocos2d::Size _mapSize;
	std::string _directory;
	int* _tiles;

public:

	/**
	 * Init this class using this function.
	 * @param  directory [The path to the directory where the files of the map are there]
	 * @return           [A pointer to UnifiedLayer object]
	 */
	static UnifiedLayer* createWithMapInfo(std::string directory,cocos2d::Size map_size);

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
