// ---------------------------------------------------------------------------
// @author Filip Kocica <xkocic01@stud.fit.vutbr.cz>
// @brief  PRL - Project 2 - Odd-even transposition sort
// @date   08.03.2020
// @file   ots.cpp
// ---------------------------------------------------------------------------

// Standard / STL
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>

// OpenMPI
#include <mpi.h>

// Local
#include "ots.h"

// ---------------------------------------------------------------------------
// Local anonymous namespace with constants
namespace
{
    const std::string InputFileName = "numbers";
    const int32_t     SendTag       = 0;
    const int32_t     SendCount     = 1;
    const int32_t     MasterProcID  = 0;

    // Predicates for odd/even number detection
    const auto isOdd  = []( int32_t num ) -> bool { return ( num % 2 ) == 0; };
    const auto isEven = []( int32_t num ) -> bool { return ( num % 2 ) != 0; };
}

// ---------------------------------------------------------------------------
// Program entry point
int main( int argc, char** argv )
{
    int32_t procCount{ 0 };
    int32_t procID{ 0 };

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &procCount );
    MPI_Comm_rank( MPI_COMM_WORLD, &procID );

    // Master process with read values and distribute them to all processes.
    if( procID == MasterProcID )
    {
        masterReadAndDistribute( procCount, InputFileName );
    }

    // All processes will receive number and handle it.
    receiveNumberAndProcess( procID, procCount );

    MPI_Finalize(); 
    return 0;
}

// ---------------------------------------------------------------------------
void masterReadAndDistribute( int32_t procCount, const std::string& fileName )
{
    int32_t processRank{ 0 };
    std::string intercalated;

    std::ifstream numbers( fileName, std::ios::binary );
    std::vector<char> bytes(
         ( std::istreambuf_iterator<char>( numbers ) ),
         ( std::istreambuf_iterator<char>() ) );
    numbers.close();

    // Read bytes (uint8_t) from file, promote them to integers and distribute to other processes.
    for( const char byte : bytes )
    {
        int32_t num{ static_cast<uint8_t>( byte ) };
        intercalated += std::to_string( num ) + " ";
        MPI_Send( &num, SendCount, MPI_INT, processRank++, SendTag, MPI_COMM_WORLD );
    }

    if( ! intercalated.empty() )
    {
        intercalated.pop_back();
    }

    // Print each value separated by a space (' ') in between, but check emptyness to avoid errors.
    std::cout << intercalated << std::endl;
}

// ---------------------------------------------------------------------------
void receiveNumberAndProcess( int32_t procID, int32_t procCount )
{
    // Locals
    int32_t procNumber{ 0 };
    int32_t neighProcNum{ 0 };
    MPI_Status mpiStatus;

    // Receive number from master process sent in `masterReadAndDistribute`
    MPI_Recv( &procNumber, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD, &mpiStatus );

    // Limits for odd/even process numbers
    int32_t oddLimit  = procCount - 1;
    int32_t evenLimit = procCount - 1;

    for( int32_t idx = 1; idx <= ( procCount / 2 ); ++idx )
    {
        // First phase of the odd-even sort algorithm.
        // Processes with odd number:
        //  - Send owned (handled) number and wait until received a new one with lower value (swap with even process),
        //  - or the value sent (no swap).
        if( isOdd( procID ) && ( procID < oddLimit ) )
        {
            MPI_Send( &procNumber, SendCount, MPI_INT, procID + 1, SendTag, MPI_COMM_WORLD );
            MPI_Recv( &procNumber, SendCount, MPI_INT, procID + 1, SendTag, MPI_COMM_WORLD, &mpiStatus );
        }
        // Processes with even number:
        //  - Receives number and in case value handled by this process is lower, pass it to odd process (swap the values).
        //  - In case the value handled by this process is not lower, return received number back to odd process (no swap).
        else if( isEven( procID ) && ( procID <= oddLimit ) )
        {
            MPI_Recv( &neighProcNum, SendCount, MPI_INT, procID - 1, SendTag, MPI_COMM_WORLD, &mpiStatus );

            if( neighProcNum > procNumber )
            {
                MPI_Send( &procNumber, SendCount, MPI_INT, procID - 1, SendTag, MPI_COMM_WORLD ); 
                procNumber = neighProcNum;
            }
            else
            {
                MPI_Send( &neighProcNum, SendCount, MPI_INT, procID - 1, SendTag, MPI_COMM_WORLD );
            }
        }

        // Second phase of the odd-even algorithm.
        // Same principle, just shifter to right by one - odd/even swapped.
        // Thats the reason why we excluded the master process (which is the most left one)
        if( isEven( procID ) && ( procID < evenLimit ) )
        {
            MPI_Send( &procNumber, SendCount, MPI_INT, procID + 1, SendTag, MPI_COMM_WORLD );
            MPI_Recv( &procNumber, SendCount, MPI_INT, procID + 1, SendTag, MPI_COMM_WORLD, &mpiStatus );
        }
        else if( isOdd( procID ) && ( procID != MasterProcID ) && ( procID <= evenLimit ) )
        {
            MPI_Recv( &neighProcNum, SendCount, MPI_INT, procID - 1, SendTag, MPI_COMM_WORLD, &mpiStatus );

            if( neighProcNum > procNumber )
            {
                MPI_Send( &procNumber, SendCount, MPI_INT, procID - 1, SendTag, MPI_COMM_WORLD ); 
                procNumber = neighProcNum;
            }
            else
            {
                MPI_Send( &neighProcNum, SendCount, MPI_INT, procID - 1, SendTag, MPI_COMM_WORLD );
            }
        }
    }

    // Buffer for sorted values
    std::vector<int32_t> reorderBuffer( procCount );

    // Send numbers from all processes to the master process
    for( int32_t idx = 1; idx < procCount; ++idx )
    {
        if( procID == MasterProcID )
        {
            // Master receives numbers from processes with ID: 1 - N
            // and stores them on appropriate position in the array to be printed
            MPI_Recv( &neighProcNum, SendCount, MPI_INT, idx, SendTag, MPI_COMM_WORLD, &mpiStatus );
            reorderBuffer.at( idx ) = neighProcNum;
        }
        if( procID == idx )
        {
            // All processes except master sends their number to the master process
            MPI_Send( &procNumber, SendCount, MPI_INT, MasterProcID, SendTag, MPI_COMM_WORLD );
        }
    }

    // Master process, which received numbers from all processes will now print
    // numbers sorted in correct order (process ID 0 - N)
    if( procID == MasterProcID )
    {
        reorderBuffer.at( MasterProcID ) = procNumber;

        std::string intercalated;

        std::for_each( reorderBuffer.begin(), reorderBuffer.end(), [&]( auto num )
                       {
                           intercalated += std::to_string( num ) + "\n";
                       } );

        if( ! intercalated.empty() )
        {
            intercalated.pop_back();
        }

        std::cout << intercalated;
    }
}
