#include "HelloWorldScene.h"

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->setEnabled(true);
    
    listener->onTouchesBegan = [this](std::vector<Touch*> touches, Event *event){
        if (touches.size() == 2){
            CCLOG("You just Touched by using 2 fingers");
        }
        
        if (touches.size() == 1){
            CCLOG("Timer : %f", _timer);
            if (_touchTime != 0.0f &&
                _timer - _touchTime < 0.3f) {
                if (_timer - _touchTime < 0.05f) {
                    CCLOG("You just Touched by using 2 fingers");
                    _touchTime = 0.0f;
                    return;
                }
                this->unschedule(schedule_selector(HelloWorld::Tap));
                CCLOG("You just did a double touch");
                _touchTime = 0.0f;
            } else {
                _touchTime = _timer;
                this->scheduleOnce(schedule_selector(HelloWorld::Tap), .2f);
            }
        }
    };
    
    listener->onTouchesMoved = [](std::vector<Touch*> touches, Event *event){
        //CCLOG("Touch Num When Move: %lu", touches.size());
    };

    listener->onTouchesEnded = [](std::vector<Touch*> touches, Event *event){
        //CCLOG("Touch Num When Touch End: %lu", touches.size());
    };

    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, 99999);
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float delta)
{
    _timer += delta;
}

void HelloWorld::Tap(float delta)
{
    if (_touchTime == 0.0f) return;
    CCLOG("You just tap");
    _touchTime = 0.0f;
}

