// ---------------------------------------------------------------------------
// @author Filip Kocica <xkocic01@stud.fit.vutbr.cz>
// @brief  PRL - Project 3 - Visibility
// @date   21.03.2020
// @file   vid.cpp
// ---------------------------------------------------------------------------

// Standard / STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>

// OpenMPI
#include <mpi.h>

// Local
#include "vid.h"

// ---------------------------------------------------------------------------
// Local anonymous namespace with constants
namespace
{
    const int32_t     SendTag       = 0;
    const int32_t     SendCount     = 1;
    const int32_t     MasterProcID  = 0;
}

// ---------------------------------------------------------------------------
// Program entry point
int main( int argc, char** argv )
{
    using pair_t = std::pair<int32_t,int32_t>;
    int32_t procCount{ 0 };
    int32_t procID{ 0 };
    size_t  n{ 0 };
    std::map<int32_t, std::vector<pair_t>> procDistribution;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &procCount );
    MPI_Comm_rank( MPI_COMM_WORLD, &procID );

    if( procID == MasterProcID )
    {
        // Parse input into vector of altitudes
        std::stringstream input( argv[1] );
        std::vector<int32_t> alt;
        int32_t in{ 0 };
        while( input >> in )
        {
            alt.emplace_back( in );
            if( input.peek() == ',' )
            {
                input.ignore();
            }
        }

        // Store input size
        n = alt.size();

        // Calculate payload for each processor.
        for( size_t i = 1; i < alt.size(); i++ )
        {
            procDistribution[ ( i - 1 ) % procCount ].emplace_back( pair_t{ alt.at( i ) - alt.front(), i } );
        }

        // Distribute payload between processors.
        for( int32_t i = 0; i < procCount; i++ )
        {
            const auto& procNumbers = procDistribution[i];
            const int32_t numCount = static_cast<int32_t>( procNumbers.size() );

            MPI_Send( &numCount, SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );

            for( const auto& p : procNumbers )
            {
                MPI_Send( &p.first, SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );
                MPI_Send( &p.second, SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );
            }
        }
    }

    int32_t recvNum{ 0 };
    int32_t recvPos{ 0 };
    int32_t numCount{ 0 };
    MPI_Status mpiStatus;

    MPI_Recv( &numCount, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );

    for( int32_t i = 0; i < numCount; i++ )
    {
        MPI_Recv( &recvNum, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );
        MPI_Recv( &recvPos, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );

        float angle = atan( recvNum / float( recvPos ) );

        MPI_Send( &angle, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD );
    }

    if( procID == MasterProcID )
    {
        float recvAngle{ 0.0 };
        std::vector<float> angle{ 0.0 };

        for( int32_t i = 1; i < n; i++ )
        {
            MPI_Recv( &recvAngle, SendCount, MPI_INT, ( i - 1 ) % procCount, SendTag, MPI_COMM_WORLD, &mpiStatus );
            angle.emplace_back( recvAngle );
        }

        for( size_t i = 0; i < n; ++i )
        {
            //std::cout << angle.at( i ) << std::endl;
        }

        std::vector<float> a( angle );

        // Parallel sum of prefixes - reduce algorithm (implementation based on the slide 15 of h006.pdf)
        // Up
        for( int32_t j = 0; j <= std::log2( n - 1 ); j++ )
        {
            for( int32_t i = 0; i <= ( n - 1 ); i += pow( 2, j + 1 ) )
            {
                a.at( i + pow( 2, j + 1 ) - 1 ) = std::max( a.at( i + pow( 2, j ) - 1 ), a.at( i + pow( 2, j + 1 ) - 1 ) );
            }
        }

        // Clear
        a.at( n - 1 ) = 0;

        // Max prescan algorithm (implemented according to the slide 20 of h006.pdf presentation - prescan)
        // Down
        for( int32_t d = std::log2( n ) - 1; d >= 0; --d )
        {
            for( int32_t i = 0; i <= ( n - 1 ); i += pow( 2, d + 1 ) )
            {
                auto t = a.at( i + pow( 2, d ) - 1 );
                a.at( i + pow( 2, d ) - 1 ) = a.at( i + pow( 2, d + 1 ) - 1 );
                a.at( i + pow( 2, d + 1 ) - 1 ) = std::max( t, a.at( i + pow( 2, d + 1 ) - 1 ) );
            }
        }

        // DEBUG: To be removed
        for( size_t i = 0; i < n; ++i )
        {
            //std::cout << a.at( i ) << std::endl;
        }

        // Calculate visibility of each point due to observation point
        std::string output = "_,";
        for( size_t i = 1; i < n; ++i )
        {
            output += ( angle.at( i ) >= a.at( i ) ) ? "v," : "u,";
        }

        // Show visibility to standard output without comma at the end
        output.pop_back();
        std::cout << output << std::endl; // TODO: Remove ending line TODO
    }

    MPI_Finalize(); 
    return 0;
}
