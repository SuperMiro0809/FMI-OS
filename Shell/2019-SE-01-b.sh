#!/bin/bash

awk '/^-?[0-9]+$/ {
    nums[NR] = $0
    n = $0 < 0 ? -$0 : $0

    sum = 0
    while (n > 0) {
        sum += n % 10
        n = int(n / 10)
    }
    
    digit_sum[NR] = sum
    if (sum > max_sum) max_sum = sum
}
END {
    min=""
    for (i in nums) {
        if (digit_sum[i] == max_sum) {
            if (min == "" || nums[i] < min) min = nums[i]
        }
    }
    print min
}'
