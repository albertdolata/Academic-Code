library IEEE;
USE IEEE.std_logic_1164.all;
USE IEEE.numeric_std.all;

entity fifo_bufor_tb is
end entity fifo_bufor_tb;

architecture test of fifo_bufor_tb is
 
   constant MEMORY_RIGHT_INDEX_DEFINE : integer := 7;
   constant DATA_BITS_RIGHT_INDEX_DEFINE : integer := 7;
   constant BITS_IN_DATA_DEFINE : integer := DATA_BITS_RIGHT_INDEX_DEFINE + 1;
   constant TEST_DATA_BITS : std_logic_vector := "01111111"; -- ilość bitów musi się zgadzać ze BITS_IN_DATA_DEFINE

   COMPONENT FIFOBuffer is
      generic (
         MEMORY_RIGHT_INDEX : integer := MEMORY_RIGHT_INDEX_DEFINE;
         DATA_BITS_RIGHT_INDEX : integer := DATA_BITS_RIGHT_INDEX_DEFINE
      );
      -- porty które sa w fifo
      port (
         clock, reset, write_en, read_en : in std_logic; 
         data_in : in std_logic_vector(DATA_BITS_RIGHT_INDEX downto 0);
         data_out : out std_logic_vector(DATA_BITS_RIGHT_INDEX downto 0);
         full_flag, empty_flag, error_flag : out std_logic
      );
  end COMPONENT FIFOBuffer;
      -- sygnały pomocnicze w testbenchu
      signal clock, reset, write_en, read_en : std_logic := '0';
      signal data_in : std_logic_vector(DATA_BITS_RIGHT_INDEX_DEFINE downto 0) := (others => '0');
      signal data_out : std_logic_vector(DATA_BITS_RIGHT_INDEX_DEFINE downto 0);
      signal full_flag, error_flag : std_logic := '0';
      signal empty_flag : std_logic := '1';
      -- stała dla zegara
      constant ckp: time := 10 ns;
      -- stała do zakończenia symulacji
      constant end_time: time := 500 ns;

begin

   -- przypisywanie portom sygnałów z tb
   uut : FIFOBuffer
         generic map (
         MEMORY_RIGHT_INDEX => MEMORY_RIGHT_INDEX_DEFINE,
         DATA_BITS_RIGHT_INDEX => DATA_BITS_RIGHT_INDEX_DEFINE
      )
      port map (
      clock => clock,
      reset => reset,
      write_en => write_en,
      read_en => read_en,
      data_in => data_in,
      data_out => data_out,
      full_flag => full_flag,
      empty_flag => empty_flag,
      error_flag => error_flag
      );

   -- blok logiki generowania zegara

   clk_gen: process
   begin
      while now < end_time loop 
         clock <= '1';
         wait for ckp/2;
         clock <= '0';
         wait for ckp/2;
      end loop;
      wait;
   end process clk_gen;

   -- blok procesu testowania bufora
   test_proces: process
   begin

      report "Restarting FIFOBuffer before tests";

      reset <= '1';
      wait for 1 ns;
      reset <= '0';
      
      report "Flag status: ";
      report "Empty flag = " & boolean'image(empty_flag = '1');
      report "Full flag = " & boolean'image(full_flag = '1');
      report "Error flag = " & boolean'image(error_flag = '1');

      report "TEST 1 - Writing and reading data without errors";

      write_en <= '1';
      for i in 0 to MEMORY_RIGHT_INDEX_DEFINE  loop
         data_in <= std_logic_vector(to_unsigned(i+1, BITS_IN_DATA_DEFINE));
         wait for ckp;
         report "Added data: '" & to_string(std_logic_vector(data_in)) & "' to Queue"; 
      end loop;
      write_en <= '0';
      
      wait for ckp;

      assert full_flag = '1' report "Error: Queue is not full after writing 4 elements" severity failure;
      assert empty_flag = '0' report "Error: Queue is empty after adding data to the queue" severity failure;

      read_en <= '1';
      for i in 0 to MEMORY_RIGHT_INDEX_DEFINE loop
         wait for ckp;
         report "Data out is:  " & to_string(std_logic_vector(data_out)); 
         assert data_out = std_logic_vector(to_unsigned(i+1, BITS_IN_DATA_DEFINE)) report "Error: Reading is incorrect" severity failure;
      end loop;
      read_en <= '0';
      wait for ckp;

      assert empty_flag = '1' report "Error: Queue is not empty after reading 4 elements" severity failure;

      report "TEST 1 passed succesfully!";
     
      report "TEST 2 - Overflow writing, error flag set, handling error flag after reading data";

      report "Empty flag is set to " & boolean'image(empty_flag = '1');
      
      report "Writing 5 elements to memory ";

      write_en <= '1';
      for i in 0 to (MEMORY_RIGHT_INDEX_DEFINE + 1) loop
         data_in <= std_logic_vector(to_unsigned(i+1, BITS_IN_DATA_DEFINE));
         wait for ckp;
         report "Added data: '" & to_string(std_logic_vector(data_in)) & "' to Queue";
         assert error_flag = '0' report "Warning: Memory full, could not write the last data" severity warning; -- & to_string(std_logic_vector(to_unsigned(i+1, 4))) 
      end loop;
      write_en <= '0';

      assert error_flag = '1' report "Error: After writing 5 elements, error flag is not set to 1" severity failure;

      report "Error flag has been set to " & boolean'image(error_flag = '1');

      report "Reading one element";

      read_en <= '1';
      wait for ckp;

      read_en <= '0';
      
      assert error_flag = '0' report "Error: After reading data, error_flag still is set to 1" severity failure;

      report "Error flag set to: " & boolean'image(error_flag = '1');

      wait for ckp;

      report "TEST 3 - Reading from empty Queue, handling error flag after writing data";

      report "Resetting Queue";

      reset <= '1';
      wait for 1 ns;
      reset <= '0';
      wait for ckp;

      report "Flag status: ";
      report "Empty flag = " & boolean'image(empty_flag = '1');
      report "Full flag = " & boolean'image(full_flag = '1');
      report "Error flag = " & boolean'image(error_flag = '1');

      report "Reading data from Queue";
      
      reset <= '0';
      read_en <= '1';
      wait for ckp;
      if error_flag = '1' then
       report "Warning: Queue is empty, couldn't read any data" severity warning;
      end if;
      
      assert error_flag = '1' report "Error: After reading form empty queue error_flag is not set to 1" severity failure;

      report "Error flag is set to " & boolean'image(error_flag = '0');

      report "Writing data to the Queue";
      read_en <= '0';
      write_en <= '1';
      data_in <= TEST_DATA_BITS;
      wait for ckp;

      assert error_flag = '0' report "Error: After writing data to the Queue, error_flag is not set to 0 " severity failure;
      assert empty_flag = '0' report "Error: After writing data to the Queue, empty_flag is not set to 0 " severity failure;

      report "Error flag is set to " & boolean'image(error_flag = '1');

      write_en <= '1';

      wait for ckp;

      report "All tests passed successfully!";
      wait;
      end process test_proces;
     
     end architecture test;
