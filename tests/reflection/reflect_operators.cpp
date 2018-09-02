#include <reflection/reflection.h>

REFLECTABLE_OPERATOR(operator!=);
REFLECTABLE_OPERATOR(operator%);
REFLECTABLE_OPERATOR(operator%=);
REFLECTABLE_OPERATOR(operator&&);
REFLECTABLE_OPERATOR(operator&);
REFLECTABLE_OPERATOR(operator&=);
REFLECTABLE_OPERATOR(operator*);
REFLECTABLE_OPERATOR(operator*=);
REFLECTABLE_OPERATOR(operator+);
REFLECTABLE_OPERATOR(operator+=);
REFLECTABLE_OPERATOR(operator-);
REFLECTABLE_OPERATOR(operator-=);
REFLECTABLE_OPERATOR(operator->*);
REFLECTABLE_OPERATOR(operator/);
REFLECTABLE_OPERATOR(operator/=);
REFLECTABLE_OPERATOR(operator<);
REFLECTABLE_OPERATOR(operator<<);
REFLECTABLE_OPERATOR(operator<<=);
REFLECTABLE_OPERATOR(operator<=);
REFLECTABLE_OPERATOR(operator==);
REFLECTABLE_OPERATOR(operator>);
REFLECTABLE_OPERATOR(operator>=);
REFLECTABLE_OPERATOR(operator>>);
REFLECTABLE_OPERATOR(operator>>=);
REFLECTABLE_OPERATOR(operator^);
REFLECTABLE_OPERATOR(operator^=);
REFLECTABLE_OPERATOR(operator|);
REFLECTABLE_OPERATOR(operator|=);
REFLECTABLE_OPERATOR(operator||);

REFLECTABLE_OPERATOR(operator());
REFLECTABLE_OPERATOR(operator++);
REFLECTABLE_OPERATOR(operator--);
REFLECTABLE_OPERATOR(operator[]);

REFLECTABLE_OPERATOR_UNARY(operator!);
REFLECTABLE_OPERATOR_UNARY(operator->);
REFLECTABLE_OPERATOR_UNARY(operator~);
// TODO: unary operator*

REFLECTABLE_OPERATOR_ASSIGNMENT(=);

// TODO: operator,
// #define REFLECTION_OPERATOR_COMMA operator,
// REFLECTABLE(auto, REFLECTION_OPERATOR_COMMA,  member_function);
