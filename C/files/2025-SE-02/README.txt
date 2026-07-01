Дадени са две примерни бази данни - една с малко записи и една с много записи.

---

Малката база данни се състои от файловете small_index.bin (индекс) и
small_values.bin (стойности). Примерни валидни стойности от малката база данни:

$ ./main small_index.bin small_values.bin accesses_total
26860175

$ ./main small_index.bin small_values.bin cpuload
7.828

---

Голямата база данни се състои от файловете large_index.bin (индекс) и
large_values.bin (стойности). Примерни валидни стойности от голямата база данни:

$ ./main large_index.bin large_values.bin mem_clients_normal
95440

$ ./main large_index.bin large_values.bin memory_Dirty_bytes
643072

$ ./main large_index.bin large_values.bin pressure_io_waiting_seconds_total
24700.947
