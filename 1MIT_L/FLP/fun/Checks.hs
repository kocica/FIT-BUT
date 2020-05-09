---------------------------------------------------------------------
-- @author Filip Kocica (xkocic01@stud.fit.vutbr.cz)
-- @date 2/22/2020
-- @brief FLP project - simplify-bkg
-- @file checks.hs
---------------------------------------------------------------------

{-# LANGUAGE RecordWildCards #-}

module Checks
    where

-- Local
import Types

-- Standard
import Data.Char
import Data.List
import Data.Ord

---------------------------------------------------------------------
-- Function checkCFG.
-- @brief Check correctness of given CFG.
---------------------------------------------------------------------
checkCFG :: CFG_t -> CFG_t
checkCFG cfg = do
    case ( duplicateT || duplicateN || duplicateR  || invalidT || invalidT2 || invalidN || invalidN2 ||
           invalidLHS || invalidRHS || invalidRHS2 || invalidS )
         of True  -> returnError  -- An error has occured - return errorneous CFG
            False -> returnCFG    -- All checks passed    - return valid CFG
         where duplicateT  = ( terms cfg ) /= nub ( terms cfg )                                                                                                               -- Duplicate term
               duplicateN  = ( nonTerms cfg ) /= nub ( nonTerms cfg )                                                                                                         -- Duplicate non term
               duplicateR  = ( rules cfg ) /= nub ( rules cfg )                                                                                                               -- Duplicate rule
               invalidT    = any ( not . \t -> ( isLower ( t !! 0 ) ) ) ( terms cfg )                                                                                         -- Invalid term
               invalidT2   = any ( \t -> ( length t ) > 1 ) ( terms cfg )                                                                                                     -- Invalid term
               invalidN    = any ( not . \n -> ( isUpper ( n !! 0 ) || ( n !! 0 ) == '#' ) ) ( nonTerms cfg )                                                                 -- Invalid non term
               invalidN2   = any ( \n -> ( length n ) > 1 ) ( nonTerms cfg )                                                                                                  -- Invalid non term
               invalidLHS  = any ( not . \r -> elem ( lhs r ) ( nonTerms cfg ) ) ( rules cfg )                                                                                -- Invalid left hand side of the rule
               invalidRHS  = any ( not . \r -> ( ( ( ( rhs r ) !! 0 ) == '#' ) || all ( \c -> elem [c] ( union ( nonTerms cfg ) ( terms cfg ) ) ) ( rhs r ) ) ) ( rules cfg ) -- rhs = epsylon or all chars terms / nonterms
               invalidRHS2 = any ( \r -> ( ( elem '#' r ) && ( ( length r ) > 1 ) ) ) ( map ( \r -> ( rhs r ) ) ( rules cfg ) )                                               -- Epsylon contained in rhs of rule of length > 1
               invalidS    = not ( elem ( startNonTerm cfg ) ( nonTerms cfg ) )                                                                                               -- Starting symbol not contained in non terminals
               returnCFG   = ( cfg )
               returnError = ( CFG_t True [] [] [] [] )