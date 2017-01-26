#include "TM2P5DCommonInfo.h"
#include <fstream>

USING_NS_CC;

/**
 * Shared instance.
 */
static TM2P5DCommonInfo* _sharedInstance = nullptr;

/**
 * Protected functions
 */

TM2P5DCommonInfo::TM2P5DCommonInfo()
:_numOfUnifiedLayers(0)
,_numOfAllLayers(0)
,_gridSize(0,0)
,_tileSizePx(0,0)
,_allLayers(nullptr)
,_unifiledLayerHeads(nullptr)
,_isLoadingFileFailed(false)
{}

TM2P5DCommonInfo::~TM2P5DCommonInfo()
{
	delete _sharedInstance;
}

/**
 * Private functions
 */

bool TM2P5DCommonInfo::parseInfoFile(std::string file)
{
	std::ifstream i_file_stream(file,std::ios::in);
	std::string line_buff;

	if(i_file_stream.eof())
		return false;

	//read line as:
	// std::getline(i_file_stream,line_buff);
	//find string as:If this is true,it means that find "STR" in the string.
	// (line_buff.find("STR") != (size_t)-1)
	while(!i_file_stream.eof())
	{
		//Read by line.
	}

	return true;
}

/**
 * Public functions
 */

TM2P5DCommonInfo* TM2P5DCommonInfo::getInstance()
{

	if(!_sharedInstance)
	{
		_sharedInstance = new TM2P5DCommonInfo();
	}

	return _sharedInstance;
}

bool TM2P5DCommonInfo::initWithFile(std::string file)
{
	if(!(_isLoadingFileFailed = parseInfoFile(file)))
		return false;

	return true;
}
