--- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s)             : Filip Kocica [xkocic01@stud.fit.vutbr.cz]
-- Date                  : 28.11.2017
-- Count of used FITkits : 3

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- Types of instruction in brainf*ck
	type INSTR_t is (I_WHILE_S, I_WHILE_E, I_BREAK, I_PRINT, I_READ, I_INC_VAL, I_DEC_VAL, I_INC_PTR, I_DEC_PTR, I_NOP, I_HALT);
	signal instr : INSTR_t;
	
	-- States of final state machine
	type FSM_STATE_t is (
	S_INC_PTR, S_DEC_PTR, S_INC_VAL, S_DEC_VAL, S_INC_VAL_2, S_DEC_VAL_2, S_PRINT, S_READ, S_PRINT_2, S_READ_2,
	S_WHILE_S, S_WHILE_E, S_BREAK_S,S_WHILE_S_FETCH_0, S_BREAK_S_FETCH_0, S_WHILE_S_FETCH_1, S_BREAK_S_FETCH_1, S_WHILE_E_FETCH_0, S_WHILE_E_FETCH_1, S_WHILE_E2, S_WHILE_E3,
	S_WHILE_S2, S_BREAK_S2, S_WHILE_S3, S_BREAK_S3, S_WHILE_E4, S_IDLE, S_DECODE, S_FETCH_0, S_FETCH_1, S_HALT, S_I_NOP
	);
	
	-- Detects break in while cycle
	signal BREAK : std_logic := '0';
	
	-- Actual and next state of final state machine
	signal act_state : FSM_STATE_t;
	signal next_state : FSM_STATE_t;
	
	-- Pointer register and enable to dec/inc
	signal POINTER_reg : std_logic_vector(9 downto 0);
	signal POINTER_inc : std_logic;
	signal POINTER_dec : std_logic;
	signal POINTER_EN : std_logic;	
	
	-- Counter register and enable to dec/inc
	signal COUNTER_reg : std_logic_vector(11 downto 0) := "000000000000";
	signal COUNTER_inc : std_logic;
	signal COUNTER_dec : std_logic;
	signal COUNTER_EN : std_logic := '0';
	
	-- Program counter register and enable to dec/inc
	signal PROGRAM_COUNTER_reg : std_logic_vector(11 downto 0);
	signal PROGRAM_COUNTER_inc : std_logic;
	signal PROGRAM_COUNTER_dec : std_logic;
	signal PROGRAM_COUNTER_EN : std_logic;
	
begin

	-- Final state machine with next state logic -- output logic -- moore final state machine
	FSM_proc: process(act_state, instr, CLK, RESET)

	begin
      next_state <= S_IDLE;

      DATA_EN <= '0';
	  CODE_EN <= '0';
	  POINTER_dec <= '0';
	  POINTER_inc <= '0';
	  POINTER_EN <= '0';
	  PROGRAM_COUNTER_EN <= '0';
	  PROGRAM_COUNTER_dec <= '0';
	  PROGRAM_COUNTER_inc <= '0';
	  OUT_WE <= '0';
	  COUNTER_EN <= '0';
	  COUNTER_inc <= '0';
	  COUNTER_dec <= '0';

		case act_state is	
			when S_IDLE =>
				next_state <= S_FETCH_0;

			 -- Fetch instr
			when S_FETCH_0 =>
				next_state <= S_FETCH_1;
				PROGRAM_COUNTER_EN <= '1';
				CODE_EN <= '1';
			
			when S_FETCH_1 =>
				next_state <= S_DECODE;
				
			 -- Decode instr
			when S_DECODE =>
				case instr is
					when I_HALT =>
						next_state <= S_HALT;

					when I_INC_PTR =>
						next_state <= S_INC_PTR;

					when I_DEC_PTR =>
						next_state <= S_DEC_PTR;
						
					when I_INC_VAL =>
						next_state <= S_INC_VAL;
						
					when I_DEC_VAL =>
						next_state <= S_DEC_VAL;
						
					when I_WHILE_S =>
						next_state <= S_WHILE_S;
						
					when I_WHILE_E =>
						next_state <= S_WHILE_E;
						
					when I_BREAK =>
						next_state <= S_BREAK_S;
						
					when I_PRINT =>
						next_state <= S_PRINT;

					when I_READ =>
						next_state <= S_READ;
						
					when I_NOP =>
						next_state <= S_I_NOP;
				end case;
			
			-- HALT
			when S_HALT =>
				next_state <= S_HALT;
				
			-- I_NOP -- ignore comments
			when S_I_NOP =>
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
				
				next_state <= S_FETCH_0;
				
			-- INCREASE PTR
			when S_INC_PTR =>
				POINTER_inc <= '1';
				POINTER_dec <= '0';
				
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
				next_state <= S_FETCH_0;
				
			-- DECREASE PTR
			when S_DEC_PTR =>
				POINTER_inc <= '0';
				POINTER_dec <= '1';
				
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
				next_state <= S_FETCH_0;
				
			-- INCREASE VAL
			when S_INC_VAL =>
				DATA_EN <= '1'; -- enable memory
				POINTER_EN <= '1'; -- enable bus			
				DATA_RDWR <= '0'; -- read content		
				
				next_state <= S_INC_VAL_2;
			
			when S_INC_VAL_2 =>
				POINTER_EN <= '1'; --enable bus
				DATA_EN <= '1'; -- enable memory
				DATA_RDWR <= '1'; -- write data
				DATA_WDATA <= DATA_RDATA + 1;
				
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter

				next_state <= S_FETCH_0;
				
			-- DECREASE VAL
			when S_DEC_VAL =>
				DATA_EN <= '1'; -- enable memory
				POINTER_EN <= '1'; -- enable bus		
				DATA_RDWR <= '0'; -- read content			
				
				next_state <= S_DEC_VAL_2;
			
			when S_DEC_VAL_2 =>
				POINTER_EN <= '1'; -- enable bus
				DATA_EN <= '1'; -- enable memory
				DATA_RDWR <= '1'; -- write data
				DATA_WDATA <= DATA_RDATA - 1;
				
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter

				next_state <= S_FETCH_0;
				
			-- PRINT			
			when S_PRINT =>
				DATA_EN <= '1'; -- enable memory
				POINTER_EN <= '1'; --enable bus				
				DATA_RDWR <= '0'; -- read content			
				
				next_state <= S_PRINT_2;		
			
			when S_PRINT_2 =>
				
				if (OUT_BUSY = '0') then -- print to lcd
					OUT_WE <= '1';
					OUT_DATA <= DATA_RDATA ;
					
					PROGRAM_COUNTER_dec <= '0';
					PROGRAM_COUNTER_inc <= '1'; -- increment program counter
					next_state <= S_FETCH_0;	
				else next_state <= S_PRINT_2;
				end if;	
			
			
			-- READ
			when S_READ =>
				IN_REQ <= '1';			
				next_state <= S_READ_2;
				
			when S_READ_2 =>
				if (IN_VLD = '1') then
					DATA_EN <= '1'; -- enable memory
					POINTER_EN <= '1'; -- enable bus				
					DATA_RDWR <= '1'; -- write data
					DATA_WDATA <= IN_DATA;
					
					IN_REQ <= '0';
					
					PROGRAM_COUNTER_dec <= '0';
					PROGRAM_COUNTER_inc <= '1'; -- increment program counter
					next_state <= S_FETCH_0;	
				else next_state <= S_READ_2;
				end if;				
				
				
			--WHILE
			when S_WHILE_S =>
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
				
				DATA_EN <= '1'; -- enable memory
				POINTER_EN <= '1'; --enable bus				
				DATA_RDWR <= '0'; -- read content	
				
				next_state <= S_WHILE_S2;			
				
			when S_WHILE_S2 =>
				if (DATA_RDATA = "00000000") then
					COUNTER_EN <= '1';
					next_state <= S_WHILE_S_FETCH_0;				
				else
					next_state <= S_FETCH_0;
				end if;				
				
			when S_WHILE_S_FETCH_0 =>
				
				next_state <= S_WHILE_S_FETCH_1;
				PROGRAM_COUNTER_EN <= '1';
				CODE_EN <= '1';
				
			when S_WHILE_S_FETCH_1 =>	  
				next_state <= S_WHILE_S3;
			
			when S_WHILE_S3 =>  
				if (COUNTER_reg /= "000000000000") then --if any braces are missing
					if (CODE_DATA = "000001011011") then -- [
						COUNTER_inc <= '1';
						COUNTER_dec <= '0';
					elsif (CODE_DATA = "000001011101") then -- ]
						COUNTER_inc <= '0';
						COUNTER_dec <= '1';
					end if;
					
					next_state <= S_WHILE_S_FETCH_0;
				else
					next_state <= S_FETCH_0;
				end if;
				
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
			
			--BREAK
			when S_BREAK_S =>
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
				
				DATA_EN <= '1'; -- enable memory
				POINTER_EN <= '1'; --enable bus				
				DATA_RDWR <= '0'; -- read content	
				
				next_state <= S_BREAK_S2;			
				
			when S_BREAK_S2 =>
				COUNTER_EN <= '1';
				next_state <= S_BREAK_S_FETCH_0;		
				
			when S_BREAK_S_FETCH_0 =>
				next_state <= S_BREAK_S_FETCH_1;
				PROGRAM_COUNTER_EN <= '1';
				CODE_EN <= '1';
				
			when S_BREAK_S_FETCH_1 =>	  
				next_state <= S_BREAK_S3;
			
			when S_BREAK_S3 =>
				if (CODE_DATA = "000001011101") then -- ]
					COUNTER_inc <= '0';
					COUNTER_dec <= '1';
					next_state <= S_FETCH_0;
				else
					next_state <= S_BREAK_S_FETCH_0;
				end if;
				
				PROGRAM_COUNTER_dec <= '0';
				PROGRAM_COUNTER_inc <= '1'; -- increment program counter
			
			--END WHILE			
			when S_WHILE_E =>				
				DATA_EN <= '1'; -- enable memory
				POINTER_EN <= '1'; -- neable bus				
				DATA_RDWR <= '0'; --read content
				
				next_state <= S_WHILE_E2;			
				
			when S_WHILE_E2 =>	  
				if (DATA_RDATA = "00000000") then					
					PROGRAM_COUNTER_dec <= '0';
					PROGRAM_COUNTER_inc <= '1'; -- increment program counter
					
					next_state <= S_FETCH_0;			
				else
					COUNTER_EN <= '1';
					PROGRAM_COUNTER_dec <= '1';
					PROGRAM_COUNTER_inc <= '0'; --increment program counter
				
					next_state <= S_WHILE_E_FETCH_0;						
				end if;				
				
			when S_WHILE_E_FETCH_0 =>
			
				next_state <= S_WHILE_E_FETCH_1;
				PROGRAM_COUNTER_EN <= '1';
				CODE_EN <= '1';
				
			when S_WHILE_E_FETCH_1 =>	  
				next_state <= S_WHILE_E3;
			
			when S_WHILE_E3 =>
				if (COUNTER_reg /= "000000000000") then -- If any braces are missing
					if (CODE_DATA = "000001011011") then -- [
						COUNTER_inc <= '0';
						COUNTER_dec <= '1';
					elsif (CODE_DATA = "000001011101") then -- ]
						COUNTER_inc <= '1';
						COUNTER_dec <= '0';
					end if;
					
					next_state <= S_WHILE_E4;
				else
					next_state <= S_FETCH_0;
				end if;
				
			when S_WHILE_E4 =>
				if (COUNTER_reg = "000000000000") then
					PROGRAM_COUNTER_dec <= '0';
					PROGRAM_COUNTER_inc <= '1'; --increment program counter
				else
					PROGRAM_COUNTER_dec <= '1';
					PROGRAM_COUNTER_inc <= '0'; -- increment program counter
				end if;
				
				next_state <= S_WHILE_E_FETCH_0;			
			
			when others => 
				next_state <= S_FETCH_0;
		end case;
	end process;
	
		-- Final state machine with actual state logic
	FSM_ACT_STATE_proc: process(EN, RESET, CLK)
	begin
	  if (RESET='1') then
		 act_state <= S_IDLE;
		elsif (CLK'event) and (CLK='1') then
		if(EN = '1') then
			act_state <= next_state; -- Save next state to actual state
		end if;
	  end if;
	end process;

	-- Instruction decoder
	INSTR_DEC_proc: process(instr, CLK, RESET)
	begin
		case CODE_DATA is
			when X"3E" => instr <= I_INC_PTR;  -- '>'
			when X"3C" => instr <= I_DEC_PTR;  -- '<'
			when X"2B" => instr <= I_INC_VAL;  -- '+'
			when X"2D" => instr <= I_DEC_VAL;  -- '-'
			when X"5B" => instr <= I_WHILE_S;  -- '['
			when X"5D" => instr <= I_WHILE_E;  -- ']'
			when X"2E" => instr <= I_PRINT;    -- '.'
			when X"2C" => instr <= I_READ;     -- ','
			when X"7E" => instr <= I_BREAK;    -- '~'
			when X"00" => instr <= I_HALT;     -- NILL
			when others => instr <= I_NOP;     -- NOP
		end case;
	end process;
	
	COUNTER_REG_proc: process(CLK, RESET)
	begin
		if (RESET = '1') then -- If reset signal is active
			COUNTER_reg <= (others=>'0');
		elsif (RESET = '0') and (CLK'event) and (CLK = '1') then -- Not reset and its leading edge
			if (COUNTER_EN = '0') then
				if (COUNTER_dec = '1') and (COUNTER_inc = '0') then
					COUNTER_reg <= COUNTER_reg - 1;
				end if;
				
				if (COUNTER_dec = '0') and (COUNTER_inc = '1') then
					COUNTER_reg <= COUNTER_reg + 1;
				end if;
			
			else COUNTER_reg <= "000000000001"; -- Set counter to 1
			end if;
		end if;	
	end process;
	
		-- Program counter register
	PROGRAM_COUNTER_proc: process(CLK, RESET)
	begin
		if (RESET = '1') then -- If reset signal is active
			PROGRAM_COUNTER_reg <= (others=>'0');		
		elsif (CLK'event) and (CLK = '1') then -- Not reset and its leading edge
			if (PROGRAM_COUNTER_dec = '1') and (PROGRAM_COUNTER_inc = '0') then -- decrement program counter
				PROGRAM_COUNTER_reg <= PROGRAM_COUNTER_reg - 1;
			end if;
			
			if (PROGRAM_COUNTER_dec = '0') and (PROGRAM_COUNTER_inc = '1') then -- increment progream counter
				PROGRAM_COUNTER_reg <= PROGRAM_COUNTER_reg + 1;
			end if;
		end if;	
	end process; 
	
	-- Save program counter to CODE_ADDR
	CODE_ADDR <= PROGRAM_COUNTER_reg when PROGRAM_COUNTER_EN='1' else
		(others => 'Z');

	
	-- Pointer to memory
	MEM_PTR_proc: process(CLK, RESET)
	begin
		if (RESET = '1') then -- If reset signal is active
			POINTER_reg <= (others=>'0');		
		elsif (RESET = '0') and (CLK'event) and (CLK = '1') then -- Not reset and its leading edge
			if (POINTER_dec = '1') and (POINTER_inc = '0') then
				POINTER_reg <= POINTER_reg - 1;
			end if;
			
			if (POINTER_dec = '0') and (POINTER_inc = '1') then
				POINTER_reg <= POINTER_reg + 1;
			end if;
		end if;	
	end process;
	
	-- R/W RAM
	DATA_ADDR <= POINTER_reg when POINTER_EN='1' else
		(others => 'Z');

end behavioral;

