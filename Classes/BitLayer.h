#ifndef BIT_LAYER_H
#define BIT_LAYER_H

#include "cocos2d.h"
#include <string>
#include <unordered_map>

typedef struct {
	unsigned int x;
	unsigned int y;
	int tileType;
} TileInfo;

class BitLayer : public cocos2d::Ref
{
protected:
	BitLayer();
	virtual ~BitLayer();
	bool initWithLayerInfoFile(std::string file);

private:
	std::unordered_map<unsigned int,TileInfo> _tileInfoHashMap;
	std::string _tile_atlas_file;
	bool _isVisible;
	cocos2d::Size _tileSize;

	/**
	 * [parseLayerInfoFile description]
	 * @param file [description]
	 */
	bool parseLayerInfoFile(std::string file);

	/**
	 * Count the number of digits of n using binary search.
	 * @param  n [target]
	 * @return   [the number of digits]
	 */
	int countDigitBinary(unsigned int n);

	/**
	 * [makeHashXYD description]
	 * @param  x [description]
	 * @param  y [description]
	 * @return   [description]
	 */
	unsigned int makeHashXYD(unsigned int x,unsigned int y);

	/**
	 * [decodeHashXYD description]
	 * @param  hash [description]
	 * @return      [description]
	 */
	cocos2d::Vec2 decodeHashXYD(unsigned int hash);

public:
	/**
	 * [createWithLayerInfoFile description]
	 * @param  file [description]
	 * @return      [description]
	 */
	static BitLayer* createWithLayerInfoFile(std::string file);

	/**
	 * [getTileInfoAtGridPos description]
	 * @param  x [description]
	 * @param  y [description]
	 * @return   [description]
	 */
	TileInfo getTileInfoAtGridPos(unsigned int x,unsigned int y);

	/**
	 * [getSpriteBatchNode description]
	 * @return [description]
	 */
	cocos2d::SpriteBatchNode* getSpriteBatchNode();
};

#endif /* defined(BIT_LAYER_H) */
