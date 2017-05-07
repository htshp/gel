#include "lexer.h"
#include <vector>
#include <utility>
#include "doctest.h"
#include "token.h"

using namespace gel;

TEST_SUITE( "lexer test" );

TEST_CASE( "a100 100a => [a100, _, 100, a]" ) {
    // Setup.
    std::vector<token> RESULT = {
        { "id",     "a100" }, 
        { "space",  " " },
        { "number", "100"},
        { "id",     "a" }
    };
    
    std::vector<token> tokens;
    
    lexer lexer{
        { "[a-zA-Z_][a-zA-Z_0-9]*", [&]( auto m){
            tokens.push_back({ "id", m.str() });
        } },
        { "[0-9]+",                 [&]( auto m){
            tokens.push_back({ "number", m.str() });
        } },
        { "[ ]+",                   [&]( auto m){
            tokens.push_back({ "space", m.str() });
        } }
    };
    
    // Run.
    lexer.run( "a100 100a" );
    
    // Check.
    REQUIRE_EQ( tokens.size(), RESULT.size() );
    
    int i = 0;
    for( auto& token : tokens ){
        REQUIRE_EQ( token.type, RESULT[i].type );
        REQUIRE_EQ( token.text,  RESULT[i].text );
        ++i;
    }
}

TEST_CASE( "Unknown error." ) {
    // Setup.
    lexer lexer{
        { "[a-zA-Z_][a-zA-Z_0-9]*", []( auto ){
        } },
        { "[0-9]+", []( auto ){
        } },
        { "[ ]+", []( auto ){
        } }
    };
    
    // Check.
    CHECK_THROWS_AS( lexer.run("a100."), unknown_char_error );
}

TEST_CASE( "Exit from lexer." ) {
    // Setup.
    lexer lexer{
        { "[a-zA-Z_][a-zA-Z_0-9]*", []( auto ){
        } },
        { "[0-9]+", []( auto ){
        } },
        { "[ ]+", []( auto ){
        } },
        { "\\.", [&]( auto ){
            lexer.exit();
        } }
    };
    
    // Check.
    CHECK_NOTHROW( lexer.run("a100.") );
}
 
TEST_SUITE_END();
