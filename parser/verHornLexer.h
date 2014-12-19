#if !defined(verHornLexer_P)
#define verHornLexer_P

#include <FlexLexer.h>

namespace ver {

class HornLexer: public yyFlexLexer
{
   int yylex();
};

}

#endif /* verHornLexer_P */
