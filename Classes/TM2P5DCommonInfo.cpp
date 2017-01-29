#include "TM2P5DCommonInfo.h"
#include <fstream>

USING_NS_CC;

#define MAX_DIRECTORY_NAME_LEN 10
#define MAX_FILE_NAME_LEN 30

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
,_isSuccessedInit(false)
{}

TM2P5DCommonInfo::~TM2P5DCommonInfo()
{
	delete _sharedInstance;
}

/**
 * Private functions
 */

void TM2P5DCommonInfo::logForDebug()
{
	// log("numOfUnifiedLayers = %zu",_numOfUnifiedLayers);
	// log("numOfAllLayers = %zu",_numOfAllLayers);
	// log("gridSize W = %d H = %d",(int)_gridSize.width,(int)_gridSize.height);
	// log("tileSizePx W = %d H = %d",(int)_tileSizePx.width,(int)_tileSizePx.height);
	// log("directory = %s",_directory.c_str());
	// log("allLayers::");
	// for(auto itr = _allLayers.begin();itr != _allLayers.end();++itr)
	// {
	// 	log("	[%s]",itr->c_str());
	// }
	// log("unifiedLayersHeads::");
	// for(auto itr = _unifiledLayerHeads.begin();itr != _unifiledLayerHeads.end();++itr)
	// {
	// 	log("	[%s]",(*itr)->c_str());
	// }
	// log("tileSheetNode::");
	// for(auto itr = _tileSheetNode.begin();itr != _tileSheetNode.end();++itr)
	// {
	// 	log("key[%s] value[%zu]",itr->first.c_str(),itr->second);
	// }
	// log("isSuccessedInit = %s",(_isSuccessedInit)?"true":"false");
}

bool TM2P5DCommonInfo::parseInfoFile(std::string file)
{
	std::ifstream i_file_stream(file,std::ios::in);
	std::string line_buff;

	if(i_file_stream.eof())
		return false;

	// log("Let's start to parse '%s'",file.c_str());
	//read line as:
	// std::getline(i_file_stream,line_buff);
	//find the string in another string as:
	// (line_buff.find("STR") != (size_t)-1)
	// If this is true,it means that find "STR" in the string.
	while(!i_file_stream.eof())
	{
		// log("!!!!!!!!![Top Of Loop]!!!!!!!!");
		//Read by line.
		std::getline(i_file_stream,line_buff);
		// log("line is -> [%s]",line_buff.c_str());
		if(line_buff.find("NUM_UNIFIED_LAYERS") != (size_t)-1)
		{
			//The number of unified layers.
			std::getline(i_file_stream,line_buff);
			// log("line is -> [%s]",line_buff.c_str());
			sscanf(line_buff.c_str(),"%zu",&_numOfUnifiedLayers);
			//set capacity
				// _unifiledLayerHeads.reserve(3);
				// log("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE[%d]",_numOfUnifiedLayers);
		}
		else if(line_buff.find("NUM_ALL_LAYERS") != (size_t)-1)
		{
			//The number of all bit layers.
			std::getline(i_file_stream,line_buff);
			// log("line is -> [%s]",line_buff.c_str());
			sscanf(line_buff.c_str(),"%zu",&_numOfAllLayers);
			//set capacity to avoid be killed iterator when call vector.pushBack().
			if(_numOfAllLayers > 0)
				_allLayers.reserve(_numOfAllLayers);
		}
		else if(line_buff.find("MAP_GRID_SIZE_W_H") != (size_t)-1)
		{
			//The size of grid of the map.
			std::getline(i_file_stream,line_buff);
			// log("line is -> [%s]",line_buff.c_str());
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
			// log("line is -> [%s]",line_buff.c_str());
			if(line_buff.size() < MAX_DIRECTORY_NAME_LEN)
			{
				sscanf(line_buff.c_str(),"%s",tmp);
				_directory = tmp;
			}
		}
		else if(line_buff.find("BEGIN_HIERARCHY") != (size_t)-1)
		{
			std::getline(i_file_stream,line_buff);
			// log("line is -> [%s]",line_buff.c_str());
			while(line_buff.find("BEGIN_UNIFIED_LAYER") != (size_t)-1)
			{
				int countCurrentLayer = 0;
				std::getline(i_file_stream,line_buff);
				// log("line is -> [%s]",line_buff.c_str());

				while(line_buff.find("BIT_LAYER") != (size_t)-1)
				{
					char tmp[MAX_DIRECTORY_NAME_LEN];
					countCurrentLayer++;
					//The bit layer name.
					std::getline(i_file_stream,line_buff);
					// log("line is -> [%s]",line_buff.c_str());
					if(line_buff.size() < MAX_FILE_NAME_LEN)
					{
						sscanf(line_buff.c_str(),"%s",tmp);
						_allLayers.push_back(std::string(tmp));
					}
					else
					return false;

					//next line
					std::getline(i_file_stream,line_buff);
				}

				// log("line is -> [%s]",line_buff.c_str());
				if(line_buff.find("END_UNIFIED_LAYER") != (size_t)-1 && countCurrentLayer > 0)
				{
					auto itr = _allLayers.end();
					itr -= countCurrentLayer;
					// log("itr->[%s]",itr->c_str());
					_unifiledLayerHeads.push_back(itr);
					// log("wwwwwwwwwwwwwwwwwwwwwwwww->[%s]",_unifiledLayerHeads[0]->c_str());
				}
				else
					return false;
			//next line
			std::getline(i_file_stream,line_buff);
			}

			// log("line is -> [%s]",line_buff.c_str());
			if(line_buff.find("END_HIERARCHY") == (size_t)-1)
				return false;
		}
		else if(line_buff.find("BEGIN_TILE_SHEET") != (size_t)-1)
		{
			std::getline(i_file_stream,line_buff);
			// log("line is -> [%s]",line_buff.c_str());
			if(line_buff.find("FILE_NAME") != (size_t)-1)
			{
				//Tile sheet file name.
				char tmp[MAX_FILE_NAME_LEN];

				std::getline(i_file_stream,line_buff);
				// log("line is -> [%s]",line_buff.c_str());
				// log("length is %d",line_buff.size());
				if(line_buff.size() < MAX_FILE_NAME_LEN)
				{
					sscanf(line_buff.c_str(),"%s",tmp);

					std::getline(i_file_stream,line_buff);
					// log("line is -> [%s]",line_buff.c_str());
					if(line_buff.find("NUM_TILES") != (size_t)-1)
					{
						size_t nt;
						std::getline(i_file_stream,line_buff);
						// log("line is -> [%s]",line_buff.c_str());
						sscanf(line_buff.c_str(),"%zu",&nt);
						//Add to the map
						_tileSheetNode[tmp] = nt;
						// log("nt = %d",nt);
					}
				}
				else
					return false;
			}

			std::getline(i_file_stream,line_buff);
			// log("line is -> [%s]",line_buff.c_str());
			if(line_buff.find("END_TILE_SHEET") == (size_t)-1)
			return false;
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
	if(!(_isSuccessedInit = parseInfoFile(file)))
		return false;

		//For debug
		logForDebug();

	return true;
}

StrVector TM2P5DCommonInfo::getBitLayerNamesInUnifiedLayer(size_t index)
{
	StrVector v;

	if(index < _numOfUnifiedLayers)
	{
		for(auto itr = _unifiledLayerHeads[index];
			(index == _numOfAllLayers - 1)
			? (itr != _allLayers.end())
			: (itr != _allLayers.end() && itr != _unifiledLayerHeads[index + 1]);
			++itr)
			{
				v.push_back(*itr);
			}
	}

	//Return v using move-semantics
	return std::move(v);
}

const StrVector& TM2P5DCommonInfo::getAllBitLayerNames()
{
	return _allLayers;
}

unsigned int TM2P5DCommonInfo::getNumOfTilesForTileSheetName(std::string name)
{
	return _tileSheetNode[name];
}
