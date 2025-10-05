#pragma once
#include <iostream>
#include "olcConsoleGameEngine.h" // 游戏引擎
#pragma once

using namespace std;

//struct MATCH_COLOUR
//{
//	COLORREF FG_DARK_GREEN = RGB(0, 250, 0);
//	COLORREF FG_GREEN = RGB(0, 139, 0);
//	COLORREF darkerFG_DARK_GREEN = RGB(
//		GetRValue(FG_DARK_GREEN) * 0.7,
//		GetGValue(FG_DARK_GREEN) * 0.7,
//		GetBValue(FG_DARK_GREEN) * 0.7
//	);
//};

/*********************************************************
// User MUST OVERRIDE THESE!!
virtual bool OnUserCreate() = 0;
virtual bool OnUserUpdate(float fElapsedTime) = 0;

// Optional for clean up 
virtual bool OnUserDestroy() { return true; }
**********************************************************/

class MatrixRain : public olcConsoleGameEngine 
{
public:
	MatrixRain()
	{

	}

private:
	struct sStreamer
	{
		int nColumn = 0;
		float fPostition = 0.0f;
		float fSpeed = 10.0f;
		wstring sText;
	};

	//从链表中获取对象，我们只需要改变其属性并重复使用它
	void PrepareStreamer(sStreamer *s) // 指定流光体函数
	{
		s->nColumn = rand() % ScreenWidth();
		s->fPostition = 0.0f;
		s->fSpeed = rand() % 23 + 5; 
		
		s->sText.clear(); // 在开始依赖字符串之前清除字符串
		int nStreamerLength = rand() % 80 + 10; // 最低10个字符流，最多80个字符流
		for (int i = 0; i < nStreamerLength; ++i) // 矩阵雨
		{
			s->sText.append(1, RandomCharcter()); // xstring 
		}
		//s->sText = L"ABCDEFGHIJKMLNOPQRSTUVWXYZ";
	}

	wchar_t RandomCharcter()
	{
		return (wchar_t)(rand() % 0x1EF + 0x0C0); 
		//return (wchar_t)(rand() % 93 + 33); // ASCII码 &#33~&#93
	}

	list<sStreamer> listStreamers;
	int nMaxStreamers = 250; // Streamers行数

protected:
	virtual bool OnUserCreate() // 纯虚函数 
	{
		for (int n = 0; n < nMaxStreamers; ++n)
		{
			sStreamer s;
			PrepareStreamer(&s);
			listStreamers.push_back(s);
		}
		return true; 
	}

	virtual bool OnUserUpdate(float fElapsedTime) // 纯虚函数
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK); // default: PIXEL_SOLID FG_BLACK
		for (auto& s : listStreamers)
		{
			// 编程技巧：尽量用 软编码(Soft coding)而不是硬编码(hard coded)
			s.fPostition += s.fSpeed * fElapsedTime; // 帧数
			// 为了能横着打印字母，做一个有趣的for循环
			for (int i = 0; i < s.sText.size(); ++i)
			{
				// 视觉效果：根据速度选择不同的绿色, 这个原因在于
				// 如果它真的离我们很远，我们会看到它移动得很慢，
				// 并且我们可以将遥远的距离等同于稍微暗一些
				short colour = s.fSpeed < 10.0f ? FG_DARK_GREEN : FG_GREEN; // 单一的0x颜色
				if (i == 0)
					colour = FG_WHITE;
				else
					if (i <= 3)
						colour = FG_GREY;

				// 这段 算法 
				int nCharIndex = (abs(i - (int)s.fPostition)) % s.sText.size();
				// x, y, 偏移字符, 文本颜色
				Draw(s.nColumn, (int)s.fPostition - i, s.sText[nCharIndex], colour);
			}
			if (s.fPostition - s.sText.size() >= ScreenHeight())
				PrepareStreamer(&s);
		}
		return true;
	}
};

int main()
{
	MatrixRain demo;
	demo.ConstructConsole(327, 57, 12, 12);
	demo.Start();

	return 0;
}