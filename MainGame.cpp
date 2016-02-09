#include "MainMenu.h"
#include "MainGame.h"
#include "Pause.h"
#include "GameOver.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
float speed = 0.3;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	speed = 0.3;
	if(!Layer::init())
		return false;

	
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Half_Bit.mp3", true);

	
	auto size = Director::getInstance()->getWinSize();

	auto background = Sprite::create("background.jpg");
	background->setPosition(Point(size.width/2, size.height/2));
	this->addChild(background);

	
	head = SpriteNode::create(1);
	head->setPositionRC(10, 7);
	this->addChild(head);

	newBody();
	newBody();

	
	hamster = SpriteNode::create(3);
	hamster->setPositionRC(5, 7);
	this->addChild(hamster);

	
	TTFConfig config("fonts/Marker Felt.ttf", 30); 

	auto labelScoreTitle = Label::createWithTTF(config, "Score");
	labelScoreTitle->setPosition(Point(100, 550));
	this->addChild(labelScoreTitle);
	
	score = 0;
	auto labelScore = Label::createWithTTF(config, "0");
	labelScore->setTag(110);
	labelScore->setPosition(Point(100, 520));
	this->addChild(labelScore);

	auto mainMenu = MenuItemFont::create("Menu", CC_CALLBACK_1(GameScene::menuCallBack, this));
	auto pause = MenuItemFont::create("Pause", CC_CALLBACK_1(GameScene::menuCallBack, this));
	mainMenu->setTag(1);
	pause->setTag(2);

	auto menu = Menu::create(pause, mainMenu, NULL);
	menu->setPosition(Point::ZERO);
	mainMenu->setPosition(Point(590, 548));
	pause->setPosition(Point(710, 548));
	
	this->addChild(menu);
	this->schedule(schedule_selector(GameScene::gameLogic), speed);

	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){
		switch (k)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			head->m_dir = ENUM_DIR::DIR_LEFT;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			head->m_dir = ENUM_DIR::DIR_RIGHT;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			head->m_dir = ENUM_DIR::DIR_UP;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			head->m_dir = ENUM_DIR::DIR_DOWN;
			break;
		default:
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void GameScene::menuCallBack(Ref* obj)
{
	auto target = (Node *)obj;
	Scene * scene;

	switch (target->getTag())
	{
	case 1:
		
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

		scene = MainMenu::createScene();
		Director::getInstance()->replaceScene(scene);
		break;
	case 2:
		
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

		scene = Pause::createScene();
		addChild(scene);
		Director::getInstance()->pause();
		break;
	default:
		break;
	}
	
}

void GameScene::gameLogic(float t)
{
	int row = head->m_row;
	int col = head->m_col;
	ENUM_DIR dir = head->m_dir;
	auto up = RotateTo::create(0,270);
	auto left = RotateTo::create(0,180);
	auto right = RotateTo::create(0, 0);
	auto down = RotateTo::create(0, 90);

	switch (head->m_dir)
	{
	case ENUM_DIR::DIR_UP:
		head->m_row++;
		head->setPositionRC(head->m_row, head->m_col);
		head->runAction(up);
		break;
	case ENUM_DIR::DIR_DOWN:
		head->m_row--;
		head->setPositionRC(head->m_row, head->m_col);
		head->runAction(down);
		break;
	case ENUM_DIR::DIR_LEFT:
		head->m_col--;
		head->setPositionRC(head->m_row, head->m_col);
		head->runAction(left);
		break;
	case ENUM_DIR::DIR_RIGHT:
		head->m_col++;
		head->setPositionRC(head->m_row, head->m_col);
		head->runAction(right);
		break;
	default:
		break;
	}

	
	moveBody(row, col, dir);


	if(head->m_row == hamster->m_row && head->m_col == hamster->m_col)
	{
		
		SimpleAudioEngine::getInstance()->playEffect("crunch.wav");

		
		this->score += 100;
		if (speed > 0.1){
			if ((score % 500) == 0){
				speed -= 0.05;
				this->schedule(schedule_selector(GameScene::gameLogic), speed);
			}
		}
		Label * label = (Label*)this->getChildByTag(110);
		char str_score[20];
		sprintf(str_score, "%d", score);
		label->setString(str_score);

		
		newBody();

		
		int flag = 0;
		int row;
		int col; 
		while(1)
		{
			row = (rand()%14)+1;
			col = (rand()%24)+1;
			for(auto pNode : allBody)
			{
				if(pNode->m_row == row && pNode->m_col == col)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0)
				break;
			flag = 0;
		}
		hamster->setPositionRC(row, col);
	}

	
	if(head->m_row < 1 || head->m_row > 14 || head->m_col < 1 || head->m_col > 24)
	{
		
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

		
		auto scene = GameOver::createScene();
		Director::getInstance()->replaceScene(scene);
	}

	
	for (auto bodyNode : allBody)
	{
		if(head->m_row == bodyNode->m_row && head->m_col == bodyNode->m_col)
		{
			
			SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

			
			auto scene = GameOver::createScene();
			Director::getInstance()->replaceScene(scene);
		}
	}
}


void GameScene::newBody()
{
	auto bodyNode = SpriteNode::create(2);
	if(allBody.size() > 0)
	{
		auto lastBody = allBody.at(allBody.size()-1);
		bodyNode->m_dir = lastBody->m_dir;

		switch(bodyNode->m_dir)
		{
		case ENUM_DIR::DIR_UP:
			bodyNode->setPositionRC(lastBody->m_row-1, lastBody->m_col);
			break;
		case ENUM_DIR::DIR_DOWN:
			bodyNode->setPositionRC(lastBody->m_row+1, lastBody->m_col);
			break;
		case ENUM_DIR::DIR_LEFT:
			bodyNode->setPositionRC(lastBody->m_row, lastBody->m_col+1);
			break;
		case ENUM_DIR::DIR_RIGHT:
			bodyNode->setPositionRC(lastBody->m_row, lastBody->m_col-1);
			break;
		default:
			break;
		}
	}
	else
	{
		bodyNode->m_dir = head->m_dir;
		switch(bodyNode->m_dir)
		{
		case ENUM_DIR::DIR_UP:
			bodyNode->setPositionRC(head->m_row-1, head->m_col);
			break;
		case ENUM_DIR::DIR_DOWN:
			bodyNode->setPositionRC(head->m_row+1, head->m_col);
			break;
		case ENUM_DIR::DIR_LEFT:
			bodyNode->setPositionRC(head->m_row, head->m_col+1);
			break;
		case ENUM_DIR::DIR_RIGHT:
			bodyNode->setPositionRC(head->m_row, head->m_col-1);
			break;
		default:
			break;
		}
	}
	this->addChild(bodyNode);
	allBody.pushBack(bodyNode);
}


void GameScene::moveBody(int row, int col, ENUM_DIR dir)
{
	if(allBody.size() > 0)
	{
		auto pBody = allBody.at(allBody.size()-1);
		pBody->setPositionRC(row, col);
		pBody->m_dir = dir;
		
		allBody.erase(allBody.size()-1);
		allBody.insert(0, pBody);
	}
}

SpriteNode* SpriteNode::create(int type)
{
	SpriteNode* pRet = new SpriteNode();
	if(pRet && pRet->init(type))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool SpriteNode::init(int type)
{
	if(!Sprite::init())
		return false;

	Sprite * sprite;

	switch(type)
	{
	case 1:
		sprite = Sprite::create("head.png");
		sprite->setAnchorPoint(ccp(0.5f, 0.5f));
		this->addChild(sprite);
		m_dir = ENUM_DIR::DIR_DOWN;
		break;
	case 2:
		sprite = Sprite::create("body.png");
		sprite->setAnchorPoint(ccp(0.5f, 0.5f));
		this->addChild(sprite);
		m_dir = ENUM_DIR::DIR_STOP;
		break;
	case 3:
		sprite = Sprite::create("hamster.png");
		sprite->setAnchorPoint(ccp(0.5f, 0.5f));
		this->addChild(sprite);
		m_dir = ENUM_DIR::DIR_STOP;
		break;
	default:
		break;
	}

	return true;
}



void SpriteNode::setPositionRC(int row, int col)
{
	this->m_row = row;
	this->m_col = col;
	this->setPosition(Point(col*32, row*32));
}