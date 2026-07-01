```bash
#!/bin/bash

# 1. изчистване
rm -rf LOGDIR
mkdir -p LOGDIR

echo "== Създаване на тестова структура =="

# 2. структура (много приятели)
for p in protocol1 protocol2 protocol3; do
  for a in account1 account2 account3 account4; do
    for f in friendA friendB friendC friendD friendE friendF friendG friendH friendI friendJ friendK friendL; do
      mkdir -p LOGDIR/$p/$a/$f
    done
  done
done

# helper функция за създаване на файл с N реда
create_file() {
  path=$1
  lines=$2
  for i in $(seq 1 $lines); do echo "msg"; done > "$path"
}

echo "== Генериране на данни =="

# различни количества (умишлено разпръснати)
create_file LOGDIR/protocol1/account1/friendA/2024-01-01-10-00-00.txt 15
create_file LOGDIR/protocol2/account2/friendA/2024-01-02-10-00-00.txt 10

create_file LOGDIR/protocol1/account1/friendB/2024-01-01-10-00-00.txt 8

create_file LOGDIR/protocol1/account2/friendC/2024-01-01-10-00-00.txt 20

create_file LOGDIR/protocol2/account1/friendD/2024-01-01-10-00-00.txt 5

create_file LOGDIR/protocol3/account3/friendE/2024-01-01-10-00-00.txt 30

create_file LOGDIR/protocol1/account4/friendF/2024-01-01-10-00-00.txt 12

create_file LOGDIR/protocol2/account2/friendG/2024-01-01-10-00-00.txt 7

create_file LOGDIR/protocol3/account1/friendH/2024-01-01-10-00-00.txt 3

create_file LOGDIR/protocol1/account3/friendI/2024-01-01-10-00-00.txt 25

create_file LOGDIR/protocol2/account4/friendJ/2024-01-01-10-00-00.txt 18

create_file LOGDIR/protocol3/account2/friendK/2024-01-01-10-00-00.txt 2

create_file LOGDIR/protocol1/account1/friendL/2024-01-01-10-00-00.txt 1

# празен, но валиден файл
touch LOGDIR/protocol2/account3/friendL/2024-01-02-10-00-00.txt

echo ""
echo "== Реален резултат (контролен) =="

find LOGDIR -type f | while read file; do
  friend=$(echo "$file" | awk -F'/' '{print $(NF-1)}')
  lines=$(wc -l < "$file")
  echo "$friend $lines"
done | awk '{sum[$1]+=$2} END {for (f in sum) print f, sum[f]}' | sort -k2 -nr

echo ""
echo "== Очакване =="
echo "→ Да се покажат само ТОП 10 приятели по брой редове"

echo ""
echo "== Пусни твоя скрипт =="
echo "./your_script.sh LOGDIR"
```

