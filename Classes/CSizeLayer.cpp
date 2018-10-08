#include "CSizeLayer.h"
#include "baseRes.h"

using namespace cocos2d;
using namespace common;

common::CSizeLayer::CSizeLayer()
{
	initSize(CCDirector::sharedDirector()->getWinSize());
}

CSizeLayer* common::CSizeLayer::createWithSize( cocos2d::CCSize _size )
{
	CSizeLayer* pSizeLayer = new CSizeLayer;
	pSizeLayer->autorelease();
	pSizeLayer->initSize(_size);
	return pSizeLayer;
}

void common::CSizeLayer::initSize( cocos2d::CCSize _size )
{
	CCLayer::init();
	m_sizeView = _size;
	return CCLayer::setContentSize(m_sizeView);
}

void common::CSizeLayer::visit()
{
	// quick return if not visible
	if (!isVisible())
	{
		return;
	}

	kmGLPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
		this->transformAncestors();
	}

	this->transform();
	this->beforeDraw();

	if(m_pChildren)
	{
		ccArray *arrayData = m_pChildren->data;
		unsigned int i=0;

		// draw children zOrder < 0
		for( ; i < arrayData->num; i++ )
		{
			CCNode *child =  (CCNode*)arrayData->arr[i];
			if ( child->getZOrder() < 0 )
				child->visit();
			else
				break;
		}
		// this draw
		this->draw();

		// draw children zOrder >= 0
		for( ; i < arrayData->num; i++ )
		{
			CCNode* child = (CCNode*)arrayData->arr[i];
			child->visit();
		}

	}
	else
	{
		this->draw();
	}

	this->afterDraw();
	if ( m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	kmGLPopMatrix();
}

void common::CSizeLayer::beforeDraw()
{
	m_bScissorRestored = false;
	CCRect frame = getViewRect();
	if (CCEGLView::sharedOpenGLView()->isScissorEnabled()) {
		m_bScissorRestored = true;
		m_tParentScissorRect = CCEGLView::sharedOpenGLView()->getScissorRect();
		//set the intersection of m_tParentScissorRect and frame as the new scissor rect
		if (frame.intersectsRect(m_tParentScissorRect)) {
			float x = MAX(frame.origin.x, m_tParentScissorRect.origin.x);
			float y = MAX(frame.origin.y, m_tParentScissorRect.origin.y);
			float xx = MIN(frame.origin.x+frame.size.width, m_tParentScissorRect.origin.x+m_tParentScissorRect.size.width);
			float yy = MIN(frame.origin.y+frame.size.height, m_tParentScissorRect.origin.y+m_tParentScissorRect.size.height);
			CCEGLView::sharedOpenGLView()->setScissorInPoints(x, y, xx-x, yy-y);
		}
	}
	else {
		glEnable(GL_SCISSOR_TEST);
		CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
	}
}

void common::CSizeLayer::afterDraw()
{
	if (m_bScissorRestored) {//restore the parent's scissor rect
		CCEGLView::sharedOpenGLView()->setScissorInPoints(m_tParentScissorRect.origin.x, m_tParentScissorRect.origin.y, m_tParentScissorRect.size.width, m_tParentScissorRect.size.height);
	}
	else {
		glDisable(GL_SCISSOR_TEST);
	}
}

cocos2d::CCRect common::CSizeLayer::getViewRect()
{
	//CCPoint screenPos = this->convertToWorldSpace(ccp(-getPositionX(), -getPositionY()));

	float scaleX = this->getScaleX();
	float scaleY = this->getScaleY();

	for (CCNode *p = m_pParent; p != NULL; p = p->getParent()) {
		scaleX *= p->getScaleX();
		scaleY *= p->getScaleY();
	}

	return CCRectMake(/*screenPos.x*/getPositionX()*Scale_X, /*screenPos.y*/getPositionY()*Scale_Y, m_sizeView.width*scaleX/**Scale_X*/, m_sizeView.height*scaleY/**Scale_Y*/);
}

void common::CSizeLayer::addChild(CCNode * child, int zOrder, int tag)
{
	//child->ignoreAnchorPointForPosition(false);
	CCLayer::addChild(child, zOrder, tag);
	sortAllChildren();
}

void common::CSizeLayer::addChild(CCNode * child, int zOrder)
{
	this->addChild(child, zOrder, child->getTag());
}

void common::CSizeLayer::addChild(CCNode * child)
{
	this->addChild(child, child->getZOrder(), child->getTag());
}

