---------------------------------------------------------------------
-- @author Filip Kocica (xkocic01@stud.fit.vutbr.cz)
-- @date 2/22/2020
-- @brief FLP project - simplify-bkg
-- @file types.hs
---------------------------------------------------------------------

{-# LANGUAGE RecordWildCards #-}

module Types
    where

-- Standard
import Data.Char
import Data.List
import Data.Ord

---------------------------------------------------------------------
-- Type declarations.
---------------------------------------------------------------------
type Term_t        = String
type NonTerm_t     = String
type TermNonTerm_t = String

data Rule_t = Rule_t {
    lhs          :: Term_t,           -- Left hand side of the rule ( non-term ).
    rhs          :: TermNonTerm_t     -- Right hand side of the rule ( combination of terminals and non-terms ).
} deriving( Eq )

data CFG_t = CFG_t {
    errorneous   :: Bool,          -- If true, CFG is errorneous.
    terms        :: [ Term_t ],    -- Input terminals.
    nonTerms     :: [ NonTerm_t ], -- Input non-terminals.
    startNonTerm ::   NonTerm_t,   -- Starting non-terminal.
    rules        :: [ Rule_t ]     -- Given deriving rules.
} deriving( Eq )

data Args_t = Args_t {
    invalidInput    :: Bool,       -- If true, wrong input has been provided.
    printCFGOut     :: Bool,       -- If true, print context free grammar to stdout.
    printFirstStep  :: Bool,       -- If true, perform first step of algorithm.
    printSecondStep :: Bool,       -- If true, perform second step of algorithm.
    inputCFG        :: FilePath    -- Given CFG in form of a file to be parsed.
}
