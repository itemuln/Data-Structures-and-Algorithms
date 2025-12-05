#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

/**
 * Finds the maximum average of a contiguous subarray of length k
 * @param nums Array of integers
 * @param numsSize Size of the array
 * @param k Length of the subarray
 * @return Maximum average value
 */
double findMaxAverage(int *nums, int numsSize, int k);

#endif // SLIDING_WINDOW_H

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
