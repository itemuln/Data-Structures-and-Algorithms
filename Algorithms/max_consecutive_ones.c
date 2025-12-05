#include "max_consecutive_ones.h"
int longestOnes(int* nums, int numsSize, int k) {
int left = 0;
int zeros = 0;
int best = 0;
for (int right = 0; right < numsSize; right++) {
    if (nums[right] == 0) zeros++;

    while (zeros > k) {
        if (nums[left] == 0) zeros--;
        left++;
    }

    int win = right - left + 1;
    if (win > best) best = win;
}

return best;

}
