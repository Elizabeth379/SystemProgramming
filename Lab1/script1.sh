#!/bin/bash

print_time() {
    local time=$(date +%T)
    tput cup $1 $2
    echo "$time"
}

# Функция для генерации случайных чисел на основе мультипликативного алгоритма
# Инициализация от текущего времени
random_number() {
    seed=$(date +%s)
    multiplier=1103515245
    increment=12345
    modulus=$((2**31))
    seed=$((($multiplier * $seed + $increment) % $modulus))
    echo $seed
}

# Получение размеров окна
max_x=$(tput cols)
max_y=$(tput lines)

while true; do
    x=$(($(random_number) % max_x))
    y=$(($(random_number) % max_y))

    clear
    
    print_time $y $x

    sleep 5
done
