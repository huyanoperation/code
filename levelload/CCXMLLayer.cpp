#include "CCXMLLayer.h"
#include "AdvanceSprite.h"
#include "GameConstant.h"

//--------------------------------------------------------------------------------------------------------------------
//
//
CCMenu* CreatGameUIButton( string filePath, SEL_MenuHandler callback, int Id, CCObject *target, CCPoint pos )
{
	CCSprite *spriteNormal = CCSprite::create( filePath.c_str() );
	CCSprite *spriteSelected = CCSprite::create( filePath.c_str() );
	spriteSelected->setColor(ccc3( 100,100,100 ));

	CCMenuItemSprite* item1 = CCMenuItemSprite::create(spriteNormal, spriteSelected, target, callback );
	item1->setTag( Id );


	CCMenu* pMenu = CCMenu::create( item1, NULL);
	pMenu->setPosition( pos );
	return pMenu;  
}
//--------------------------------------------------------------------------------------------------------------------
//
//
CCMenu* CreatGameUIButtonWithSprite( CCSprite* pSprite, SEL_MenuHandler callback, int Id, CCObject *target, CCPoint pos )
{
	CCSprite *spriteNormal = new CCSprite();
	spriteNormal->initWithTexture(pSprite->getTexture());
	spriteNormal->autorelease();
    
	CCSprite *spriteSelected = new CCSprite();
	spriteSelected->initWithTexture(spriteNormal->getTexture());
	spriteSelected->autorelease();
    
	spriteSelected->setColor(ccc3( 100,100,100 ));
    
	CCMenuItemSprite* item1 = CCMenuItemSprite::create(spriteNormal, spriteSelected, target, callback );
	item1->setTag( Id );
        
	CCMenu* pMenu = CCMenu::create( item1, NULL);
	pMenu->setPosition( pos );
	return pMenu;  
}
//----------------------------------------------------------------------------------------------------------------------
//
//
CCControlButton *CreatGameUIButtonWith9Sprite( string path, SEL_CCControlHandler callback, int Id, CCObject *target, CCPoint pos, string text )
{
    // Add the button
    CCScale9Sprite *backgroundButton = CCScale9Sprite::create(path.c_str());
    CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create(path.c_str());
    
    CCLabelTTF *titleButton = CCLabelTTF::create(text.c_str(), "Marker Felt", 30);
    
    CCControlButton *controlButton = CCControlButton::create(titleButton, backgroundButton);
    controlButton->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
    controlButton->setTitleColorForState(ccWHITE, CCControlStateHighlighted);
    controlButton->setAnchorPoint(ccp(0.5f, 1));
    controlButton->setPosition(pos);
    
    
    controlButton->addTargetWithActionForControlEvent(target, callback, CCControlEventTouchUpInside);
    
    return controlButton;
}

CCXMLLayer::CCXMLLayer()
{
	
}//------------------------------------------------------
//
//
CCXMLLayer::~CCXMLLayer()
{
	m_vNodeObject.clear();
}
//---------------------------------------------------------------
//
//
void CCXMLLayer::onEnter()
{
	CCLayer::onEnter();
}
//----------------------------------------------------------------
//
//
void CCXMLLayer::onExit()
{
	CCLayer::onExit();
}
//----------------------------------------------------------------
//
//
//const char * CCXMLLayer::valueForKey(const char *key, CCDictionary<std::string, CCObject*> *dict)
//{
//	if (dict)
//	{
//		CCString *pString = (CCString*)dict->objectForKey(std::string(key));
//		return pString ? pString->m_sString.c_str() : "";
//	}
//	return "";
//}
//-------------------------------------------------------------------
//
//
void CCXMLLayer::TurnitorqueWorldToCoco2d( float &x, float &y, float &layer )
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	x += ( s.width * CC_CONTENT_SCALE_FACTOR() ) / 2;
	y += ( s.height * CC_CONTENT_SCALE_FACTOR() ) / 2;

	y = s.height * CC_CONTENT_SCALE_FACTOR() - y;

	layer = MAXTORQUELAYERNUM - layer;
}
//----------------------------------------------------------------
//
//
CCNode* CCXMLLayer::GetXMLNodeFromKey( string key )
{
	for( int i = 0; i < m_vNodeObject.size(); i++ )
	{
		string *strKey = ( string* )m_vNodeObject[i]->getUserData();
		if( 0 == strcmp( strKey->c_str(), key.c_str() ) )
		{
			return m_vNodeObject[i];
		}
	}
	return NULL;
}
//-----------------------------------------------------------------
//
//
void CCXMLLayer::RemoveNode( CCNode* pNode )
{
	for(vector<CCNode*>::iterator it=m_vNodeObject.begin(); it!=m_vNodeObject.end(); it++)
	{
		if( *it== pNode )
		{
			m_vNodeObject.erase( it );
			break;
		}
	}

	removeChild( pNode, true );
}
//-----------------------------------------------------------------
//
//
void CCXMLLayer::LoadPlist( const char *pList )
{
	string strPath = GetGameLevelPath();
	strPath = strPath + pList;

    const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(strPath.c_str());
    
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);
    CCDictionary *imagesDict = (CCDictionary*)dict->objectForKey("images");

	m_vNodeObject.clear();
    
    CCDictElement* pElement = NULL;
   
    CCDICT_FOREACH(imagesDict, pElement)
	{
        
        std::string key = pElement->getStrKey();
        CCDictionary* imageDict = (CCDictionary*)pElement->getObject();
        
		float x = imageDict->valueForKey("x")->floatValue();
		float y = imageDict->valueForKey("y")->floatValue();
		float w = imageDict->valueForKey("width")->floatValue();
		float h = imageDict->valueForKey("height")->floatValue();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(  2 == CC_CONTENT_SCALE_FACTOR() || CCApplication::sharedApplication().isIpad() )
        {
            
        }
        else
        {
            x = x / 2;
            y = y / 2;
            w = w / 2;
            h = h / 2;
        }
        #endif    
            
		float layer = imageDict->valueForKey("layer")->floatValue();
		bool flipX = imageDict->valueForKey("FlipX")->boolValue();
		bool flipY = imageDict->valueForKey("FlipY")->boolValue();
		TurnitorqueWorldToCoco2d( x, y, layer );

		if( strstr( key.c_str(), "t2dSceneObject_") )
		{
			CCNode *pNode = CCNode::create();
			pNode->setPosition( ccp( x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR() ) );
			CCSize size( w / CC_CONTENT_SCALE_FACTOR(), h / CC_CONTENT_SCALE_FACTOR() );
			pNode->setContentSize( size );
			
			string *pNameKey = new string( key );
			pNode->setUserData( pNameKey );

			addChild( pNode, (int)layer );
			m_vNodeObject.push_back( pNode );
		}
		else if( strstr( key.c_str(), "t2dAnimatedSprite_") )
		{
			AdvanceSprite *m_pAnimation = new AdvanceSprite();
			
			string plist = imageDict->valueForKey("plist")->getCString();
			m_pAnimation->addFramesFromiT2D( plist.c_str() );
            m_pAnimation->autorelease();
			m_pAnimation->setPosition( ccp( x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR() ) );
			m_pAnimation->setScaleX( ( w / CC_CONTENT_SCALE_FACTOR() ) / m_pAnimation->getContentSize().width );
			m_pAnimation->setScaleY( ( h / CC_CONTENT_SCALE_FACTOR() ) / m_pAnimation->getContentSize().height );
			m_pAnimation->setFlipX( flipX );
			m_pAnimation->setFlipY( flipY );
		    string *pNameKey = new string( key );
			m_pAnimation->setUserData( pNameKey );
			

			int startFrameIndex = imageDict->valueForKey("startframe")->intValue();
			int endFrameIndex   = imageDict->valueForKey("endframe")->intValue();
			float time = imageDict->valueForKey("animationtime")->floatValue();

			m_pAnimation->startAnimation( startFrameIndex, endFrameIndex, -1, NULL, this, (float)( ( endFrameIndex - startFrameIndex + 1 ) / time ), false, false  );
			addChild( m_pAnimation, (int)layer );

			m_vNodeObject.push_back( m_pAnimation );
		}
		else
		{
			const char *image = imageDict->valueForKey("image")->getCString();

			string ImagePath = GetGameImagesPath();
			ImagePath = ImagePath + image;
            
            CCTextureCache::sharedTextureCache()->addImage(ImagePath.c_str());

			CCSprite *pSprite = CCSprite::create( ImagePath.c_str() );
			pSprite->setPosition( ccp( x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR() ) );
			pSprite->setScaleX( ( w / CC_CONTENT_SCALE_FACTOR() ) / pSprite->getTextureRect().size.width );
			pSprite->setScaleY( ( h / CC_CONTENT_SCALE_FACTOR() ) / pSprite->getTextureRect().size.height );
			pSprite->setFlipX( flipX );
			pSprite->setFlipY( flipY );
		    string *pNameKey = new string( key );
			pSprite->setUserData( pNameKey );
			addChild( pSprite, (int)layer );

			m_vNodeObject.push_back( pSprite );
		}
	}
}

