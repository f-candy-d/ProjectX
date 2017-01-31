#include "BitLayer.h"
#include <fstream>
#include <cmath>

USING_NS_CC;

/**
 * Protected functions
 */

BitLayer::BitLayer()
:_tileAtlasFile("")
,_isVisible(false)
,_isFailedLoadingFile(false)
,_tileSize(0,0)
,_bitFlag(-1)
,_spriteBatchNode(nullptr)
{}

BitLayer::~BitLayer()
{
	//release objects
	CC_SAFE_RELEASE_NULL(_spriteBatchNode);
}

bool BitLayer::initWithLayerInfoFile(std::string file)
{
	if(!parseLayerInfoFile(file))
	{
		//If initialization is failed...
		log("WARNING!! :: Loading '%s' was failed in BitLayer class.",file.c_str());
		_isFailedLoadingFile = true;
		return false;
	}

	// TileInfo info;
	// int c = 0;
	// for(auto itr = _tileInfoHashMap.begin(); itr != _tileInfoHashMap.end(); ++itr)
	// {
	// 	info = itr->second;
	// 	log("[TILE-INFO] :: X=%d	Y=%d	TILE_TYPE=%d",info.x,info.y,info.tileType);
	// 	c++;
	// }
	// log("!--%d tiles--!",c);
	// log("[IS-VISIBLE] :: %s",(_isVisible)?"true":"false");
	// log("[TILE-ATLAS-FILE] :: %s",_tileAtlasFile.c_str());
	// log("[TILE-SIZE] :: W=%d	H=%d",(int)_tileSize.width,(int)_tileSize.height);

	//Own SpriteBatchNode
	makeOwnSpriteBatchNode();

	return true;
}

/**
 * Private functions
 */

bool BitLayer::parseLayerInfoFile(std::string file)
{
	std::ifstream i_file_stream(file,std::ios::in);
	std::string line_buff;

	if(i_file_stream.fail())
		return false;

	// log("Read file :: %s",file.c_str());
	while(!i_file_stream.eof())
	{
		//Read by line.
		std::getline(i_file_stream,line_buff);
		// log("Line is '%s'",line_buff.c_str());
		//Parse the layer-info-file.
		if(line_buff.find("BEGIN_TILE_INFO") != (size_t)-1)
		{
			// log("in #BEGIN_TILE_INFO");
			//Make a TileInfo object.
			TileInfo info = {0,0,0-1};

			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			if(line_buff.find("X_COORDINATE") != (size_t)-1)
			{
				// log("in #X_COORIDINATE");
				//X coordinate.
				std::getline(i_file_stream,line_buff);
				// log("Line is '%s'",line_buff.c_str());
				sscanf(line_buff.c_str(),"%d",&info.x);
			} else
				return false;

			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			if(line_buff.find("Y_COORDINATE") != (size_t)-1)
			{
				// log("in #Y_COORIDINATE");
				//Y coordinate.
				std::getline(i_file_stream,line_buff);
				// log("Line is '%s'",line_buff.c_str());
				sscanf(line_buff.c_str(),"%d",&info.y);

			} else
				return false;

			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			if(line_buff.find("TILE_TYPE") != (size_t)-1)
			{
				// log("in #TILE_TYPE");
				//Tile-type
				std::getline(i_file_stream,line_buff);
				// log("Line is '%s'",line_buff.c_str());
				sscanf(line_buff.c_str(),"%d",&info.tileType);
			} else
				return false;

			if(info.tileType > -1)
			{
				_tileInfoHashMap[makeHashXYD(info.x,info.y)] = info;
			} else
				return false;

			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			if(line_buff.find("END_TILE_INFO") == (size_t)-1)
				return false;

		}
		else if(line_buff.find("IS_VISIBLE") != (size_t)-1)
		{
			// log("in #IS_VISIBLE");
			int b;
			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			sscanf(line_buff.c_str(),"%d",&b);
			_isVisible = static_cast<bool>(b);
		}
		else if(line_buff.find("TILE_ATLAS_FILE") != (size_t)-1)
		{
			// log("in #TILE_ATLAS_FILE");
			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			_tileAtlasFile = line_buff;
		}
		else if(line_buff.find("TILE_SIZE_W") != (size_t)-1)
		{
			// log("in #TILE_SIZE_W");
			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			sscanf(line_buff.c_str(),"%f",&_tileSize.width);
		}
		else if(line_buff.find("TILE_SIZE_H") != (size_t)-1)
		{
			// log("in #TILE_SIZE_H");
			std::getline(i_file_stream,line_buff);
			// log("Line is '%s'",line_buff.c_str());
			sscanf(line_buff.c_str(),"%f",&_tileSize.height);
		}
	}

	// log("End of paresing file.");

	return true;
}

int BitLayer::countDigitBinary(unsigned int n)
{
	int digit;

	if (n < 100000)
	{
		if (n < 1000)
		{
			if (n < 10)
			digit = 1;
			else if (n < 100)
			digit = 2;
			else
			digit = 3;
		}
		else
		{
			if (n < 10000)
			digit = 4;
			else
			digit = 5;
		}
	}
	else
	{
		if (n < 10000000)
		{
			if (n < 1000000)
			digit = 6;
			else
			digit = 7;
		}
		else
		{
			if (n < 100000000)
			digit = 8;
			else if (n < 1000000000)
			digit = 9;
			else
			digit = 10;
		}
	}

	return digit;
}

/**
 * This function will be only called in initWithLayerInfoFile() function.
 */
void BitLayer::makeOwnSpriteBatchNode()
{
	if(_isVisible && !_tileInfoHashMap.empty() && _spriteBatchNode == nullptr) {
		//Own SpriteBatchNode
		auto sprite_batch_node = SpriteBatchNode::create("tm2p5d/test_tile_atlas.png");
		this->setSpriteBatchNode(sprite_batch_node);
	}
}

/**
 * Public functions
 */

BitLayer* BitLayer::createWithLayerInfoFile(std::string file)
{
	BitLayer* ret = new BitLayer();
	if(ret->initWithLayerInfoFile(file))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

BitLayer::TileInfo BitLayer::getTileInfoAtGridPos(unsigned int x,unsigned int y)
{
	return _tileInfoHashMap[makeHashXYD(x,y)];
}

bool BitLayer::isOwnAnyTileAt(unsigned int x,unsigned int y)
{
	return (_tileInfoHashMap.find(makeHashXYD(x,y)) != _tileInfoHashMap.end());
}

bool BitLayer::isVisible()
{
	return _isVisible;
}

unsigned int BitLayer::makeHashXYD(unsigned int x,unsigned int y)
{
	int dig_y = countDigitBinary(y);
	return (x * (unsigned int)std::pow(10,dig_y) + y) * 10 + dig_y;
}

Vec2 BitLayer::decodeHashXYD(unsigned int hash)
{
	Vec2 vec = Vec2(-1,-1);
	int y_digit;
	int pow_10_y_dg;

	y_digit = hash % 10;
	hash /= 10;
	pow_10_y_dg = (int)std::pow(10,y_digit);
	vec.x = hash / pow_10_y_dg;
	vec.y = hash - vec.x * pow_10_y_dg;
	return vec;
}
