---------------------------------------------------------------------
-- @author Filip Kocica (xkocic01@stud.fit.vutbr.cz)
-- @date 2/22/2020
-- @brief FLP project - simplify-bkg
-- @file algorithms.hs
---------------------------------------------------------------------

{-# LANGUAGE RecordWildCards #-}

module Algorithms
    where

-- Local
import Types

-- Standard
import Data.Char
import Data.List
import Data.Ord

---------------------------------------------------------------------
-- Function algorithm1.
-- @brief First algorithm 4.1 of TIN study text - implementation.
---------------------------------------------------------------------
createN_t :: CFG_t -> Int -> Bool -> CFG_t
-- Recurstion stop condition
createN_t cfg 1 _ = ( CFG_t False ( terms cfg ) [] ( startNonTerm cfg ) ( rules cfg ) )
-- Calculate N_t using recursion
createN_t cfg i stop = do
    let n_i1 = ( nonTerms ( createN_t cfg ( i - 1 ) stop ) )
    if ( stop )
        then cfg
        else do let n_i1_sigma  = union n_i1 ( terms cfg )
                let filterRules = filter ( \r -> ( ( rhs r ) == "#" ) || all ( \e -> elem e ( map ( \s -> ( s !! 0 ) ) n_i1_sigma ) ) ( rhs r ) ) ( rules cfg )
                let n_i         = nub ( map ( \r -> ( lhs r ) ) filterRules )
                if ( n_i1 == n_i )
                    then do let stop = True
                            ( CFG_t False ( terms cfg ) n_i1 ( startNonTerm cfg ) ( rules cfg ) )
                    else ( CFG_t False ( terms cfg ) n_i ( startNonTerm cfg ) ( rules cfg ) )

-- Calculate algorithm using calculated n_t
algorithm1 :: CFG_t -> CFG_t
algorithm1 cfg = do
    let resCFG = createN_t cfg ( length ( nonTerms cfg ) + 1 ) False
    if ( elem ( startNonTerm resCFG ) ( nonTerms resCFG ) )
        then do
            let resNonTerms = union ( nonTerms resCFG ) [ ( startNonTerm resCFG ) ]
            let termNonTerm = union ( nonTerms resCFG ) ( terms resCFG )
            let resRulesL   = filter ( \r -> elem ( ( lhs r ) !! 0 ) ( map ( \s -> ( s !! 0 ) ) termNonTerm ) ) ( rules resCFG )
            let resRulesLR  = filter ( \r -> ( ( rhs r ) == "#" ) || ( all ( \c -> elem c ( map ( \s -> ( s !! 0 ) ) termNonTerm ) ) ( rhs r ) ) ) ( resRulesL )
            ( CFG_t False ( terms resCFG ) resNonTerms ( startNonTerm resCFG ) resRulesLR )
        else
            ( CFG_t True [] [] [] [] )

---------------------------------------------------------------------
-- Function algorithm2.
-- @brief Second algorithm 4.2 of TIN study text - implementation.
---------------------------------------------------------------------
createV_i :: CFG_t -> Int -> Bool -> CFG_t
-- Recursion stop condition
createV_i cfg 1 _ = ( CFG_t False ( terms cfg ) [ ( startNonTerm cfg ) ] ( startNonTerm cfg ) ( rules cfg ) )
-- Calculate V_i using recursion
createV_i cfg i stop = do
    let v_i1 = ( nonTerms ( createV_i cfg ( i - 1 ) stop ) )
    if ( stop )
        then cfg
        else do let filteredRhs    = ( map ( \r -> ( rhs r ) ) ( filter ( \r -> ( elem ( lhs r ) v_i1 ) ) ( rules cfg ) ) )
                let flattenedRhs   = intercalate "" filteredRhs
                let filteredTokens = ( nub ( map ( \f -> [f] ) flattenedRhs ) )
                let v_i            = union filteredTokens v_i1
                if ( sort v_i == sort v_i1 )
                    then do let stop = True
                            ( CFG_t False ( terms cfg ) v_i1 ( startNonTerm cfg ) ( rules cfg ) )
                    else ( CFG_t False ( terms cfg ) v_i ( startNonTerm cfg ) ( rules cfg ) )

-- Calculate algorithm using calculated v_n
algorithm2 :: CFG_t -> CFG_t
algorithm2 cfg = do
    let resCFG      = createV_i cfg ( length ( terms cfg ) + length ( nonTerms cfg ) + 1 ) False
    let resNonTerms = intersect ( nonTerms resCFG ) ( nonTerms cfg )
    let resTerms    = intersect ( nonTerms resCFG ) ( terms cfg )
    let resRules    = filter ( \r -> ( elem ( lhs r ) ( nonTerms resCFG ) ) && ( ( ( rhs r ) == "#" ) || all ( \c -> elem c ( map ( \s -> ( s !! 0 ) ) ( nonTerms resCFG ) ) ) ( rhs r ) ) ) ( rules resCFG )
    ( CFG_t False resTerms resNonTerms ( startNonTerm resCFG ) resRules )