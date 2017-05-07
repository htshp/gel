#include "lexer.h"
#include "doctest.h"
#include <vector>
#include <utility>

using namespace gel;

TEST_SUITE( "lexer test" );

TEST_CASE( "a100 100a => [a100, _, 100, a]" ) {
    // Setup.
    std::vector<std::pair<str, str>> RESULT = {
        { "id",     "a100" }, 
        { "space",  " " },
        { "number", "100"},
        { "id",     "a" }
    };
    
    lexer lexer{
        { "id",     "[a-zA-Z_][a-zA-Z_0-9]*" },
        { "number", "[0-9]+" },
        { "space",  "[ ]+" }
    };
    
    // Run.
    auto tokens = lexer.run( "a100 100a" );
    
    // Check.
    REQUIRE_EQ( tokens.childs.size(), RESULT.size() );
    
    int i = 0;
    for( auto& token : tokens.childs ){
        REQUIRE_EQ( token.tag,  RESULT[i].first );
        REQUIRE_EQ( token.text, RESULT[i].second );
        ++i;
    }
}

TEST_CASE( "Unknown error." ) {
    // Setup.
    lexer lexer{
        { "id",     "[a-zA-Z_][a-zA-Z_0-9]*" },
        { "number", "[0-9]+" },
        { "space",  "[ ]+" }
    };
    
    // Check.
    CHECK_THROWS_AS( lexer.run("a100."), unknown_char_error );
}
 
TEST_SUITE_END();
