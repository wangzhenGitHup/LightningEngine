/* protobuf config.h for MSVC.  On other platforms, this is generated
 * automatically by autoheader / autoconf / configure. */




/* the namespace of hash_map/hash_set */
// Apparently Microsoft decided to move hash_map *back* to the std namespace
// in MSVC 2010:
//   http://blogs.msdn.com/vcblog/archive/2009/05/25/stl-breaking-changes-in-visual-studio-2010-beta-1.aspx
// TODO(kenton):  Use unordered_map instead, which is available in MSVC 2010.
#if _MSC_VER < 1310 || _MSC_VER >= 1600
#define HASH_NAMESPACE std
#else
#define HASH_NAMESPACE stdext
#endif




/* the location of <hash_set> */
/* the location of <hash_map> */
#if _MSC_VER >= 1900
#else
#if ALICE_EDITOR||ALICE_WIN_PLAYER
/* define if the compiler has hash_map */
#define HAVE_HASH_MAP 1
/* define if the compiler has hash_set */
#define HAVE_HASH_SET 1
#define HASH_SET_H <hash_set>
#define HASH_MAP_H <hash_map>
#else

#endif
#endif

#if ALICE_ANDROID
#ifndef HASH_SET_H
#define HASH_SET_H <ext/hash_set>
#endif
#ifndef HASH_MAP_H
#define HASH_MAP_H <ext/hash_map>
#endif
#ifndef HASH_NAMESPACE
#define HASH_NAMESPACE _gnu_cxx
#endif
#define HAVE_PTHREAD
#endif

#if defined(__APPLE__)
#ifndef HASH_SET_H
#define HASH_SET_H <ext/hash_set>
#endif
#ifndef HASH_MAP_H
#define HASH_MAP_H <ext/hash_map>
#endif
#ifndef HASH_NAMESPACE
#define HASH_NAMESPACE _gnu_cxx
#endif
#define HAVE_PTHREAD
#endif


/* define if you want to use zlib.  See readme.txt for additional
 * requirements. */
// #define HAVE_ZLIB 1
