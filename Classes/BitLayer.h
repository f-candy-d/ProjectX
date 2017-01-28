#ifndef BIT_LAYER_H
#define BIT_LAYER_H

#include "../cocos2d/cocos/cocos2d.h"
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
	std::string _tileAtlasFile;
	bool _isVisible;
	bool _isFailedLoadingFile;
	cocos2d::Size _tileSize;
	CC_SYNTHESIZE(int,_bitFlag,BitFlag);
	CC_SYNTHESIZE_RETAIN(cocos2d::SpriteBatchNode*,_spriteBatchNode,SpriteBatchNode);

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

	/**
	 * [makeOwnSpriteBatchNode description]
	 */
	void makeOwnSpriteBatchNode();

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
	 * If this layer has any tile at (x,y) ,return true.
	 * Otherwise return false.
	 * @param  x [x position on grid cooridinate]
	 * @param  y [y position on grid cooridinate]
	 * @return   [ture or false]
	 */
	bool isOwnAnyTileAt(unsigned int x,unsigned int y);

	bool isVisible();

};

#endif /* defined(BIT_LAYER_H) */
