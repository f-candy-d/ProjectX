#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "UnifiedLayer.h"
#include "TiledMap2P5D.h"
#include "TM2P5DCommonInfo.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // // add "HelloWorld" splash screen"
    // auto sprite = Sprite::create("HelloWorld.png");
    //
    // // position the sprite on the center of the screen
    // sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //
    // // add the sprite as a child to this layer
    // this->addChild(sprite, 0);


    /**
     * test codes...
     */


    //test
    auto map = TiledMap2P5D::create("Resources/tm2p5d/map_info.dat");
    this->addChild(map);
    // auto unified_lahyer = UnifiedLayer::createWithLayerIndex(0);
    // int tag = 1 << 0;
    // unified_lahyer->addBitLayer("testLayer.dat",tag);
    // tag = 1 << 1;
    // unified_lahyer->addBitLayer("testLayer2.dat",tag);
    // unified_lahyer->makeTileSpriets();
    // this->addChild(unified_lahyer);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [map](Touch* touch,Event* event) {
        auto v = map->getPosition();
        log("position->(%f,%f)",v.x,v.y);
        return true;
    };
    listener->onTouchMoved = map->getLambdaOnTouchMoved();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
