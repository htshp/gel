
// Copyright (C) 2017 htshp.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef GEL_DOM_H
#define GEL_DOM_H

#include <string>
#include <list>
#include <map>
#include <stdexcept>
#include <iterator>
#include <map>
#include <sstream>

namespace gel{

class dom{
public:
    // Member variables.
    std::string                        tag;
    std::map<std::string, std::string> attrs;
    std::string                        text;
    std::list<dom>                    childs;
    
    // Constructor.
    dom(const std::string& tag, const std::map<std::string, std::string>& attrs={}, const std::string& text="", const std::list<dom>& childs={})
        :tag(tag), attrs(attrs), text(text), childs( childs ){}
    
    // An indexer that references child trees.
    dom& operator[](int index){
        // Check index.
        if( index < 0 || index >= childs.size() ){
            throw std::out_of_range("tree::operator ()");
        }
        
        auto it = childs.begin();
        std::advance( it, index );
        return *it;
    }
    const dom& operator[](int index)const{
        // Check index.
        if( index < 0 || index >= childs.size() ){
            throw std::out_of_range("tree::operator ()");
        }
        
        auto it = childs.cbegin();
        std::advance( it, index );
        return *it;
    }
    
    // Accessor for referring to attributes.
    // When an element that does not exist is accessed, a new element is created.
    template<typename TKey>
    std::string& operator()(const TKey &key){
        return attrs[key];
    }
    
    // Convert to xml.
    std::string to_xml( int indent = 4, const std::string &nl = {'\n'} ) const{
        // Write a tag.
        std::stringstream ss;
        ss << '<' << tag;
        
        int attr_indent = ss.str().length() + 1;
        bool oneliner = attrs.size() > 1;
        
        // Write attributes.
        bool first = true;    
        for(auto attr : attrs){
            if(first){
                first = false;
                ss << ' ' << attr.first << "=\"" << attr.second << '"';
                continue;
            }
            ss << nl << std::string(attr_indent, ' ') << attr.first << "=\"" << attr.second << '"';
        }
        
        // If there is no text or child element, write an end tag and exit.
        if( text.empty() && childs.empty() ){
            ss << "/>";
            return ss.str();
        }
        ss << '>';
        
        if( !text.empty() ){
            // テキストの出力.
            if( oneliner ){
                ss << nl << std::string(indent, ' ');
            }
            ss << text;
        }else{
            // 子要素の出力.
            for(auto child : childs){
                ss << nl << std::string(indent, ' ') << child.to_xml( indent, nl + std::string(indent, ' ') );
            }
            ss << nl;
        }
        
        if( oneliner ){
            ss << nl;
        }
        ss << "</" << tag << '>';
    
        return ss.str();
    }
    
    // Convert to haml.
    std::string to_haml( int indent = 4, const std::string &nl = {'\n'} ) const{
        // Write a tag.
        std::stringstream ss;
        ss << '%' << tag;
    
        // Write attributes.
        bool first = true;    
        for(auto attr : attrs){
            if(first){
                first = false;
                ss << "{ " << attr.first << ": '" << attr.second << '\'';
                continue;
            }
            ss << ", " << attr.first << ": '" << attr.second << '\'';
        }
        if(!first){
            ss << " }";
        }
    
        if( !text.empty() ){
            ss << nl << std::string(indent, ' ') << text;
        }else{
            // Write childs.
            for(auto child : childs){
                ss << nl << std::string(indent, ' ') << child.to_haml( indent, nl + std::string(indent, ' ') );
            }
        }
    
        return ss.str();
    }
};

}

#endif
