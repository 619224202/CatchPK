#ifndef _FILTERWORD_H_
#define _FILTERWORD_H_

#define FILTERWORD_PATH "filterword.txt"

#include "cocos2d.h"

namespace common
{
	bool CanInput(const char* _word)
	{
		std::string checkStr = _word;
		if ( -1 != checkStr.find((char)32) ||
			-1 != checkStr.find((char)13) ||
			-1 != checkStr.find((char)10))
			return false;

		unsigned long size;
		unsigned char* uc = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(FILTERWORD_PATH, "r", &size);
		char c[15000];
		strcpy(c,(char*)uc);
		std::string str = c;

		char separate[2];
		separate[0] = 13;
		separate[1] = '\0';

		while ( str.length() )
		{
			int index = str.find(separate);
			if ( index == -1 )
			{
				break;
			}
			else
			{
				std::string check = str.substr(0, index);
				str = str.substr(index+1);
				if ( -1 != checkStr.find(check) )
					return false;
				while ( 0 == str.find((char)10) || 0 == str.find((char)13) )
					str = str.substr(1);
			}
		}

		return true;
	}
}

#endif
