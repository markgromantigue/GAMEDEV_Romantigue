#include "MainMenu.h"
#include "MainGame.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init()
{
	if(!Layer::init())
		return false;

	auto size = Director::getInstance()->getWinSize();

	auto background = Sprite::create("mainMenu.jpg");
	background->setPosition(Point(size.width/2, size.height/2));
	this->addChild(background);

	auto startButton = MenuItemFont::create("START", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	startButton->setTag(1);
	auto space = MenuItemFont::create(" ", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	space->setTag(2);
	auto menu = Menu::create(startButton, space, NULL);

	menu->setPosition(Point::ZERO);
	startButton->setPosition(Point(400, 80));
	//space->setPosition(Point(size.width*0.6, size.height*0.2));

	this->addChild(menu);
	return true;
}

void MainMenu::menuCallBack(Ref* obj)
{
	auto target = (Node *)obj;
	Scene * scene;

	switch (target->getTag())
	{
	case 1:
		scene = GameScene::createScene();
		break;
	default:
		break;
	}
	Director::getInstance()->replaceScene(scene);
}