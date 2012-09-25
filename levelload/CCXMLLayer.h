#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d::extension;
using namespace cocos2d;
using namespace std;

CCMenu* CreatGameUIButton( string filePath, SEL_MenuHandler callback, int Id, CCObject *target, CCPoint pos );
CCMenu* CreatGameUIButtonWithSprite( CCSprite* pSprite, SEL_MenuHandler callback, int Id, CCObject *target, CCPoint pos );
CCControlButton *CreatGameUIButtonWith9Sprite( string path, SEL_CCControlHandler callback, int Id, CCObject *target, CCPoint pos, string text );

#define MAXTORQUELAYERNUM 100

class CCXMLLayer : public CCLayer
{
public:
	CCXMLLayer();
	~CCXMLLayer();

	void onEnter();
	void onExit();
	void LoadPlist( const char *pList );
	void TurnitorqueWorldToCoco2d( float &x, float &y, float &layer );
	//const char *valueForKey(const char *key, CCDictionary<std::string, CCObject*> *dict);
	vector< CCNode* > m_vNodeObject;
	CCNode* GetXMLNodeFromKey( string key );
	void RemoveNode( CCNode* pNode );

};