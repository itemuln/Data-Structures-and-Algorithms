double findMaxAverage(int* nums, int numsSize, int k) {
long long sum = 0;
for (int i = 0; i < k; i++) {
    sum += nums[i];
}

long long maxSum = sum;

for (int i = k; i < numsSize; i++) {
    sum += nums[i] - nums[i - k];
    if (sum > maxSum) {
        maxSum = sum;
    }
}

return (double)maxSum / k;

}
