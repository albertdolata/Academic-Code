library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL; 

entity FIFOBuffer is
    generic (
        MEMORY_RIGHT_INDEX : integer := 3; --
        DATA_BITS_RIGHT_INDEX : integer := 3
    );

    Port (
        -- Sygnały systemowe: 
        clock   : in  std_logic; -- sygnał zegarowy 
        reset : in  std_logic; -- reset 
        -- Sygnały sterujące:
        write_en : in  std_logic; -- wejście determinujące zapis do kolejki
        read_en : in  std_logic;  -- wejście determinujące odczyt z kolejki
        -- Dane wejściowe:
        data_in : in std_logic_vector(DATA_BITS_RIGHT_INDEX downto 0); -- wejście z danymi
        -- Dane wyjściowe:
        data_out : out std_logic_vector(DATA_BITS_RIGHT_INDEX downto 0); -- wyjście z danymi
        full_flag : out std_logic; -- flaga informująca o pełnej kolecje
        empty_flag : out std_logic; -- flaga informująca o pustej kolecje
        error_flag : out std_logic -- flaga informująca o błędzie
    );
end FIFOBuffer;

architecture Behavioral of FIFOBuffer is
    -- Sygnały wewnętrzne
    type BufferMemory is array (0 to MEMORY_RIGHT_INDEX) of std_logic_vector(DATA_BITS_RIGHT_INDEX downto 0); -- deklaracja BufferMemory jako tablicy przechowywującej 4 wektory 8 bitowych
    signal memory : BufferMemory; -- sygnał reprezentujący pamięć
    signal write_ptr : integer range 0 to memory'right := 0; -- wskaźnik do miejsca gdzie zapisać nowe dane
    signal read_ptr : integer range 0 to memory'right := 0; -- wskaźnik do miejsca gdzie odczytać dane
    -- flagi wewnętrznę żeby móc odczytywać wartości z wyjść
    signal full_flag_internal : std_logic := '0'; -- flaga wewnętrzna informująca o pełnej kolecje
    signal empty_flag_internal : std_logic := '1'; -- flaga wewnętrzna informująca o pustej kolecje
    signal error_flag_internal : std_logic := '0'; -- flaga wewnętrzna informująca o błędzie

    -- signal licznik : integer range 0 to 4 := 0; -- sygnał pomocniczny do testowania


begin
    process(clock, reset)
        variable counter : integer range 0 to memory'length := 0; -- licznik do ilości zajętych miejsc w kolejce
    begin
        if reset = '1' then
            -- ustawienie wszystkich wektorów w pamięci na 0 czyli poporstu reset pamięci
            for i in memory'range loop
            memory(i) <= (others => '0'); 
            end loop;
            -- resetowanie wewnętrznych flag do początkowych wartości:
            full_flag_internal <= '0'; 
            empty_flag_internal <= '1';
            error_flag_internal <= '0';
            data_out <= (others => '0');
            write_ptr <= 0;
            read_ptr <= 0;
            counter := 0;
            
        elsif rising_edge(clock) then
            -- obsługa zapisu danych do kolejki: 
            if write_en = '1' then
                -- jak kolejka nie jest pełna to zapisz do pamieci nowe dane i zwiększ wskaźnik zapisu danych:
                if full_flag_internal = '0' then
                    memory(write_ptr) <= data_in; -- zapis danych do kolejki zgodnie ze wskaźnikiem zapisu
                    write_ptr <= (write_ptr + 1) mod memory'length; -- zwiększenie wskaźnika zapisu wraz z zawijaniem w przypadku dojścia do ostatniego indeksu
                    counter := counter + 1; -- zwiększ licznik zajetych miejsc w kolejce
                    
                    --resetowanie flagi błędu spowodowanej odczytem z pustej kolejki
                    if empty_flag_internal = '1' and error_flag_internal = '1' then
                        empty_flag_internal <= '0';
                        error_flag_internal <= '0';
                    -- zmiana flagi oznaczającej pustą kolejke w przypadku pierwszego zapisu:
                    elsif empty_flag_internal = '1' then
                        empty_flag_internal <= '0';
                    end if;
                    
                    -- zmiana flagi w przypadku przepełnienia się kolejki
                    if counter = memory'length then
                        full_flag_internal <= '1';
                    end if;
                -- zmiana flagi error jeżeli kolejka jest pełna    
                else
                    error_flag_internal <= '1';
                end if;

            -- obsługa odczytu danych z kolejki:
            elsif read_en = '1' then
                -- jeżeli kolejka nie jest pusta odczytaj z pamięcia dane wskazywane przez wskaźnik odczytu:
                if empty_flag_internal = '0' then
                    data_out <= memory(read_ptr); -- przypisanie do wyjścia danych w wektorze wskazywanym przez wskaźnik odczytu
                    read_ptr <= (read_ptr + 1) mod memory'length; -- zwiększenie wskaźnika odczytu wraz z zawijaniem w przypadku dojścia do sotatniego indeksu
                    counter := counter - 1; -- zmniejsz licznik zajetych miejsc w kolejce
                    
                    -- resetowanie flagi błędu spowodowanej zapisem do pełnej kolejki
                    if full_flag_internal = '1' and error_flag_internal = '1' then
                        full_flag_internal <= '0';
                        error_flag_internal <= '0';
                    -- zmiana flagi po odczycie jeżeli wcześniej pamięć była pełna:
                    elsif full_flag_internal = '1' then
                        full_flag_internal <= '0';
                    end if;
                    
                    -- zmiana flagi w przypadku pustej kolejki:
                    if counter = 0 then
                        empty_flag_internal <= '1';
                    end if;

                -- zmiana flagi error jeżeli kolejka jest pusta
                else
                    error_flag_internal <= '1';
                end if;
            end if;
            -- licznik <= counter; -- sygnał pomocniczny do testowania
        end if;
    end process;

    full_flag <= full_flag_internal; 
    empty_flag <= empty_flag_internal;
    error_flag <= error_flag_internal;

end Behavioral;