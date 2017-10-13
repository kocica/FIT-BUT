/**
 * @file parser.h
 *
 * IAL; IFJ project 2017 -- IFJ17 Compiler
 *
 * <br><b>IFJ17 Compiler<b>
 * <br>
 * <br>Compiles IFJ17 language (subset of FreeBASIC) source code to  IFJcode17 intercode.
 * <br>
 * <br> 1) Read source code from standard input
 * <br> 2) Perform syntactic analyzation
 * <br> 3) Compile source code to target code
 * <br> 4) Print intercode to standard output
 * <br>
 * <br><b>Team<b>
 * <br>  Filip Kocica   (xkocic01)
 * <br>  Matej Knazik   (xknazi00)
 * <br>  Andrea Fickova (xficko00)
 * <br>  Jiri Fiala     (xfiala54)
 *
 * @author(s) Filip Kocica (xkocic01@fit.vutbr.cz), FIT
 * @date      24.9.2017
 */

#include "parser.h"

void parser()
{
  init_parser();

  //print_token_type(token.token_type);
  printf("%d", token.token_type);
}

void init_parser()
{
  if (get_token() == EOF)
  {
    // EOF
  }

}

void syntax_error()
{
  error_code = E_SYNTACTIC;
  error_msg("%d:%d: Parser: Syntax error.", row, col);
}

void accept(TOKEN_TYPE act_token)
{
  if (token.token_type == act_token)
  {
    if (get_token() == EOF)
    {
      // EOF
    }
  }
  else
  {
    syntax_error();
  }
}

// -----------------------------------------------------

void parse_print()
{
  do
  {
    switch(token.token_type)
    {
      case T_ID:
      {

        break;
      }
      case T_INT:
      {

        break;
      }
      case T_FLOAT:
      {

        break;
      }
      case T_STRING:
      {

        break;
      }
      default:
      {
        syntax_error();
      }
    }
    accept(TD_SEMICOLON);
  } while (token.token_type == TD_SEMICOLON);
}
