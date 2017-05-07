
// Copyright (C) 2017 htshp.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef GEL_LEXER_H
#define GEL_LEXER_H

#include <string>
#include <regex>
#include <vector>
#include <utility>
#include <initializer_list>
#include <functional>
#include <list>
#include "error.h"

namespace gel{

class unknown_char_error : public gel_error {
public:
    unknown_char_error( char unknown_char )
        :gel_error( str("Unknown character '") + unknown_char + "'." ){}
};

class lexer{
public:
    using action = std::function<void(const std::cmatch&)>;

    // Constructor.
    lexer( std::initializer_list<std::pair<str, action>> rules){
        // Convert string pairs to pairs of strings and regular expressions.
        // Regular expressions are converted to forward-match regular expressions.
        for(auto& rule : rules){
            std::regex regex{ str("^(") + rule.first + ")" };
            rules_.push_back({ regex, rule.second });
        }
    }
    
    void run(const str& source){
        cur_ = source.c_str();
        exit_flag = false;
        
        while( *cur_ != '\0' && !exit_flag ){
            scan_token_();
        }
    }
    
    void exit(){
        exit_flag = true;
    }
    
private:
    // Member variables.
    std::vector<std::pair<std::regex, action>> rules_;
    const char*                                cur_; // A variable that keep the reading position.
    bool                                       exit_flag = false;
    
    // Scan a token.
    void scan_token_(){
        std::cmatch match;
        bool matched = false;
        
        // Test whether each regular expression matches.
        for(auto& rule : rules_){
            if( std::regex_search(cur_, match, rule.first) ){
                matched = true;
                
                // Advance reading position.
                cur_ = match[0].second;
                
                // Run action.
                rule.second( match );
            }
        }
        
        if( !matched ){
            throw unknown_char_error( *cur_ );
        }
    }
};

}

#endif
