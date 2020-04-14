---------------------------------------------------------------------
-- @author Filip Kocica (xkocic01@stud.fit.vutbr.cz)
-- @date 2/22/2020
-- @brief FLP project - simplify-bkg
-- @file parsers.hs
---------------------------------------------------------------------

{-# LANGUAGE RecordWildCards #-}

module Parsers
    where

-- Local
import Types

-- Standard
import Text.ParserCombinators.ReadP

---------------------------------------------------------------------
-- Function parseInput.
-- @brief Parse program input arguments into variables.
---------------------------------------------------------------------
parseInput :: [String] -> Args_t
parseInput []                  = Args_t True  False False False ""
parseInput [s]     | s == "-i" = Args_t False True  False False ""
                   | s == "-1" = Args_t False False True  False ""
                   | s == "-2" = Args_t False False False True  ""
                   | otherwise = Args_t True  False False False ""
parseInput [s, fn] | s == "-i" = Args_t False True  False False fn
                   | s == "-1" = Args_t False False True  False fn
                   | s == "-2" = Args_t False False False True  fn
                   | otherwise = Args_t True  False False False ""
parseInput _                   = Args_t True  False False False ""

---------------------------------------------------------------------
-- Function parseCFG.
-- @brief Parse context free grammar.
---------------------------------------------------------------------
parseCFG :: String -> CFG_t
parseCFG fileContent = case readP_to_S parseCFG_P fileContent
                         of [ ( cfg, _ ) ] -> ( cfg )
                            _              -> ( CFG_t True [] [] [] [] )

-- ReadP version of parseCFG.
parseCFG_P :: ReadP CFG_t
parseCFG_P = do
    nonTerms      <- sepBy1 ( many1 $ satisfy predicate1 ) ( char ',' )
    char '\n'
    terms         <- sepBy1 ( many1 $ satisfy predicate1 ) ( char ',' )
    char '\n'
    startNonTerm  <- many1 $ satisfy predicate1
    char '\n'
    rules         <- parseRules
    eof
    return $ CFG_t False terms nonTerms startNonTerm rules

-- Helpers for parseCFG
predicate1 :: Char -> Bool
predicate1 ch = ( ch /= '\n' ) && ( ch /= ',' )

predicate2 :: Char -> Bool
predicate2 ch = ( ch /= '-' )

parseRules = many1 $ do
    rule <- parseRule
    char '\n'
    return rule

parseRule = do
    lhs <- many1 $ satisfy predicate2
    char '-'
    char '>'
    rhs <- many1 $ satisfy predicate1
    return $ Rule_t lhs rhs
