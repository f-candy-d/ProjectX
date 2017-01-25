#ifndef TM2P5D_COMMON_INFO_H
#define TM2P5D_COMMON_INFO_H

#include "cocos2d.h"
#include <unordered_map>
#include <string>

class TM2P5DCommonInfo : public cocos2d::Ref
{
protected:
	TM2P5DCommonInfo();
	virtual ~TM2P5DCommonInfo();
	bool initWithMapInfo(std::string file);

private:
	std::unordered_map<std::string,std::unordered_map<int,cocos2d::Rect>*> _atlasRectNode;

public:
	static TM2P5DCommonInfo* createWithMapInfo(std::string file);

	/**
	 * [getRectOnTextureAtlasForTileType description]
	 * @param  type [description]
	 * @return      [description]
	 */
	cocos2d::Rect getRectOnTextureAtlasForTileType(int type);
};

#endif /* defined(TM2P5D_COMMON_INFO_H) */
