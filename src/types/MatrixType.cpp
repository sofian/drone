#include "MatrixType.h"
#include "ColorSpace.h"

template<>
std::string MatrixType<float>::name() const { return "MatrixTypeFloat"; }

template<>
std::string MatrixType<char>::name() const { return "MatrixTypeChar"; }

template<>
std::string MatrixType<RGBA>::name() const { return "MatrixTypeRGBA"; }
