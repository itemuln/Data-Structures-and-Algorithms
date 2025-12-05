#include "sliding_window.h"
//The code takes array and its size. Sliding window algorithm works by eleminating
// extra loop inside a loop by removing unneccessary elements 1 by 1 making it faster than
// O(n^2) time complexity. The algorithm works by doing what its needed on the first k elements
// then doing neccessary changes 1 by 1 to the first k elements in the loop.
double findMaxAverage(int *nums, int numsSize, int k)
{
    long long sum = 0;
    for (int i = 0; i < k; i++)
    {
        sum += nums[i];
    }

    long long maxSum = sum;

    for (int i = k; i < numsSize; i++)
    {
        sum += nums[i] - nums[i - k];
        if (sum > maxSum)
        {
            maxSum = sum;
        }
    }

    return (double)maxSum / k;
}
