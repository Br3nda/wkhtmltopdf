// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
// This file is part of wkhtmltopdf.
//
// wkhtmltopdf is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// wkhtmltopdf is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with wkhtmltopdf.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __IMAGECONVERTER_HH__
#define __IMAGECONVERTER_HH__
#include "settings.hh"
#include "converter.hh"

namespace wkhtmltopdf {

class ImageConverterPrivate;

class ImageConverter: public Converter {
	Q_OBJECT
public:
	ImageConverter(settings::Global & settings);
	~ImageConverter();
private:
	ImageConverterPrivate * d;
	virtual ConverterPrivate & priv();
	friend class ImageConverterPrivate;
};

}
#endif //__IMAGECONVERTER_HH__
