// ---------------------------------------------------------------------------
// @author Filip Kocica <xkocic01@stud.fit.vutbr.cz>
// @brief  PRL - Project 2 - Odd-even transposition sort
// @date   08.03.2020
// @file   ots.hpp
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// @brief Master process will read input file and distribute work to other
//        processes which will do the computation.
// @param procCount - Number of processes (np argument)
// @param fileName  - Input file name
void masterReadAndDistribute( int32_t procCount, const std::string& fileName );

// ---------------------------------------------------------------------------
// @brief Process will receive number and process it (sort - transfer to
//        neighbour process and receive another one or keep number).
// @param procID    - Is identifier of process (min 0, max np - 1)
// @param procCount - Number of processes (np argument)
void receiveNumberAndProcess( int32_t procID, int32_t procCount );