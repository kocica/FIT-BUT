-- First project to course INP
-- Author: Filip Kocica (xkocic01 [at] fit [dot] vutbr [dot] cz)

-- Library
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- Entity
entity ledc8x8 is -- Took from assignment
	port (
		ROW, LED: out std_logic_vector(0 to 7);
		RESET	: in std_logic;
		SMCLK	: in std_logic
	);
end ledc8x8;

-- Architecture
architecture main of ledc8x8 is
-- Signals
signal leds, rows: std_logic_vector(7 downto 0);
signal ce_counter: std_logic_vector(7 downto 0);
signal ce: std_logic := '0';

begin
	
	-- Rotation of rows via rotation register
	rotation_reg: process(RESET, ce, SMCLK)
	begin	
		if RESET = '1' then -- Reset
			rows <= "10000000"; 
		elsif SMCLK'event and SMCLK = '1' and ce = '1' then
			rows <= rows(0) & rows(7 downto 1);
		end if;
	end process rotation_reg;
	

	-- Coutner decreasing frequency
	generator_ce: process(SMCLK, RESET)
	begin
		if RESET = '1' then -- Reset
			ce_counter <= "00000000";
		elsif SMCLK'event and SMCLK = '1' then -- 1
			ce_counter <= ce_counter + 1;
		end if;
	end process generator_ce;
	ce <= '1' when ce_counter(7 downto 0) = "11111111" else '0';
	
	
	-- Decoder
	decoder: process(rows)
	begin
		case rows is
			when "10000000" => leds <= "00001111";
			when "01000000" => leds <= "01111111";
			when "00100000" => leds <= "00011111";
			when "00010000" => leds <= "01110110";
			when "00001000" => leds <= "01110101";
			when "00000100" => leds <= "11110011";
			when "00000010" => leds <= "11110101";
			when "00000001" => leds <= "11110110";
			when others =>     leds <= "00000000";
		end case;
	end process decoder;
	
	
	-- Send signals to ports
	ROW <= rows;
	LED <= leds;

end main;
