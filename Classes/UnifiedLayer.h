#ifndef UNIFIED_LAYER_H
#define UNIFIED_LAYER_H

#include "cocos2d.h"

class UnifiedLayer public:cocos2d::Node
{
protected:

private:
	cocos2d::Vecotr<BitLayer*> bit_layers;

public:
};

#endif /* defined(UNIFIED_LAYER_H) */
