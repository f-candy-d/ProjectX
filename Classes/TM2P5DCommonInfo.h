#ifndef TM2P5D_COMMON_INFO_H
#define TM2P5D_COMMON_INFO_H

#include "cocos2d.h"
#include <string>

typedef std::string* StrArray;

/**
 * This class is Singleton class.
 */
class TM2P5DCommonInfo : public cocos2d::Ref
{
protected:
	TM2P5DCommonInfo();
	virtual ~TM2P5DCommonInfo();

private:
	int _numOfUnifiedLayers;
	int _numOfAllLayers;
	cocos2d::Size _gridSize;
	cocos2d::Size _tileSizePx;
	StrArray _allLayers;
	StrArray* _unifiledLayerHeads;
	std::unordered_map<std::string,int> _tileSheetNode;
	std::string _directory;
	bool _isLoadingFileFailed;

	/**
	 * Parse map-info file. If any error occer,
	 * stop parseing right now,and return false.
	 * @param  file [description]
	 * @return      [description]
	 */
	bool parseInfoFile(std::string file);

public:
	/**
	 * [getInstence description]
	 * @return [description]
	 */
	static TM2P5DCommonInfo* getInstance();

	/**
	 * Initialize shared instance with map-info file.
	 * If it is successful, return true,otherwise false.
	 * @param  file [description]
	 * @return      [description]
	 */
	bool initWithFile(std::string file);
};

#endif /* defined(TM2P5DCommonInfo) */
