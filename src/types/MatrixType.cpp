#include "MatrixType.h"
#include "ColorSpace.h"

template<>
std::string MatrixType<float>::name() const { return "MatrixTypeFloat"; }

template<>
QColor MatrixType<float>::color() const { return QColor(20,20,90); }

template<>
std::string MatrixType<char>::name() const { return "MatrixTypeChar"; }

template<>
QColor MatrixType<char>::color() const { return QColor(90,200,90); }

template<>
std::string MatrixType<RGBA>::name() const { return "MatrixTypeRGBA"; }

template<>
QColor MatrixType<RGBA>::color() const { return QColor(20,200,255); }

