#include "dom.h"
#include "doctest.h"

using namespace gel;

TEST_SUITE( "dom test" );

TEST_CASE( "Single element." ) {
    dom x{"elem"};
    CHECK_EQ( x.to_xml(), "<elem/>" );
}

TEST_CASE( "Single data." ) {
    dom x{"data", {}, "123"};
    CHECK( x.to_xml() == "<data>123</data>" );
}

TEST_CASE( "Attributes." ) {
    dom x{"data",{
        { "line", "5" },
        { "column", "12" }
    }, "123"};
    CHECK( x.to_xml() == "<data column=\"12\"\n\
      line=\"5\">\n\
    123\n\
</data>" );
}

TEST_CASE( "1+2 expr abstract syntax tree." ) {
    dom x{"expr", {}, "", {
        {"int", {}, "1"},
        {"sign", {}, "+"},
        {"int", {}, "2"}
    }};
    CHECK( x.to_xml() == "<expr>\n\
    <int>1</int>\n\
    <sign>+</sign>\n\
    <int>2</int>\n\
</expr>");
}

TEST_CASE( "Triple nest." ) {
    dom x{"a", {}, "", {
        {"b", {}, "", {
            {"c"}
        }}
    }};
    CHECK( x.to_xml() == "<a>\n\
    <b>\n\
        <c/>\n\
    </b>\n\
</a>");
}

TEST_CASE( "Haml." ) {
    const std::string HAML =
"%a{ column: '12', line: '5' }\n"
"  %b\n"
"    %c";

    dom x{"a", {
        { "line", "5" },
        { "column", "12" }      
    }, "", {
        {"b", {}, "", {
            {"c"}
        }}
    }};
    CHECK( x.to_haml(2) == HAML);
}

 
TEST_SUITE_END();
