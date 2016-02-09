#include "GameOver.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameOver::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);
	return scene;
}

bool GameOver::init()
{
	if(!Layer::init())
		return false;

	SimpleAudioEngine::getInstance()->playBackgroundMusic("FX50.wav");

	auto size = Director::getInstance()->getWinSize();

	TTFConfig config("fonts/Halo3.ttf", 80);

	auto label = Label::createWithTTF(config, "GAME OVER");
	label->setPosition(Point(size.width/2, size.height/2));
	this->addChild(label);

	auto back = MenuItemFont::create("Back", CC_CALLBACK_1(GameOver::menuCallBack, this));
	auto menu = Menu::create(back, NULL);

	menu->setPosition(Point::ZERO);
	back->setPosition(Point(size.width*5/6, size.height/8));

	this->addChild(menu);

	return true;
}

void GameOver::menuCallBack(Ref* obj)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}