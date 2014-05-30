#include "HelloWorldScene.h"

USING_NS_CC;

//使おうかなと思ったけど、また今度考えよう。
enum TouchType{
    TOUCH_TAP,
    TOUCH_DOUBLE_TAP,
    TOUCH_SWIPE,
    TOUCH_2_FINGER_TAP,
    TOUCH_2_FINGER_DOUBLE_TAP,
    TOUCH_2_FINGER_SWIPE
};

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
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->setEnabled(true);
    
    listener->onTouchesBegan = [this](std::vector<Touch*> touches, Event *event){
        //タッチが2カ所を検知されたら
        if (touches.size() == 2){
            _touch_type = TOUCH_2_FINGER_TAP;
        }
        
        //タッチが1カ所しかなかったら
        if (touches.size() == 1){
            
            // ここはそこしハックになるが、タッチ1カ所しか検出されなくても、
            // 実際ユーザーが指二本でタッチしようとしても、微妙なずれがあるので、
            // 許容範囲内なら二本指タッチとします
            if (_touchTime != 0.0f &&
                _timer2 - _touchTime < 0.05f) {
                _touch_type = TOUCH_2_FINGER_TAP;
                return;
            }
            
            _touch_type = TOUCH_TAP;
        }
        //タッチ数を保存する、これはダブルタップ判定に使う。
        _num_touches += 1;
        _touchTime = _timer2;
    };
    
    listener->onTouchesMoved = [this](std::vector<Touch*> touches, Event *event){
        _swipe_type = 0;
        //移動したら、スワイプとします。
        if (touches.front()->getLocation().x - touches.front()->getPreviousLocation().x > 10.0f)
        {
            _swipe_type = 1;
        }
    };

    listener->onTouchesEnded = [this](std::vector<Touch*> touches, Event *event){
        //CCLOG("Touch Num When Touch End: %lu", touches.size());
        if (_touch_type == TOUCH_2_FINGER_TAP) {
            if (_swipe_type != 1) {
                this->unschedule(schedule_selector(HelloWorld::TwoFingerTap));
                // タイマーをクリア
                if (_num_touches >= 2) {
                    CCLOG("2 finger double touch");
                    
                    //タッチ数とタッチ時間をリセット。
                    _num_touches = 0;
                    _touchTime = 0;
                    
                } else {
                    this->scheduleOnce(schedule_selector(HelloWorld::TwoFingerTap), .2f);
                    //タップした場合は、0.2秒の遅延を設定。もし次のタップが0.2秒の間に発生したら、
                    //このタイマーが消される
                }
            } else {
                CCLOG("2 finger swipe");
            }
        }
        
        if (_touch_type == TOUCH_TAP) {
            if (_swipe_type != 1) {
                this->unschedule(schedule_selector(HelloWorld::Tap));
                if (_num_touches >= 2) {
                    CCLOG("double tap");
                    _num_touches = 0;
                    _touchTime = 0;
                } else {
                    this->scheduleOnce(schedule_selector(HelloWorld::Tap), .2f);
                }
            } else {
                CCLOG("swipe");
            }
        }
        _swipe_type = 0;
        _timer = 0;
    };

    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, 99999);
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float delta)
{
    _timer2 += delta;
    _timer += delta;
    if (_timer > 0.5f) {
        _num_touches = 0;
        _timer = 0;
    }
}

void HelloWorld::Tap(float delta)
{
    CCLOG("tap");
    _touchTime = 0.0f;
}

void HelloWorld::TwoFingerTap(float delta)
{
    CCLOG("2 finger tap");
    _touchTime = 0.0f;
}

