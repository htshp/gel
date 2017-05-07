
// Copyright (C) 2017 htshp.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef GEL_ERROR_H
#define GEL_ERROR_H

#include <exception>
#include <string>
#include "util.h"

namespace gel{

class gel_error{
public:
    // Constructor.
    gel_error( const str& message ):message_(message){
        
    }
    
    virtual const char* what() const noexcept{
        return message_.c_str();
    }
    
private:
    // Member variables.
    str message_;
};

}

#endif
