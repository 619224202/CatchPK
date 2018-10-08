#include "CTiledLayer.h"

using namespace cocos2d;
using namespace common;

common::CTiledLayer::CTiledLayer()
{
	m_size = CCDirector::sharedDirector()->getWinSize();
}

CTiledLayer* common::CTiledLayer::create( const char* _filePath )
{
	CTiledLayer* pTiledLayer = new CTiledLayer;
	pTiledLayer->initWithTMXFile(_filePath);
	pTiledLayer->autorelease();
	return pTiledLayer;
}

CTiledLayer* common::CTiledLayer::createWithSize( const char* _filePath, cocos2d::CCSize _size )
{
	CTiledLayer* pTiledLayer = new CTiledLayer;
	pTiledLayer->initWithTMXFile(_filePath);
	pTiledLayer->autorelease();
	pTiledLayer->m_size = _size;
	return pTiledLayer;
}

void common::CTiledLayer::visit()
{
	if (!m_bVisible)
	{
		return;
	}
	kmGLPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
	}

	this->transform();

	CCNode* pNode = NULL;
	unsigned int i = 0;

	if(m_pChildren && m_pChildren->count() > 0)
	{
		// draw children zOrder < 0
		ccArray *arrayData = m_pChildren->data;
		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];

			if ( pNode && pNode->getZOrder() < 0 && InVisible(pNode) ) 
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
		// self draw
		this->draw();

		for( ; i < arrayData->num; i++ )
		{
			pNode = (CCNode*) arrayData->arr[i];
			if ( pNode && InVisible(pNode) )
			{
				pNode->visit();
			}
		}        
	}
	else
	{
		this->draw();
	}

	// reset for next frame
	m_uOrderOfArrival = 0;

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	kmGLPopMatrix();
}

bool common::CTiledLayer::InVisible(cocos2d::CCNode* pNode)
{
	CCPoint anchorPoint = pNode->getAnchorPoint();
	CCSize	nodeSize	= pNode->getContentSize();
	CCPoint nodePoint	= pNode->getPosition();
	nodePoint.x -= anchorPoint.x*nodeSize.width;
	nodePoint.y -= anchorPoint.y*nodeSize.height;

	if ( (((nodePoint.x + nodeSize.width - getPositionX() > 0.0f || nodePoint.x - getPositionX() - m_size.width < 0.0f) ||
		  (nodePoint.x - getPositionX() < 0.0f && nodePoint.x + nodeSize.width - getPositionX() - m_size.width > 0.0f)) &&
		(nodePoint.y + nodeSize.height - getPositionY() > 0.0f || nodePoint.y - getPositionY() - m_size.height < 0.0f)) ||
		(((nodePoint.y + nodeSize.height - getPositionY() > 0.0f || nodePoint.y - getPositionY() - m_size.height < 0.0f) ||
		(nodePoint.y - getPositionY() < 0.0f && nodePoint.y + nodeSize.height - getPositionY() - m_size.height > 0.0f)) &&
		(nodePoint.x + nodeSize.width - getPositionX() > 0.0f || nodePoint.x - getPositionX() - m_size.width < 0.0f))
		)
		return true;
	else
		return false;
}

