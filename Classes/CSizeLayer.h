/************************************************************************/
/* code by 熊宇
/* date 2013/06/21
/* version v1.0.0
/* 使用名字空间common
/* use namespace common
/* 当是全屏显示的Layer时使用方法和CCLayer完全一样
/* use it as CCLayer if it is full of window
/* 当是自定范围时使用createWithSize(CCSize)来创建自定义可视大小的Layer
/* use createWithSize(CCSize) to create your Layer with custom visible rect
/* 可以在任何时间使用initSize(CCSize)来改变可视范围大小
/* use initSize(CCSize) to change visible rect in anytime
/************************************************************************/

#ifndef _CSIZELAYER_H_
#define _CSIZELAYER_H_

#include "cocos2d.h"

namespace common
{
	class CSizeLayer : public cocos2d::CCLayer
	{
	public:
		CSizeLayer();

		static CSizeLayer* createWithSize(cocos2d::CCSize _size);
		void initSize(cocos2d::CCSize _size);

		virtual void visit();
		virtual void addChild(CCNode * child, int zOrder, int tag);
		virtual void addChild(CCNode * child, int zOrder);
		virtual void addChild(CCNode * child);
	protected:
		void beforeDraw();
		void afterDraw();
		cocos2d::CCRect getViewRect();

		bool m_bScissorRestored;
		cocos2d::CCSize m_sizeView;
		cocos2d::CCRect m_tParentScissorRect;
	};
}

#endif
