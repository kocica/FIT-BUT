---------------------------------------------------------------------
-- @author Filip Kocica (xkocic01@stud.fit.vutbr.cz)
-- @date 2/22/2020
-- @brief FLP project - simplify-bkg
-- @file simplify-bkg.hs
---------------------------------------------------------------------

{-# LANGUAGE RecordWildCards #-}

-- Local
import Types
import Parsers
import Printers
import Algorithms
import Checks

-- Standard
import Data.Char
import Data.List
import Data.Ord
import System.IO
import System.Exit
import System.Environment
import System.Directory
import Control.Applicative
import Text.ParserCombinators.ReadP

---------------------------------------------------------------------
-- Function processCFG.
-- @brief Process context free grammar according to the give args.
--        Then print CFG on standard output.
---------------------------------------------------------------------
processCFG :: Args_t -> CFG_t -> IO()
processCFG args cfg = do
    case ( errorneous cfg ) of
        True  -> do hPutStrLn stderr $ "An error has occured: Failed to parse CFG."
                    exitWith( ExitFailure 1 )
        False -> case ( errorneous ( checkCFG cfg ) ) of
            True  -> do hPutStrLn stderr $ "An error has occured: Invalid CFG format."
                        exitWith( ExitFailure 1 )
            False -> if      ( printCFGOut args )     then printCFG cfg
                     else if ( printFirstStep args )  then do let resCFG = algorithm1 cfg
                                                              if ( errorneous resCFG ) then do hPutStrLn stderr $ "Notice: L(G) is empty."
                                                                                               exitWith( ExitFailure 1 )
                                                                                       else printCFG resCFG
                     else if ( printSecondStep args ) then do let firstCFG = algorithm1 cfg
                                                              if ( errorneous firstCFG ) then do hPutStrLn stderr $ "Notice: L(G) is empty."
                                                                                                 exitWith( ExitFailure 1 )
                                                                                         else do let secondCFG = algorithm2 firstCFG
                                                                                                 printCFG secondCFG
                     else do hPutStrLn stderr $ "An error has occured: Invalid input."
                             exitWith( ExitFailure 1 )

---------------------------------------------------------------------
-- Function main.
-- @brief Entry point of program.
---------------------------------------------------------------------
main :: IO()
main = do
    args <- parseInput <$> getArgs
    if ( invalidInput args ) then do hPutStrLn stderr $ "An error has occured: Invalid input."
                                     exitWith( ExitFailure 1 )
                             else if ( null $ inputCFG args ) then do cfg <- parseCFG <$> hGetContents stdin
                                                                      processCFG args cfg
                                                              else do cfg <- parseCFG <$> readFile (inputCFG args)
                                                                      processCFG args cfg