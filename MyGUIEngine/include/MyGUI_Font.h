/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_FONT_H__
#define __MYGUI_FONT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IManualResourceLoader.h"

namespace MyGUI
{

	class MYGUI_EXPORT Font : public IManualResourceLoader
    {

	public:

		// информация об диапазоне
		struct PairCodePoint
		{
			PairCodePoint(Char _first, Char _last) :
				first(_first),
				last(_last)
			{
			}

			// проверяет входит ли символ в диапазон
			bool isExist(Char _code) { return _code >= first && _code <= last; }

		public:
			Char first;
			Char last;
		};

		typedef std::vector<PairCodePoint> VectorPairCodePoint;

		// информация об одном символе
		struct GlyphInfo
		{
			Char codePoint;
			FloatRect uvRect;
			float aspectRatio;

			GlyphInfo() :
				codePoint(0),
				aspectRatio(1)
			{
			}

			GlyphInfo(Char _code, const FloatRect& _rect, float _aspect) :
				codePoint(_code),
				uvRect(_rect),
				aspectRatio(_aspect)
			{
			}

		};

		typedef std::vector<GlyphInfo> VectorGlyphInfo;

		// инфомация о диапазоне символов
		struct RangeInfo
		{
		private:
			RangeInfo() { }

		public:
			RangeInfo(Char _first, Char _last) :
				first(_first),
				last(_last)
			{
				range.resize(last - first + 1);
			}

			// проверяет входит ли символ в диапазон
			bool isExist(Char _code) { return _code >= first && _code <= last; }

			// возвращает указатель на глиф, или 0, если код не входит в диапазон
			GlyphInfo * getInfo(Char _code) { return isExist(_code) ? &range[_code - first] : nullptr; }

		public:
			Char first;
			Char last;
			VectorGlyphInfo range;
		};

		typedef std::vector<RangeInfo> VectorRangeInfo;

		struct PairCodeCoord
		{
			PairCodeCoord(Char _code, const IntCoord& _coord) :
				code(_code),
				coord(_coord)
			{
			}

			bool operator < (const PairCodeCoord & _value) const { return code < _value.code; }

			Char code;
			IntCoord coord;
		};

		typedef std::vector<PairCodeCoord> VectorPairCodeCoord;

		enum constCodePoints
		{
			FONT_CODE_SELECT = 0,
			FONT_CODE_TAB = 0x0009,
			FONT_CODE_LF = 0x000A,
			FONT_CODE_CR = 0x000D,
			FONT_CODE_SPACE = 0x0020,
			FONT_CODE_LATIN_START = 0x0021,
			FONT_CODE_NEL = 0x0085,
			FONT_CODE_LATIN_END = 0x00A6
		};

	public:
		Font(const std::string& _name);
        virtual ~Font();

		GlyphInfo * getSpaceGlyphInfo() { return & mSpaceGlyphInfo; }
		GlyphInfo * getTabGlyphInfo() { return & mTabGlyphInfo; }
		GlyphInfo * getSelectGlyphInfo() { return & mSelectGlyphInfo; }
		GlyphInfo * getSelectDeactiveGlyphInfo() { return & mSelectDeactiveGlyphInfo; }
		GlyphInfo * getCursorGlyphInfo() { return & mCursorGlyphInfo; }

		GlyphInfo * getGlyphInfo(Char _id);

		void addCodePointRange(Char _first, Char _second);
		void addHideCodePointRange(Char _first, Char _second);

		// проверяет, входит ли символ в зоны ненужных символов
		bool checkHidePointCode(Char _id);

		/** Clear the list of code point ranges. */
		void clearCodePointRanges();

		ITexture* getTextureFont() { return mTexture; }

		const std::string& getName() { return mName; }

		void setSource(const std::string& _source) { mSource = _source; }
		const std::string& getSource() const { return mSource; }

		void setTrueTypeSize(float _ttfSize) { mTtfSize = _ttfSize; }
		float getTrueTypeSize() const { return mTtfSize; }

		void setTrueTypeResolution(uint _ttfResolution) { mTtfResolution = _ttfResolution; }
		uint getTrueTypeResolution() const { return mTtfResolution; }

        void setAntialiasColour(bool _enabled) { mAntialiasColour = _enabled; }
        bool getAntialiasColour() const { return mAntialiasColour; }

		// дефолтная высота, указанная в настройках шрифта
		int getDefaultHeight() const { return mDefaultHeight; }
		void setDefaultHeight(int _height) { mDefaultHeight = _height; }

		// получившаяся высота при генерации в пикселях
		int getHeightPix() { return mHeightPix; }

		//ширина пробела в пикселях
		int getSpaceWidth() { return mSpaceWidth; }
		void setSpaceWidth(int _width) { mSpaceWidth = _width; }

		// ширина таба в пикселях
		int getTabWidth() { return mTabWidth; }
		void setTabWidth(int _width) { mTabWidth = _width; }

		// ширина курсора в пикселях
		int getCursorWidth() { return mCursorWidth; }
		void setCursorWidth(int _width) { mCursorWidth = _width; }

		// расстояние между символами при генерации в пикселях
		int getDistance() { return mDistance; }
		void setDistance(int _dist) { mDistance = _dist; }

		// смещение всех символов по горизонтали
		int getOffsetHeight() { return mOffsetHeight; }
		void setOffsetHeight(int _height) { mOffsetHeight = _height; }

		void addGlyph(Char _index, const IntCoord& _coord);

		bool isTrueType() { return mTtfResolution != 0; }

		void initialise();

	private:
		void addGlyph(GlyphInfo * _info, Char _index, int _left, int _top, int _right, int _bottom, int _finalw, int _finalh, float _aspect, int _addHeight = 0);

		void loadResourceTrueType(IRenderResource* _resource);
		void addRange(VectorPairCodeCoord & _info, size_t _first, size_t _last, int _width, int _height, float _aspect);
		void checkTexture();

		virtual void loadResource(IRenderResource* _resource);

	private:
        /// Source of the font (either an image name or a truetype font)
		std::string mSource;
        /// Size of the truetype font, in points
		float mTtfSize;
        /// Resolution (dpi) of truetype font
		uint mTtfResolution;

		int mDistance;
		int mSpaceWidth;
		int mTabWidth;
		int mCursorWidth;
		int mOffsetHeight;
		int mDefaultHeight;
		int mHeightPix;

		// отдельная информация о символах
		GlyphInfo mSpaceGlyphInfo, mTabGlyphInfo, mSelectGlyphInfo, mSelectDeactiveGlyphInfo, mCursorGlyphInfo;

		// символы которые не нужно рисовать
		VectorPairCodePoint mVectorHideCodePoint;

		// символы созданные руками
		VectorPairCodeCoord mVectorPairCodeCoord;

		// вся информация о символах
		VectorRangeInfo mVectorRangeInfo;

		MyGUI::ITexture* mTexture;

        /// for TRUE_TYPE font only
        bool mAntialiasColour;

		std::string mName;

    };

} // namespace MyGUI

#endif // __MYGUI_FONT_H__
