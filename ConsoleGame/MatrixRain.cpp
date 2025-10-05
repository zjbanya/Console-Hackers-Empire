#pragma once
#include <iostream>
#include "olcConsoleGameEngine.h" // ��Ϸ����
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

	//�������л�ȡ��������ֻ��Ҫ�ı������Բ��ظ�ʹ����
	void PrepareStreamer(sStreamer *s) // ָ�������庯��
	{
		s->nColumn = rand() % ScreenWidth();
		s->fPostition = 0.0f;
		s->fSpeed = rand() % 23 + 5; 
		
		s->sText.clear(); // �ڿ�ʼ�����ַ���֮ǰ����ַ���
		int nStreamerLength = rand() % 80 + 10; // ���10���ַ��������80���ַ���
		for (int i = 0; i < nStreamerLength; ++i) // ������
		{
			s->sText.append(1, RandomCharcter()); // xstring 
		}
		//s->sText = L"ABCDEFGHIJKMLNOPQRSTUVWXYZ";
	}

	wchar_t RandomCharcter()
	{
		return (wchar_t)(rand() % 0x1EF + 0x0C0); 
		//return (wchar_t)(rand() % 93 + 33); // ASCII�� &#33~&#93
	}

	list<sStreamer> listStreamers;
	int nMaxStreamers = 250; // Streamers����

protected:
	virtual bool OnUserCreate() // ���麯�� 
	{
		for (int n = 0; n < nMaxStreamers; ++n)
		{
			sStreamer s;
			PrepareStreamer(&s);
			listStreamers.push_back(s);
		}
		return true; 
	}

	virtual bool OnUserUpdate(float fElapsedTime) // ���麯��
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK); // default: PIXEL_SOLID FG_BLACK
		for (auto& s : listStreamers)
		{
			// ��̼��ɣ������� �����(Soft coding)������Ӳ����(hard coded)
			s.fPostition += s.fSpeed * fElapsedTime; // ֡��
			// Ϊ���ܺ��Ŵ�ӡ��ĸ����һ����Ȥ��forѭ��
			for (int i = 0; i < s.sText.size(); ++i)
			{
				// �Ӿ�Ч���������ٶ�ѡ��ͬ����ɫ, ���ԭ������
				// �������������Ǻ�Զ�����ǻῴ�����ƶ��ú�����
				// �������ǿ��Խ�ңԶ�ľ����ͬ����΢��һЩ
				short colour = s.fSpeed < 10.0f ? FG_DARK_GREEN : FG_GREEN; // ��һ��0x��ɫ
				if (i == 0)
					colour = FG_WHITE;
				else
					if (i <= 3)
						colour = FG_GREY;

				// ��� �㷨 
				int nCharIndex = (abs(i - (int)s.fPostition)) % s.sText.size();
				// x, y, ƫ���ַ�, �ı���ɫ
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