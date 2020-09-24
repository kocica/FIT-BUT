// ---------------------------------------------------------------------------
// @author Filip Kocica <xkocic01@stud.fit.vutbr.cz>
// @brief  KRY - Project 2 - RSA Implementation
// @date   22.03.2020
// @file   kry.cpp
// ---------------------------------------------------------------------------

// Standard / STL
#include <vector>
#include <algorithm>
#include <cmath>

// Local
#include "kry.h"

// ---------------------------------------------------------------------------
// Program entry point
int main( int argc, char** argv )
{
    Options_t options = parseInput( argc, argv );

    switch( options.getMode() )
    {
        case Mode_t::GENERATE:
        {
            if( std::stoi( options.first ) <= 30 )
            {
                std::cerr << "Notice: Size of modulus has to be at least 31 bits." << std::endl;
                return 1;
            }

            RSA_t{}.generate( options.first );
            break;
        }
        case Mode_t::ENCRYPT:
        {
            RSA_t{}.encrypt( options.first, options.second, options.third );
            break;
        }
        case Mode_t::DECRYPT:
        {
            RSA_t{}.decrypt( options.first, options.second, options.third );
            break;
        }
        case Mode_t::BREAKING:
        {
            RSA_t{}.breaking( options.first, options.second, options.third );
            break;
        }
        case Mode_t::ERROR:
        default:
        {
            std::cerr << "Error: Wrong input." << std::endl;
            return 1;
        }
    }

    return 0;
}
