#!/bin/bash

awk '/^-?[0-9]+$/ {
    nums[NR] = $0
    
    abs = $0 < 0 ? -$0 : $0
    if (abs > max) max = abs
}
END{
    for (i in nums) {
        abs = nums[i] < 0 ? -nums[i] : nums[i]
        if (abs == max) print nums[i]
    }
}' | sort -un
