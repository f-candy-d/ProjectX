#ifndef isSuccessedInitH
#define TM2P5D_COMMON_INFO_H

#include "cocos2d.h"
#include <string>
#include <vector>

typedef std::vector<std::string> StrVector;
typedef std::vector<StrVector::iterator> ItrVector;

/**
 * This class is Singleton class.
 */
class TM2P5DCommonInfo : public cocos2d::Ref
{
protected:
	TM2P5DCommonInfo();
	virtual ~TM2P5DCommonInfo();

private:
	CC_SYNTHESIZE_READONLY(int,_numOfUnifiedLayers,NumOfUnifiedLayers);
	CC_SYNTHESIZE_READONLY(int,_numOfAllLayers,NumOfAllLayers);
	CC_SYNTHESIZE_READONLY(cocos2d::Size,_gridSize,GridSize);
	CC_SYNTHESIZE_READONLY(cocos2d::Size,_tileSizePx,TileSizePx);
	CC_SYNTHESIZE_READONLY(std::string,_directory,Directory);
	CC_SYNTHESIZE_READONLY(bool,_isSuccessedInit,IsSuccessedInit);
	StrVector _allLayers;
	ItrVector _unifiledLayerHeads;
	std::unordered_map<std::string,unsigned int> _tileSheetNode;

	/**
	 * Using for debug...
	 */
	void logForDebug();

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

	/**
	 * Return the names of bitlayers in specific UnifiedLayer.
	 * Use move-semantics when return vector object.
	 * @param  index [Index of specific UnifiedLayer is.]
	 * @return       [The names of BitLayers.]
	 */
	StrVector getBitLayerNamesInUnifiedLayer(int index);

	/**
	 * Return the names of all bitLayers in the TiledMap2P5D.
	 * @return [The names of all BitLayers.]
	 */
	const StrVector& getAllBitLayerNames();

	/**
	 * Return the number of the tiles in the specific tile sheet.
	 * @param  name [The name of specific tile sheet file.]
	 * @return      [The number of the tiles.]
	 */
	unsigned int getNumOfTilesForTileSheetName(std::string name);

};

#endif /* defined(TM2P5DCommonInfo) */
