<?php
/**
 *
 * @file    parser.php
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    6/2/2018
 *
 * IPP project 1
 *
 */


/** Enumerate of tokens used to detect tokens */
class CTokenType
{
	const T_NONE = -1;
	const T_STOP = 0;
	const T_ID = 1;
	//const T_INT = 2;
	//const T_FLOAT = 3;
	//const T_STRING = 4;
	const T_MOVE = 5;
	const T_DEFVAR = 6;
	const T_CREATEFRAME = 7;
	const T_PUSHFRAME = 8;
	const T_POPFRAME = 9;
	const T_CALL = 10;
	const T_RETURN = 11;
	const T_PUSHS = 12;
	const T_POPS = 13;
	const T_ADD = 14;
	const T_SUB = 15;
	const T_MUL = 16;
	const T_IDIV = 17;
	const T_LT = 18;
	const T_GT = 19;
	const T_EQ = 20;
	const T_AND = 21;
	const T_OR = 22;
	const T_NOT = 23;
	const T_INT2CHAR = 24;
	const T_STRI2INT = 25;
	const T_READ = 26;
	const T_WRITE = 27;
	const T_CONCAT = 28;
	const T_STRLEN = 29;
	const T_GETCHAR = 30;
	const T_SETCHAR = 31;
	const T_TYPE = 32;
	const T_LABEL = 33;
	const T_JUMP = 34;
	const T_JUMPIFEQ = 35;
	const T_JUMPIFNEQ = 36;
	const T_DPRINT = 37;
	const T_BREAK  = 38;
	const T_INT_W = 39;
	const T_STRING_W = 41;
	const T_BOOL_W = 42;
	const T_LF = 43;
	const T_GF = 44;
	const T_TF = 45;
	const T_AT = 46;
	//const T_BOOL = 47;
	const T_EOL = 48;
	const T_TEST = 49;
}

/** Array of instructions avaible in IPPcode18 */
$instructions = array(
	CTokenType::T_MOVE => "MOVE",
	CTokenType::T_DEFVAR => "DEFVAR",
	CTokenType::T_CREATEFRAME => "CREATEFRAME",
	CTokenType::T_PUSHFRAME => "PUSHFRAME",
	CTokenType::T_POPFRAME => "POPFRAME",
	CTokenType::T_CALL => "CALL",
	CTokenType::T_RETURN => "RETURN",
	CTokenType::T_PUSHS => "PUSHS",
	CTokenType::T_POPS => "POPS",
	CTokenType::T_ADD => "ADD",
	CTokenType::T_SUB => "SUB",
	CTokenType::T_MUL => "MUL",
	CTokenType::T_IDIV => "IDIV",
	CTokenType::T_LT => "LT",
	CTokenType::T_GT => "GT",
	CTokenType::T_EQ => "EQ",
	CTokenType::T_AND => "AND",
	CTokenType::T_OR => "OR",
	CTokenType::T_NOT => "NOT",
	CTokenType::T_INT2CHAR => "INT2CHAR",
	CTokenType::T_STRI2INT => "STRI2INT",
	CTokenType::T_READ => "READ",
	CTokenType::T_WRITE => "WRITE",
	CTokenType::T_CONCAT => "CONCAT",
	CTokenType::T_STRLEN => "STRLEN",
	CTokenType::T_GETCHAR => "GETCHAR",
	CTokenType::T_SETCHAR => "SETCHAR",
	CTokenType::T_TYPE => "TYPE",
	CTokenType::T_LABEL => "LABEL",
	CTokenType::T_JUMP => "JUMP",
	CTokenType::T_JUMPIFEQ => "JUMPIFEQ",
	CTokenType::T_JUMPIFNEQ => "JUMPIFNEQ",
	CTokenType::T_DPRINT => "DPRINT",
	CTokenType::T_BREAK => "BREAK"
);

/** Array of types and frames avaible in IPPcode18 */
$frames = array(
	CTokenType::T_INT_W => "int",
	CTokenType::T_STRING_W => "string",
	CTokenType::T_BOOL_W => "bool",
	CTokenType::T_LF => "LF",
	CTokenType::T_GF => "GF",
	CTokenType::T_TF => "TF"
);
	

/** Class representing one token, his type and attribute */
class CToken
{
	public $token_type;
	public $token_atr;
	
	public function __construct($type, $atr)
	{
		$this->token_type = $type;
		$this->token_atr = $atr;
	}
}

/** Runs the arguments check and extract + runs the lexical and syntax analyzation
    if some function throws error we may catch them and write error to stderr + return correct error code */
try
{
	CArguments::getArgs($argc, $argv);
	if(CArguments::$help)
	{
		display_help();
		die(0);
	}
	$fileCont = CFileOp::readFileContent();
	$tokens = CLexer::FSM($fileCont);
	CParser::syntaxCheck($tokens);
}
catch(CRuntimeErrEx $e)
{
	$e->what();
	die($e->ec);
}

/** Class which performs actions with files */
class CFileOp
{
	
	/**
	 *  Reads and returns content of 'php://stdin'
	 */
	public static function readFileContent()
	{
		$fileCont = file_get_contents('php://stdin');
		/*if($fileCont == FALSE)
		{
			throw new CRuntimeErrEx("Can't read from stdin.", 11);
		}*/
		return $fileCont;
	}
	
	/**
	 * Write content of $content to 'php://stdout'
	 */
	public static function writeFileContent($content)
	{
		$file = file_put_contents('php://stdout', $content);
		if($file == FALSE)
		{
			throw new CRuntimeErrEx("Can't write to stdout.", 12);
		}
	}
}

/** Exception derived from standard exception containing error message and code */
class CRuntimeErrEx extends Exception
{
	public $ec;
	public $msg;
	public function __construct($msg, $ec)
	{
		$this->msg = $msg;
		$this->ec = $ec;
	}
	public function what()
	{
		fwrite(STDERR, "Runtime error: " . $this->msg . " Exit code = " . $this->ec . ".\n");
	}
}

/** Class which checks arguments validity and extracts important data from them saving to member variables */
class CArguments
{
	static $help = FALSE;
	
	public static function getArgs($argc, array $argv)
	{
		if ($argc > 2)
		{
			throw new CRuntimeErrEx("Wrong count of arguments.", 10);
		}
		else if ($argc === 2)
		{
			if ($argv[1] === '--help')
			{
				self::$help = TRUE;
			}
			else
			{
				throw new CRuntimeErrEx("Unknown argument.", 10);
			}
		}
	}
}

class CLexer
{
	/** Is string insruction ? */
	private static function isInstr($instID)
	{
		global $instructions;
		$tmpInstr = strtoupper($instID);
		
		foreach ($instructions as $token_type => $instruction)
		{
			if ($instruction === $tmpInstr)
			{
				return $token_type;
			}
		}
		
		return CTokenType::T_NONE;
	}

	/** Is string frame ? GF/LF/TF */
	private static function isFrame($frame)
	{
		global $frames;
		
		foreach ($frames as $token_type => $frameName)
		{
			if ($frameName === $frame)
			{
				return $token_type;
			}
		}
		
		return CTokenType::T_NONE;
	}
	
	/**
	 * FSM - Final state machine (Check lexical analyzation + gets tokens from input text)
	 *
	 * fileCont        -   contains string read from stdin
	 * return tokens passed to syntax analyzation
	 */
	public static function FSM($fileCont)
	{
		// Check .IPPcode18 header
		if (!(substr($fileCont, 0, 10) === '.IPPcode18'))
		{
			throw new CRuntimeErrEx("Lexical error -- Missing header.", 21);
		}
		
		$fileCont = str_split($fileCont);
		
		$buffer = NULL;
		$token = CTokenType::T_NONE;
		$len = count($fileCont);
		$c = NULL;
		$token_buffer = array();


		if ($len === 10)
		{
			return $token_buffer;
		}
		else if(!($fileCont[11] === "\n"))
		{
			throw new CRuntimeErrEx("Lexical error -- Missing header.", 21);
		}
		
		for($i = 12; $i < $len; $i++)
		{
			$c = $fileCont[$i];
			while (ctype_space($c))
			{
				if ($c === "\n")
				{
					array_push($token_buffer, new CToken(CTokenType::T_EOL, NULL));
				}
				$i++;
				if ($i === $len)
				{
					return $token_buffer;
				}
				$c = $fileCont[$i];
				$buffer = NULL;
			}
			
			if ($c === '#')
			{
				while (!($c === "\n"))
				{
					$i++;
					if ($i === $len)
					{
						if ($token !== CTokenType::T_NONE)
						{
							array_push($token_buffer, new CToken($token, $buffer));
						}
						return $token_buffer;
					}
					$c = $fileCont[$i];
				}
				array_push($token_buffer, new CToken(CTokenType::T_EOL, NULL));
				$buffer = NULL;
				continue;
			}
			
			if (ctype_alpha($c) OR $c === '_' OR $c === '-' OR $c === '$' OR $c === '&'
			    OR $c === '%' OR $c === '*')
			{
				$token = CTokenType::T_ID;
				$buffer .= $c;
				$i++;
				if ($i === $len)
				{
					array_push($token_buffer, new CToken($token, $buffer));
					return $token_buffer;
				}
				$c = $fileCont[$i];
				
				while (ctype_alnum(strval($c)) OR $c === '_' OR $c === '-' OR $c === '$' OR $c === '&'
			           OR $c === '%' OR $c === '*')
				{
					$buffer .= $c;
					$i++;
					if ($i === $len)
					{
						if (!(($ret = self::isInstr($buffer)) === CTokenType::T_NONE))
						{
							$token = $ret;
						}
					
						array_push($token_buffer, new CToken($token, $buffer));
						return $token_buffer;
					}
					$c = $fileCont[$i];
				}
				
				if (ctype_space($c) OR $c === '#')
				{
					if (!(($ret = self::isInstr($buffer)) === CTokenType::T_NONE))
					{
						$token = $ret;
					}
					
					array_push($token_buffer, new CToken($token, $buffer));
					$buffer = NULL;
					$token = CTokenType::T_NONE;
					$i--;
					continue;
				}
				else if ($c === '@')
				{
					if (!(($ret = self::isFrame($buffer)) === CTokenType::T_NONE))
					{
						$token = $ret;
					}
					else
					{
						throw new CRuntimeErrEx("Lexical error.", 21);
					}
					
					array_push($token_buffer, new CToken($token, $buffer));
					array_push($token_buffer, new CToken(CTokenType::T_AT, '@'));
					$buffer = NULL;
					
					if ($token === CTokenType::T_TF OR
					    $token === CTokenType::T_LF OR
						$token === CTokenType::T_GF)
					{
						$token = T_ID;
						
						$i++;
						if ($i === $len)
						{
							return $token_buffer;
						}
						$c = $fileCont[$i];
							
						while (ctype_alnum(strval($c)) OR $c === '_' OR $c === '-' OR $c === '$' OR $c === '&'
						       OR $c === '%' OR $c === '*')
						{
							$buffer .= $c;
							$i++;
							if ($i === $len)
							{
								array_push($token_buffer, new CToken($token, $buffer));
								return $token_buffer;
							}
							$c = $fileCont[$i];
						}
						
						if ($c === '#')
						{
							$i--;
						}
					}
					else
					{
						$token = CTokenType::T_TEST;
						$i++;
						if ($i === $len)
						{
							return $token_buffer;
						}
						$c = $fileCont[$i];
						while (!(ctype_space($c)) AND !($c === '#'))
						{
							$buffer .= $c;
							if ($c === '\\')
							{
								$int_array = array(100, 10, 1);
								$value = 0;
								for ($ind = 0; $ind < 3; $ind++)
								{
									$i++;
									$c = $fileCont[$i];
									if (!(ctype_digit(strval($c)))) { throw new CRuntimeErrEx("Lexical error.", 21); }
									else { $value += $int_array[$ind] * $c; }
									$buffer .= $c;
								}
								if (!($value >= 0 AND $value <= 999))
								{
									throw new CRuntimeErrEx("Lexical error.", 21);
								}
							}
							$i++;
							if ($i === $len)
							{
								array_push($token_buffer, new CToken($token, $buffer));
								return $token_buffer;
							}
							$c = $fileCont[$i];
						}
						if ($c === '#')
						{
							$i--;
						}
					}
					
					if (strtoupper($buffer) === 'TRUE' OR strtoupper($buffer) === 'FALSE')
					{
						array_push($token_buffer, new CToken($token, strtolower($buffer)));
					}
					else
					{
						array_push($token_buffer, new CToken($token, $buffer));
					}
					$buffer = NULL;
					$token = CTokenType::T_NONE;
				}
				else
				{
					throw new CRuntimeErrEx("Lexical error.", 21);
				}
			}
			
			else
			{
				throw new CRuntimeErrEx("Lexical error.", 21);
			}
		}
		return $token_buffer;
	}
}

class CParser
{
	/** Accepts expected token or throws exception */
	private static function accept($first, $second)
	{
		if (!($first === $second))
		{
			throw new CRuntimeErrEx("Syntax error.", 21);
		}
	}
	
	/** Is token ID ? True/False */
	private static function isID($token)
	{
		if ($token === CTokenType::T_NONE   OR
			$token === CTokenType::T_STOP   OR
			$token === CTokenType::T_AT     OR
			$token === CTokenType::T_TEST   OR
			$token === CTokenType::T_EOL)
		{
			return FALSE;
		}
		return TRUE;
	}
	
	/** Function which performs syntax check on <type> */
	private static function typeCheck($tokens, $index, $XMLbuffer, $n)
	{
		xmlwriter_start_element($XMLbuffer, 'arg' . $n);
		xmlwriter_start_attribute($XMLbuffer, 'type');
		xmlwriter_text($XMLbuffer, 'type');
		xmlwriter_end_attribute($XMLbuffer);
		xmlwriter_text($XMLbuffer, $tokens[$index+1]->token_atr);
		xmlwriter_end_element($XMLbuffer);
		
		$index++;
		
		if (!($tokens[$index]->token_type === CTokenType::T_ID))
		{
			return FALSE;
		}
		
		$tmpID = strtoupper($tokens[$index]->token_atr);
		
		if (!($tmpID === 'INT'    OR
		      $tmpID === 'FLOAT'  OR
			  $tmpID === 'STRING' OR
			  $tmpID === 'BOOL'))
		{
			return FALSE; 
		}
		
		return TRUE;
	}
	
	/** Function which performs syntax check on <label>, which represents label */
	private static function labelCheck($tokens, $index, $XMLbuffer, $n)
	{
		xmlwriter_start_element($XMLbuffer, 'arg' . $n);
		xmlwriter_start_attribute($XMLbuffer, 'type');
		xmlwriter_text($XMLbuffer, 'label');
		xmlwriter_end_attribute($XMLbuffer);
		xmlwriter_text($XMLbuffer, $tokens[$index+1]->token_atr);
		xmlwriter_end_element($XMLbuffer);
		
		$index++;
		
		if (!(self::isID($tokens[$index]->token_type)))
		{
			return FALSE;
		}
		
		return TRUE;
	}
	
	/** Function which performs syntax check on <var> which is variable */
	private static function variableCheck($tokens, $index, $XMLbuffer, $n)
	{
		global $instructions;
		
		xmlwriter_start_element($XMLbuffer, 'arg' . $n);
		xmlwriter_start_attribute($XMLbuffer, 'type');
		xmlwriter_text($XMLbuffer, 'var');
		xmlwriter_end_attribute($XMLbuffer);
		xmlwriter_text($XMLbuffer, strtoupper($tokens[$index+1]->token_atr) . '@' . $tokens[$index+3]->token_atr);
		xmlwriter_end_element($XMLbuffer);
			
		$index++;
		if (!($tokens[$index]->token_type === CTokenType::T_LF OR
			$tokens[$index]->token_type === CTokenType::T_GF OR
			$tokens[$index]->token_type === CTokenType::T_TF))
		{
			return FALSE;
		}
		
		$index++;
		if (!($tokens[$index]->token_type === CTokenType::T_AT))
		{
			return FALSE;
		}
		
		$index++;
		if (!(self::isID($tokens[$index]->token_type)))
		{
			return FALSE;
		}
		
		return TRUE;
	}
	
	/** Function which performs syntax check on <symb>, which may be variable or constant */
	private static function symbolCheck($tokens, $index, $XMLbuffer, $n)
	{
		if (!($tokens[$index+1]->token_type === CTokenType::T_INT_W OR
			$tokens[$index+1]->token_type === CTokenType::T_STRING_W OR
			$tokens[$index+1]->token_type === CTokenType::T_BOOL_W))
		{
			if (self::variableCheck($tokens, $index, $XMLbuffer, $n))
			{
				return TRUE;
			}
			return FALSE;
		}
			
		xmlwriter_start_element($XMLbuffer, 'arg' . $n);
		xmlwriter_start_attribute($XMLbuffer, 'type');
		xmlwriter_text($XMLbuffer, $tokens[$index+1]->token_atr);
		xmlwriter_end_attribute($XMLbuffer);
		xmlwriter_text($XMLbuffer, $tokens[$index+3]->token_atr);
		xmlwriter_end_element($XMLbuffer);
		
		$index++;
		$index++;
		if (!($tokens[$index]->token_type === CTokenType::T_AT))
		{
			return FALSE;
		}
		
		$index++;
		if (!($tokens[$index]->token_type === CTokenType::T_TEST))
		{
			return FALSE;
		}
		
		return TRUE;
	}
	
	/** Function which performs syntax analyzation on tokens passed as argument */
	public static function syntaxCheck($tokens)
	{
		array_push($tokens, new CToken(CTokenType::T_STOP, NULL));
		
		$XMLbuffer = xmlwriter_open_memory();
		xmlwriter_set_indent($XMLbuffer, 1);
		$res = xmlwriter_set_indent_string($XMLbuffer, ' ');
		xmlwriter_start_document($XMLbuffer, '1.0', 'UTF-8');
		xmlwriter_start_element($XMLbuffer, 'program');
		xmlwriter_start_attribute($XMLbuffer, 'language');
		xmlwriter_text($XMLbuffer, 'IPPcode18');

		$index = 0;
		$instr_counter = 1;
		$max_index = count($tokens);
		
		for($index = 0; $index < $max_index; $index++)
		{
			if ($tokens[$index]->token_type === CTokenType::T_EOL)
			{
				continue;
			}
			else if ($tokens[$index]->token_type === CTokenType::T_STOP)
			{
				xmlwriter_end_element($XMLbuffer);
				xmlwriter_end_document($XMLbuffer);
				echo xmlwriter_output_memory($XMLbuffer); return NULL;
			}
			
			xmlwriter_start_element($XMLbuffer, 'instruction');
			xmlwriter_start_attribute($XMLbuffer, 'order');
			xmlwriter_text($XMLbuffer, (string)$instr_counter++);
			xmlwriter_start_attribute($XMLbuffer, 'opcode');
			xmlwriter_text($XMLbuffer, strtoupper($tokens[$index]->token_atr));
			
			switch ($tokens[$index]->token_type)
			{
				// ========================== WITHOUT OPERANDS ============================
				
				case CTokenType::T_CREATEFRAME:
				case CTokenType::T_POPFRAME:
				case CTokenType::T_PUSHFRAME:
				case CTokenType::T_RETURN:
				case CTokenType::T_BREAK:
					break;

				// ========================== ONE OPERAND <var> ============================
				
				case CTokenType::T_DEFVAR:
				case CTokenType::T_POPS:
					if(!(self::variableCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					break;
					
				// ========================== ONE OPERAND <label> ============================
				
				case CTokenType::T_LABEL:
				case CTokenType::T_JUMP:
				case CTokenType::T_CALL:
					if(!(self::labelCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index++;
					break;
					
				// ========================== ONE OPERAND <symb> ============================
				
				case CTokenType::T_PUSHS:
				case CTokenType::T_WRITE:
				case CTokenType::T_DPRINT:
					if(!(self::symbolCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					break;
					
				// ========================== TWO OPERANDS <var> <symb> ============================
				
				case CTokenType::T_MOVE:
				case CTokenType::T_INT2CHAR:
				case CTokenType::T_STRLEN:
				case CTokenType::T_TYPE:
					if(!(self::variableCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					if(!(self::symbolCheck($tokens, $index, $XMLbuffer, 2)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					break;
				
				// ========================== TWO OPERANDS <var> <type> ============================
				
				case CTokenType::T_READ:
					if(!(self::variableCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					if(!(self::typeCheck($tokens, $index, $XMLbuffer, 2)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index++;
					break;
					
				// ========================== THREE OPERANDS <var> <symb1> <symb2> ============================
				
				case CTokenType::T_ADD:
				case CTokenType::T_SUB:
				case CTokenType::T_MUL:
				case CTokenType::T_IDIV:
				case CTokenType::T_LT:
				case CTokenType::T_GT:
				case CTokenType::T_EQ:
				case CTokenType::T_AND:
				case CTokenType::T_OR:
				case CTokenType::T_NOT:
				case CTokenType::T_STRI2INT:
				case CTokenType::T_CONCAT:
				case CTokenType::T_GETCHAR:
				case CTokenType::T_SETCHAR:
					if(!(self::variableCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					if(!(self::symbolCheck($tokens, $index, $XMLbuffer, 2)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					if(!(self::symbolCheck($tokens, $index, $XMLbuffer, 3)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					break;
					
				// ========================== THREE OPERANDS <label> <symb1> <symb2> ============================
				
				case CTokenType::T_JUMPIFEQ:
				case CTokenType::T_JUMPIFNEQ:
					if(!(self::labelCheck($tokens, $index, $XMLbuffer, 1)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index++;
					if(!(self::symbolCheck($tokens, $index, $XMLbuffer, 2)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					if(!(self::symbolCheck($tokens, $index, $XMLbuffer, 3)))
					{
						throw new CRuntimeErrEx("Syntax error.", 21);
					}
					$index += 3;
					break;
					
				default:
					throw new CRuntimeErrEx("Syntax error.", 21);
			}
			xmlwriter_end_element($XMLbuffer); // End of instruction element
			
			if ($tokens[$index+1]->token_type === CTokenType::T_STOP)
			{
				xmlwriter_end_element($XMLbuffer);
				xmlwriter_end_document($XMLbuffer);
				echo xmlwriter_output_memory($XMLbuffer); return NULL;
			}

			self::accept($tokens[++$index]->token_type, CTokenType::T_EOL);
		}
	}
}

/** Function which prints help to stdout */
function display_help()
{
	echo "  php5.6 parse.php [--help]\n";
	echo "     --help     - Zobrazi napovedu\n";
	echo "Skript typu filtr (parse.php v jazyce PHP 5.6)\n";
	echo "načte ze standardního vstupu zdrojový kód v IP-Pcode18\n";
	echo "(viz sekce 6), zkontroluje lexikální a syntaktickou\n";
	echo "správnost kódu a vypíše na standardní výstup XML reprezentaci programu.\n\n";
}