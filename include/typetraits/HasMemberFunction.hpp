#pragma once

#define DEFINE_HAS_MEMBER_FUNCTION(__classname__, __name__, __signature__) \
template <typename Class> class __classname__ \
{ \
    template <typename, typename> struct memfunc_type; \
    template <typename T, typename R, typename... P> struct memfunc_type<T, R(P...)> { typedef R(T::*type)(P...); }; \
    typedef char pass; typedef long fail; \
    template <typename T,T> class check {}; \
    template <typename T> static pass sfinae(check<typename memfunc_type<T,__signature__>::type, &T::__name__>*); \
    template <typename  > static fail sfinae(...); \
    public: bool static const value = (sizeof(sfinae<Class>(nullptr)) == sizeof(pass)); \
};

#define DEFINE_HAS_MEMBER_FUNCTION_QUALIFIERS(__classname__, __name__, __signature__, __qualifiers__) \
template <typename Class> class __classname__ \
{ \
    template <typename, typename> struct memfunc_type; \
    template <typename T, typename R, typename... P> struct memfunc_type<T, R(P...)> { typedef R(T::*type)(P...) __qualifiers__; }; \
    typedef char pass; typedef long fail; \
    template <typename T,T> class check {}; \
    template <typename T> static pass sfinae(check<typename memfunc_type<T,__signature__>::type, &T::__name__>*); \
    template <typename  > static fail sfinae(...); \
    public: bool static const value = (sizeof(sfinae<Class>(nullptr)) == sizeof(pass)); \
};
