#ifndef HEADER_NAMESPACE_ZLIB_H
#define HEADER_NAMESPACE_ZLIB_H

// 만일, C 컴파일(Driver등)에서 사용하려면,
// namespace를 사용하지 않도록 한다.
// 그래서,
// 아래 define들이 비어있도록 #ifdef로 switch한다.

//////////////////////////////////////////////////////////////////////////
// ADAM Namespace Scope
#define NAMESPACE_ZLIB_BEGIN	namespace zlib {
#define NAMESPACE_ZLIB_END		}

//////////////////////////////////////////////////////////////////////////
// Using zlib Namesapce
// (Visual Studio + Visual Assist)에서,
// USING_NAMESPACE_xxx
// 하면, 다음번 선언하는 struct등이,
// 제대로 파싱되지 않아, 색상이 흰색으로 나오는 경우가 있다.
// 그래서, USING_NAMESPACE_xxx;
// 와 같이 끝에 ;를 붙여 사용하는 경우를 추천한다.
#define USING_NAMESPACE_ZLIB	using namespace zlib;

#endif // HEADER_NAMESPACE_ZLIB_H