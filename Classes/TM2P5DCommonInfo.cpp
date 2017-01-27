#include "TM2P5DCommonInfo.h"
#include <fstream>

USING_NS_CC;

#define MAX_DIRECTORY_NAME_LEN 10

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
,_directory("")
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
	//find the string in another string as:
	// (line_buff.find("STR") != (size_t)-1)
	// If this is true,it means that find "STR" in the string.
	while(!i_file_stream.eof())
	{
		//Read by line.
		std::getline(i_file_stream,line_buff);

		if(line_buff.find("NUM_UNIFIED_LAYERS") != (size_t)-1)
		{
			//The number of unified layers.
			std::getline(i_file_stream,line_buff);
			sscanf(line_buff.c_str(),"%d",&_numOfUnifiedLayers);
		}
		else if(line_buff.find("NUM_ALL_LAYERS") != (size_t)-1)
		{
			//The number of all bit layers.
			std::getline(i_file_stream,line_buff);
			sscanf(line_buff.c_str(),"%d",&_numOfAllLayers);
		}
		else if(line_buff.find("MAP_GRID_SIZE_W_H") != (size_t)-1)
		{
			//The size of grid of the map.
			std::getline(i_file_stream,line_buff);
			sscanf(line_buff.c_str(),"%f%f",&_gridSize.width,&_gridSize.height);
		}
		else if(line_buff.find("TILE_SIZE_PIXEL_W_H") != (size_t)-1)
		{
			//The size of a tile (pixel).
			std::getline(i_file_stream,line_buff);
			sscanf(line_buff.c_str(),"%f%f",&_tileSizePx.width,&_tileSizePx.height);
		}
		else if(line_buff.find("DIRECTORY") != (size_t)-1)
		{
			char tmp[MAX_DIRECTORY_NAME_LEN];
			//The directory name that map-ino files are there.
			std::getline(i_file_stream,line_buff);
			if(line_buff.size() < MAX_DIRECTORY_NAME_LEN)
			{
				sscanf(line_buff.c_str(),"%*[^ ]%s",tmp);
				_directory = tmp;
			}
		}
		else if(line_buff.find("BEGIN_HIERARCHY") != (size_t)-1)
		{

		}
		else if(line_buff.find("BEGIN_TILE_SHEET") != (size_t)-1)
		{

		}
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
