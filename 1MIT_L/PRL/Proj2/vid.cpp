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
    int32_t procCount{ 0 };
    int32_t numCount{ 0 };
    int32_t procID{ 0 };
    MPI_Status mpiStatus;

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

            if ( input.peek() == ',' )
            {
                input.ignore();
            }
        }

        numCount = static_cast<int32_t>( alt.size() );
        int32_t diff = alt.size() - procCount;
        int32_t skip{ 0 };

        // Distribute altitudes to other processors
        for( int32_t i = 0; i < procCount; i++ )
        {
            const int32_t c = diff --> 0 ? 2 : 1;
            MPI_Send( &c, SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );
            MPI_Send( &alt.front(), SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );
            int32_t is = i + skip;
            MPI_Send( &is, SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );

            for( int32_t j = 0; j < c; j++ )
            {
                MPI_Send( &alt.at( i + skip + j ), SendCount, MPI_INT, i, SendTag, MPI_COMM_WORLD );
            }

            if( c > 1 )
            {
                skip++;
            }
        }
    }

    // Receive input size, process assigned number, front number and position of number in input vector
    int32_t c{ 0 }, m{ 0 }, m1{ 0 }, m2{ 0 }, f{ 0 }, i{ 0 };
    MPI_Recv( &c, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );
    MPI_Recv( &f, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );
    MPI_Recv( &i, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );

    MPI_Recv( &m1, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );
    if( c > 1 )
    {
        MPI_Recv( &m2, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );
    }

    // Calculate angle
    float a1 = ( i == 0 ) ? -std::numeric_limits<float>::max() : atan( ( m1 - f ) / float( i ) );
    float a2 = atan( ( m2 - f ) / float( i + 1 ) );
    float ma{ 0.0 };

    if( c > 1 )
    {
        ma = std::max( a1, a2 );
    }
    else
    {
        ma = a1;
    }

    // Parallel sum of prefixes - reduce algorithm (up sweep)
    for( int32_t i = 1; i <= procCount / 2; i *= 2 )
    {
        if( ( procID + 1 ) % i == 0 )
        {
            if( procID / i % 2 == 0 )
            {
                MPI_Send( &ma, SendCount, MPI_FLOAT, procID + i, SendTag, MPI_COMM_WORLD );
            }
            else
            {
                float t{ 0.0 };
                MPI_Recv( &t,  SendCount, MPI_FLOAT, procID - i, SendTag, MPI_COMM_WORLD, &mpiStatus );
                ma = std::max( ma, t );
            }
        }
        MPI_Barrier( MPI_COMM_WORLD );
    }

    // Clear
    if( procID == ( procCount - 1 ) )
    {
        ma = -std::numeric_limits<float>::max();
    }

    // Max prescan algorithm - down sweep
    for( int32_t i = procCount / 2; i >= 1; i /= 2 )
    {
        if( ( procID + 1 ) % i == 0 )
        {
            if( procID / i % 2 == 0 )
            {
                MPI_Send( &ma, SendCount, MPI_FLOAT, procID + i, SendTag, MPI_COMM_WORLD );
                MPI_Recv( &ma, SendCount, MPI_FLOAT, procID + i, SendTag, MPI_COMM_WORLD, &mpiStatus );
            }
            else
            {
                float t{ 0.0 };
                MPI_Send( &ma, SendCount, MPI_FLOAT, procID - i, SendTag, MPI_COMM_WORLD );
                MPI_Recv( &t,  SendCount, MPI_FLOAT, procID - i, SendTag, MPI_COMM_WORLD, &mpiStatus );
                ma = std::max( ma, t );
            }
        }
        MPI_Barrier( MPI_COMM_WORLD );
    }

    float ma2 = ma;
    if( c > 1 && a1 > ma )
    {
        ma2 = a1;
    }

    // Send visibility info from all processors to master and display
    if( procID == MasterProcID )
    {
        std::string output = "_";

        int32_t diff = numCount - procCount;

        if( c > 1 )
        {
            output += ",";
            output += a2 >= ma2 ? 'v' : 'u';
            diff--;
        }

        for( int32_t i = 1; i < procCount; i++ )
        {
            const int32_t c = diff --> 0 ? 2 : 1;

            for( int32_t j = 0; j < c; j++ )
            {
                output += ",";
                char v;
                MPI_Recv( &v, SendCount, MPI_CHAR, i, SendTag, MPI_COMM_WORLD, &mpiStatus );
                output += v;
            }
        }

        std::cout << output << std::endl;
    }
    else
    {
        char v1 = ( a1 > ma ? 'v' : 'u' );
        MPI_Send( &v1, SendCount, MPI_CHAR, MasterProcID, SendTag, MPI_COMM_WORLD );

        if( c > 1 )
        {
            char v2 = ( a2 > ma2 ? 'v' : 'u' );
            MPI_Send( &v2, SendCount, MPI_CHAR, MasterProcID, SendTag, MPI_COMM_WORLD );
        }
    }

    // Cleanup
    MPI_Finalize(); 
    return 0;
}
