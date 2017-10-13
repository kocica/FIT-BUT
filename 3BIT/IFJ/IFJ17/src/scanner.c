/**
 * @file scanner.c
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

#include "scanner.h"

static String token_buffer;
int row, col;

int get_token()
{
  SCANNER_STATE scanner_state;
  int c;
  bool iter = true;

  scanner_state = S_INIT;
  string_clear(&token_buffer);

  c = getchar();
  while (iter)
  {
    if (c == '\n')
    {
      col = 0;
      row++;
    }
    else
    {
      col++;
    }
    if (c == EOF && scanner_state == S_INIT)
    {
      return EOF;
    }

    switch(scanner_state)
    {
      case S_INIT:
      {
        if(c == '\'') // One line comment
        {
          scanner_state = S_ONE_LINE_COMMENT;
        }
        else if (is_delimeter(c))
        {
          token.token_type = DELIMETERS_BEGIN + get_delimeter_type(c);
          RETURN_FINAL_STATE; // Final state - T_DELIMETER
        }
        else if (c == '/')
        {
          scanner_state = S_SLASH;
        }
        else if (is_operator(c))
        {
          int following_c;
          if ((following_c = getchar()) == EOF)
          {
            return EOF;
          }
          if (!is_operator(following_c))
          {
            token.token_type = OPERATORS_BEGIN + get_operator_type(c);
            ungetc(following_c, stdin);
            RETURN_FINAL_STATE;
          }
          if      (c == '<' && following_c == '>') { token.token_type = TO_NOT_EQUAL;     RETURN_FINAL_STATE; }
          else if (c == '<' && following_c == '=') { token.token_type = TO_LOWER_EQUAL;   RETURN_FINAL_STATE; }
          else if (c == '>' && following_c == '=') { token.token_type = TO_GREATER_EQUAL; RETURN_FINAL_STATE; }
          else
          {
            error_code = E_LEXICAL;
            error_msg("%d:%d: Scanner: Unexpected character -%c- (ASCII: %d).", row, col, c, c);
          }
        }
        else if (c == '!')
        {
          scanner_state = S_STRING_BEGIN;
        }
        else if (isdigit(c))
        {
          scanner_state = S_NUMBER;
          string_push_char(&token_buffer, c);
        }
        else if (c == '_')
        {
          scanner_state = S_ID;
          string_push_char(&token_buffer, c);
        }
        else if (isalpha(c))
        {
          scanner_state = S_ID_KEYWORD;
          string_push_char(&token_buffer, c);
        }
        else if (!isspace(c))
        {
          error_code = E_LEXICAL;
          error_msg("%d:%d: Scanner: Unexpected character -%c- (ASCII: %d).", row, col, c, c);
        }
        break;
      }
      case S_ONE_LINE_COMMENT:
      {
        if (c == '\n')
        {
          row++;
          scanner_state = S_INIT;
          string_clear(&token_buffer);
        }
        break;
      }
      case S_MULTIPLE_LINES_COMMENT:
      {
        if (c == '\'')
        {
          scanner_state = S_MULTIPLE_LINES_COMMENT_END;
        }
        else if (c == '\n')
        {
          row++;
        }
        break;
      }
      case S_MULTIPLE_LINES_COMMENT_END:
      {
        if (c == '/')
        {
          scanner_state = S_INIT;
          string_clear(&token_buffer);
        }
        else
        {
          scanner_state = S_MULTIPLE_LINES_COMMENT;
        }
        break;
      }
      case S_ID:
      {
        if (isalnum(c) || c == '_')
        {
          string_push_char(&token_buffer, c);
        }
        else
        {
          ungetc(c, stdin);
          token.token_type = T_ID;
          string_to_upper(&token_buffer);
          token.data_string = string_get_string(&token_buffer);
          RETURN_FINAL_STATE;
        }
        break;
      }
      case S_ID_KEYWORD:
      {
        if (isalpha(c))
        {
          string_push_char(&token_buffer, c);
        }
        else if (c == '_' || isdigit(c))
        {
          string_push_char(&token_buffer, c);
          scanner_state = S_ID;
        }
        else
        {
          ungetc(c, stdin);
          if (is_reserved_keyword(&token_buffer))
          {
            token.token_type = KEYWORDS_BEGIN + get_keyword_type(&token_buffer);
            RETURN_FINAL_STATE;
          }
          else
          {
            token.token_type = T_ID;
            token.data_string = string_get_string(&token_buffer);
            RETURN_FINAL_STATE;
          }
        }

        break;
      }
      case S_SLASH:
      {
        if (c == '\'')
        {
          scanner_state = S_MULTIPLE_LINES_COMMENT;
        }
        else
        {
          token.token_type = T_DIVISION;
          ungetc(c, stdin);
          RETURN_FINAL_STATE;
        }
        break;
      }
      case S_STRING_BEGIN:
      {
        if (c == '\"')
        {
          scanner_state = S_STRING;
        }
        else
        {
          // TODO: Has '!' any other usecase than string literal ?
          // If not -> error
        }
        break;
      }
      case S_STRING:
      {
        if (c == '\"')
        {
          token.token_type = T_STRING;
          token.data_string = string_get_string(&token_buffer);
          RETURN_FINAL_STATE;
        }
        else if (c == '\\')
        {
          scanner_state = S_STRING_BACKSLASH;
        }
        else if (c == '\n')
        {
          error_code = E_LEXICAL;
          error_msg("%d:%d: Scanner: Unexpected character -%c- (ASCII: %d).", row, col, c, c);
        }
        else
        {
          string_push_char(&token_buffer, c);
        }
        break;
      }
      case S_STRING_BACKSLASH:
      {
        if (c == '\\')
        {
          string_push_char(&token_buffer, '\\');
        }
        else if (c == 'n')
        {
          string_push_char(&token_buffer, '\n');
        }
        else if (c == 't')
        {
          string_push_char(&token_buffer, '\t');
        }
        else if (c == '\"')
        {
          string_push_char(&token_buffer, '\"');
        }
        else if (isdigit(c))
        {
          int value = (c - '0') * 100;
          c = getchar();
          if (isdigit(c)) { value += (c - '0') * 10; }  // TODO: Do it with brain
          c = getchar();
          if (isdigit(c)) { value += (c - '0'); }
          if (value >= 1 && value <= 255)
          {
            string_push_char(&token_buffer, (char)value);
          }
        }

        scanner_state = S_STRING;

        break;
      }
      case S_NUMBER:
      {
        if (isdigit(c))
        {
          string_push_char(&token_buffer, c);
        }
        else if (c == '.')
        {
          string_push_char(&token_buffer, c);
          scanner_state = S_FLOAT;
        }
        else if (toupper(c) == 'E')
        {
          string_push_char(&token_buffer, c);
          scanner_state = S_EXP;
        }
        else
        {
          ungetc(c, stdin);
          token.token_type = T_INT;
          sscanf(string_get_string(&token_buffer), "%d", &token.data_int);
          RETURN_FINAL_STATE;
        }
        break;
      }
      case S_FLOAT:
      {
        if (isdigit(c))
        {
          string_push_char(&token_buffer, c);
        }
        else if (toupper(c) == 'E')
        {
          string_push_char(&token_buffer, c);
          scanner_state = S_EXP;
        }
        else
        {
          token.token_type = T_FLOAT;
          sscanf(string_get_string(&token_buffer), "%lf", &token.data_float);
          RETURN_FINAL_STATE;
        }
        break;
      }
      case S_EXP:
      {
        if (isdigit(c))
        {
          string_push_char(&token_buffer, c);
        }
        else if (c == '+' || c == '-')
        {
          string_push_char(&token_buffer, c);
        }
        else
        {
          token.token_type = T_FLOAT;
          sscanf(string_get_string(&token_buffer), "%lf", &token.data_float);
          RETURN_FINAL_STATE;
        }
        break;
      }
    }

    if (c == EOF)
    {
      iter = false;
    }
    c = getchar();
  }

  return EOF;
}

#ifdef SCANNER_DEBUG
  void scanner()
  {
    int ret;
    string_init(&token_buffer);

    while((ret = get_token()) != EOF)
    {
        print_token_type(token.token_type);

        if (token.token_type == T_STRING || token.token_type == T_ID)
        {
          printf("---%s---\n", string_get_string(&token_buffer));
        }
        if (token.token_type == T_INT)
        {
          printf("---%d---\n", token.data_int);
        }
        if (token.token_type == T_FLOAT)
        {
          printf("---%g---\n", token.data_float);
        }
    }

    string_free(&token_buffer);
  }
#endif

#ifdef SCANNER_DEBUG
  void print_token_type(TOKEN_TYPE tt)
  {
    switch(tt)
    {
      case T_NONE : { puts("T_NONE"); return; }
      case TK_AS : { puts("TK_AS"); return; }         // <--- KEYWORDS_BEGIN
      case TK_ASC : { puts("TK_ASC"); return; }
      case TK_DECLARE : { puts("TK_DECLARE"); return; }
      case TK_DIM : { puts("TK_DIM"); return; }
      case TK_DO : { puts("TK_DO"); return; }
      case TK_DOUBLE : { puts("TK_DOUBLE"); return; }
      case TK_ELSE : { puts("TK_ELSE"); return; }
      case TK_END : { puts("TK_END"); return; }
      case TK_CHR : { puts("TK_CHR"); return; }
      case TK_FUNCTION : { puts("TK_FUNCTION"); return; }
      case TK_IF : { puts("TK_IF"); return; }
      case TK_INPUT : { puts("TK_INPUT"); return; }
      case TK_INTEGER : { puts("TK_INTEGER"); return; }
      case TK_LENGTH : { puts("TK_LENGTH"); return; }
      case TK_LOOP : { puts("TK_LOOP"); return; }
      case TK_PRINT : { puts("TK_PRINT"); return; }
      case TK_RETURN : { puts("TK_RETURN"); return; }
      case TK_SCOPE : { puts("TK_SCOPE"); return; }
      case TK_STRING : { puts("TK_STRING"); return; }
      case TK_SUBSTR : { puts("TK_SUBSTR"); return; }
      case TK_THEN : { puts("TK_THEN"); return; }
      case TK_WHILE : { puts("TK_WHILE"); return; }
      case TK_AND : { puts("TK_AND"); return; }
      case TK_BOOLEAN : { puts("TK_BOOLEAN"); return; }
      case TK_CONTINUE : { puts("TK_CONTINUE"); return; }
      case TK_ELSEIF : { puts("TK_ELSEIF"); return; }
      case TK_EXIT : { puts("TK_EXIT"); return; }
      case TK_FALSE : { puts("TK_FALSE"); return; }
      case TK_FOR : { puts("TK_FOR"); return; }
      case TK_NEXT : { puts("TK_NEXT"); return; }
      case TK_NOT : { puts("TK_NOT"); return; }
      case TK_OR : { puts("TK_OR"); return; }
      case TK_SHARED : { puts("TK_SHARED"); return; }
      case TK_STATIC : { puts("TK_STATIC"); return; }
      case TK_TRUE : { puts("TK_TRUE"); return; }
      case TO_ASTERISK : { puts("TO_ASTERISK"); return; }        // <--- OPERATORS_BEGIN
      case TO_SLASH : { puts("TO_SLASH"); return; }
      case TO_BACKSLASH : { puts("TO_BACKSLASH"); return; }
      case TO_LOWER : { puts("TO_LOWER"); return; }
      case TO_GREATER : { puts("TO_GREATER"); return; }
      case TO_EQUAL : { puts("TO_EQUAL"); return; }
      case TO_PLUS : { puts("TO_PLUS"); return; }
      case TO_MINUS : { puts("TO_MINUS"); return; }
      case TO_LOWER_EQUAL : { puts("TO_LOWER_EQUAL"); return; }
      case TO_GREATER_EQUAL : { puts("TO_GREATER_EQUAL"); return; }
      case TO_NOT_EQUAL : { puts("TO_NOT_EQUAL"); return; }
      case TD_OPEN_BRACE : { puts("TD_OPEN_BRACE"); return; }      // <--- DELIMETERS_BEGIN
      case TD_CLOSE_BRACE : { puts("TD_CLOSE_BRACE"); return; }
      case TD_SEMICOLON : { puts("TD_SEMICOLON"); return; }
      case T_STRING : { puts("T_STRING"); return; }           // <--- OTHERS
      case T_ID : { puts("T_ID"); return; }
      case T_INT : { puts("T_INT"); return; }
      case T_FLOAT : { puts("T_FLOAT"); return; }
      case T_DIVISION : { puts("T_DIVISION"); return; }
    }
  }
#endif
