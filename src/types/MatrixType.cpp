#include "MatrixType.h"

template<>
std::string MatrixType<float>::name() const { return "MatrixTypeFloat"; }

template<>
QColor MatrixType<float>::color() const { return QColor(20,20,90); }

template<>
std::string MatrixType<char>::name() const { return "MatrixTypeChar"; }

template<>
QColor MatrixType<char>::color() const { return QColor(90,200,90); }

