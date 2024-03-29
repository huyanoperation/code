//
//  AdvanceSprite.cpp
//
//  Created by varadharaj on 16/05/11.
//  
//  Copyright 2011 __Sourcebits Technologies Pvt. Ltd., Bangalore.__. All rights reserved.
//

#include "AdvanceSprite.h"
#include "GameConstant.h"

namespace   cocos2d {
AdvanceSprite::AdvanceSprite()
:m_pfnSelectorDelegate(0)
,m_pTarget(0)
,m_isPlistLoaded(false)
,m_ElaspeTime(0.0)
,m_IncrementValue(0)
,m_isAnimationRunning(false)
{
   init();
}

AdvanceSprite::~AdvanceSprite()
{
    if(m_isPlistLoaded)
	{
		//m_AnimationFrames->removeAllObjects();
        //CC_SAFE_DELETE(m_AnimationFrames);
        m_AnimationFrames.clear();
	}
}

void AdvanceSprite::addFramesFromiT2D(const char *pList, int plistType)
{
	string path = GetGameImagesPath().c_str();
    string FulList = path + pList;
	addFrames( FulList.c_str() );

	//if( 1 == plistType )
	//{
	//	addFrames( FulList.c_str() );
	//	return;
	//}
    //m_isPlistLoaded = true;
    //m_AnimationFrames = new vector<CCSpriteFrame *>;
    
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(FulList.c_str());
    
    //const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(FulList.c_str());
    //CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);
    //CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");

    //framesDict->begin();
	//std::string keyTemp = pList;
	// remove .xxx
	//size_t startPos = keyTemp.find_last_of("."); 
	//keyTemp = keyTemp.erase(startPos);
	//string key = keyTemp + "1";
	//int count = 1;
    
	//while((CCDictionary<std::string, CCObject*>*)framesDict->objectForKey(key.c_str()))
	//{
    //    m_AnimationFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key.c_str()));
        
	//	char keyNumBuffer[128];
	//	sprintf( keyNumBuffer,"%d", ++count);
	//	key = keyTemp + keyNumBuffer;
    //}
    //setDisplayFrame(m_AnimationFrames->getObjectAtIndex(0)); 
	//setContentSize( m_AnimationFrames->getObjectAtIndex(0)->getRect().size );
}

void AdvanceSprite::addFrames(const char *pList)
{
    m_isPlistLoaded = true;
    //m_AnimationFrames = new vector<CCSpriteFrame *>;
        
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pList);
        
    const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pList);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);
    CCDictionary *framesDict = (CCDictionary*)dict->objectForKey("frames");
        
    std::string key = "";
    
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(framesDict, pElement)
    {
        //CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
        std::string spriteFrameName = pElement->getStrKey();
         m_AnimationFrames.push_back(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteFrameName.c_str()));
    }

    setDisplayFrame(m_AnimationFrames[0]);
    //setContentSize( m_AnimationFrames[0]->getRect().size );
    //setTextureRect( m_AnimationFrames[0]->getRect() ); s
}
    
void AdvanceSprite::addFrames(const char *pList, const char *textureFileName)
{
    m_isPlistLoaded = true;
//    m_AnimationFrames = new CCMutableArray<CCSpriteFrame *>;
//    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pList, textureFileName);
//    
//    const char *pszPath = CCFileUtils::fullPathFromRelativePath(pList);
//	CCDictionary<std::string, CCObject*> *dict = CCFileUtils::dictionaryWithContentsOfFile(pszPath);
//    CCDictionary<std::string, CCObject*> *framesDict = (CCDictionary<std::string, CCObject*>*)dict->objectForKey(std::string("frames"));
//    
//    framesDict->begin();
//	std::string key = "";
//	while((CCDictionary<std::string, CCObject*>*)framesDict->next(&key))
//	{
//        m_AnimationFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(key.c_str()));
//    }
//    setDisplayFrame(m_AnimationFrames->getObjectAtIndex(0)); 
}

void AdvanceSprite::addFrames(vector<cocos2d::CCSpriteFrame *> *frames)
{
    //m_AnimationFrames = frames;
    //setDisplayFrame(m_AnimationFrames->front());
}

void AdvanceSprite::addFrames(vector<cocos2d::CCSpriteFrame *> *frames, int displayTextureIndex)
{
    //m_AnimationFrames = frames;
    //setDisplayFrame(m_AnimationFrames->front());
}

void AdvanceSprite::increaseCurrentIndex()
{
    if(m_AnimationMode && (m_EndingIndex - m_CurrentIndex) != 0)
        m_IncrementValue += (m_EndingIndex - m_CurrentIndex) / abs(m_EndingIndex - m_CurrentIndex);
    else if((m_StartingIndex - m_CurrentIndex) != 0)
        m_IncrementValue += (m_StartingIndex - m_CurrentIndex) / abs(m_StartingIndex - m_CurrentIndex);
}

void AdvanceSprite::update(float dt)
{
    m_ElaspeTime += dt;
    while (m_ElaspeTime >= m_FrameRate) 
    {
        m_ElaspeTime -= m_FrameRate;
        
        m_CurrentIndex += m_IncrementValue;
        setDisplayFrame(m_AnimationFrames[m_CurrentIndex]);
             
         //Forward Animation....
        if (m_CurrentIndex == m_EndingIndex) 
        {
            if(m_NeedToRunReverseAnimation)
            {
                m_AnimationMode = false;
                increaseCurrentIndex();
                return;
            }
            
            m_RunningLoop++;
            if(m_RunningLoop == m_NumberOfLoop)
            {
                stopAnimaiton();
                //Raising Callback.
                if(m_pTarget != 0 && m_pfnSelectorDelegate != 0)
                    (m_pTarget->*m_pfnSelectorDelegate)();
                
                //Remove Object by Itself.
                if(m_NeedToDeleteItself)
                    removeObjectItself();
                return;
            }
            else
            {
                m_CurrentIndex = m_StartingIndex;
            }
        }
        
        //Reverse Animation...
        else if(m_CurrentIndex == m_StartingIndex)
        {
            m_RunningLoop++;
            if(m_RunningLoop == m_NumberOfLoop)
            {
                stopAnimaiton();
                //Raising Callback.
                if(m_pTarget != 0 && m_pfnSelectorDelegate != 0)
                    (m_pTarget->*m_pfnSelectorDelegate)();
                
                //Remove Object by Itself.
                if(m_NeedToDeleteItself)
                    removeObjectItself();
                return;
            }
            else
            {
                m_AnimationMode = true;
                increaseCurrentIndex();
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------
//
//
void AdvanceSprite::PlayAnimation( int *pframeData, int number_Loop, SEL_CallFunc pfnSelectorDelegate, CCObject *pTarget, bool NeedToRunReverseAnimation, bool NeedToDeleteItself  )
{
	startAnimation( pframeData[0], pframeData[1], number_Loop, pfnSelectorDelegate, pTarget, pframeData[2], NeedToRunReverseAnimation, NeedToDeleteItself  );
}
//-----------------------------------------------------------------------------------------------
//
//
void AdvanceSprite::startAnimation(int startInd, int endInd, int number_Loop, SEL_CallFunc pfnSelectorDelegate, cocos2d::CCObject *pTarget, int NumberOfFramesPerSecond, bool NeedToRunReverseAnimation, bool NeedToDeleteItself)
{
    if(m_isAnimationRunning)
    {
        stopAnimaiton();
        
        //Reset pointer.
        m_pfnSelectorDelegate = 0;
        m_pTarget = 0;
        m_ElaspeTime = 0.0f;
    }
    
    //Assign values.
    m_isAnimationRunning = true;
    
    m_StartingIndex = startInd - 1;
    m_EndingIndex = endInd;
    
    m_NumberOfLoop = number_Loop;
    
    m_pfnSelectorDelegate = pfnSelectorDelegate;
    m_pTarget = pTarget;
    if(NumberOfFramesPerSecond != -1)
        m_FrameRate = calculateFrameRate(NumberOfFramesPerSecond);
    
    //Initilize values....
    m_NeedToRunReverseAnimation = NeedToRunReverseAnimation;
    m_AnimationMode = true;
    m_CurrentIndex = m_StartingIndex;
    m_RunningLoop = 0;
    m_IncrementValue = 0;
    m_NeedToDeleteItself = NeedToDeleteItself;
    increaseCurrentIndex();
    this->scheduleUpdateWithPriority(-1);
    resumeAnimation();
}

void AdvanceSprite::stopAnimaiton()
{
    m_isAnimationRunning = false;
    this->unscheduleUpdate();
}

void AdvanceSprite::removeObjectItself()
{
	schedule( schedule_selector( AdvanceSprite::scheduleDelete ),0.2 );
}

void AdvanceSprite::scheduleDelete(float tt)
{
	this->removeFromParentAndCleanup(true);
}
}//namespace   cocos2d 