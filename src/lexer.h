
// Copyright (C) 2017 htshp.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef GEL_LEXER_H
#define GEL_LEXER_H

#include <string>
#include <map>
#include <regex>
#include <utility>
#include <initializer_list>
#include "dom.h"
#include "error.h"

namespace gel{

class unknown_char_error : public gel_error {
public:
    unknown_char_error( char unknown_char )
        :gel_error( str("Unknown character '") + unknown_char + "'." ){}
};

class lexer{
public:
    // Constructor.
    lexer( std::initializer_list<std::pair<str, str>> rules){
        // Convert string pairs to pairs of strings and regular expressions.
        // Regular expressions are converted to forward-match regular expressions.
        for(auto& rule : rules){
            std::regex regex{ str("^(") + rule.second + ")" };
            regex_table_.insert({ rule.first, regex });
        }
    }
    
    dom run(const str& source){
        cur_ = source.c_str();
        
        dom tokens{"tokens"};
        
        while( *cur_ != '\0' ){
            tokens.childs.push_back( scan_token_() );
        }
        
        return tokens;
    }
    
private:
    // Member variables.
    std::map<str, std::regex> regex_table_;
    const char*               cur_; // A variable that keep the reading position.
    
    // Scan a token.
    dom scan_token_(){
        std::cmatch match;
        
        // Test whether each regular expression matches.
        for(auto& rule : regex_table_){
            if( std::regex_search(cur_, match, rule.second) ){
                cur_ = match[0].second;
                return dom{ rule.first, {}, match[0].str() };    
            }
        }
        
        throw unknown_char_error( *cur_ );
    }
};

}

#endif
