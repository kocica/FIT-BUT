---------------------------------------------------------------------
-- @author Filip Kocica (xkocic01@stud.fit.vutbr.cz)
-- @date 2/22/2020
-- @brief FLP project - simplify-bkg
-- @file printers.hs
---------------------------------------------------------------------

{-# LANGUAGE RecordWildCards #-}

module Printers
    where

-- Local
import Types

-- Standard
import Data.List
import Data.Ord
import System.IO

---------------------------------------------------------------------
-- Function printCFG.
-- @brief Print context free grammar.
---------------------------------------------------------------------
printCFG :: CFG_t -> IO()
printCFG cfg = do
    putStrLn $ asCSL ( sort ( nonTerms cfg ) )
    putStrLn $ asCSL ( sort ( terms cfg ) )
    putStrLn $ startNonTerm cfg
    printRules $ rules cfg

printRules :: [ Rule_t ] -> IO()
printRules rules = do
    putStrLn $ asNSL( map asRule rules )

-- Helpers for printCFG - https://codereview.stackexchange.com/a/70463/176373

-- Convert list to comma separated list
asCSL :: [ String ] -> String
asCSL ss = ( intercalate "," ss )

-- Convert list to new-line separated list
asNSL :: [ String ] -> String
asNSL ss = ( intercalate "\n" ss )

-- Convert rule data structure to string representation
asRule :: Rule_t -> String
asRule rl = ( lhs rl ) ++ "->" ++ ( rhs rl )