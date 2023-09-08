#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>
using namespace std;

// bubble_sort   n2,n2,n,1,Y

// insert_sort   n2,n2,n,1,Y
// shell_sort   n1.3,n2,n,1,N 

// select_sort   n2,n2,n2,1,N
// heap_sort   nlogn,nlogn,nlogn,1,N

// quick_sort   nlogn,n2,nlogn,nlogn,N
// merge_sort   nlogn,nlogn,nlogn,n,Y

// count_sort   n+k,n+k,n+k,n+k,Y
// bucket_sort   n+k,n2,n,n+k,Y
// radix_sort   n*k,n*k,n*k,n+k,Y

// 平均，最坏，最好，空间，稳定
void bubble_sort(vector<int>& nums) { // n2,n2,n,1,Y
    int n = nums.size();
    // 第几轮，每一轮确定一个最大值
    for (int i = 0; i < n-1; i++) {
        bool ok = true;
        for (int j = 0; j < n-i-1; j++) {
            if (nums[j] > nums[j+1]) {
                swap(nums[j], nums[j+1]);
                ok = false;
            }
        }
        if (ok) break;
    }
}

void insert_sort(vector<int>& nums) { // n2,n2,n,1,Y
    int n = nums.size();
    // 前i个数排序好了，每一轮将nums[i]插入到排序好的子数组
    for (int i = 1; i < n; i++) {
        if (nums[i] >= nums[i-1]) continue;
        int key = nums[i];
        int j = i;
        for (; j >= 1 && key < nums[j-1]; j--) {
            nums[j] = nums[j-1];
        }
        nums[j] = key;
    }
}

void shell_sort(vector<int>& nums) { // n1.3, n2, n, 1, N 
    int n = nums.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int key = nums[i];
            int j = i;
            for (; j >= gap && key < nums[j-gap]; j -= gap) {
                nums[j] = nums[j-gap];
            }
            nums[j] = key;
        }
    }
}

void select_sort(vector<int>& nums) { // n2,n2,n2,1,N
    int n = nums.size();
    // 第几轮，每一轮在未排序子数组里选一个最小值
    for (int i = 0; i < n; i++) {
        int mi = i;
        for (int j = i; j < n; j++) {
            if (nums[j] < nums[mi]) {
                mi = j;
            }
        }
        swap(nums[i], nums[mi]);
    }
}

void heap_sort(vector<int>& nums) { // nlogn,nlogn,nlogn,1,N
    int n = nums.size();
    // function<void(void)> heap_build = [&](){};
    // heap_build();
    make_heap(nums.begin(), nums.end());
    function<void(int, int)> heap_adjust = [&](int a, int b){};
    for (int i = n-1; i > 0; i--) {
        swap(nums[0], nums[i]);
        heap_adjust(0, i-1);
    }
}

void merge_sort(vector<int>& nums) {}

void quick_sort(vector<int>& nums) {}



int main() {
    int n = 10;
    vector<int> nums(n);
    cout << "Ori nums: " << endl;
    for (int i = 0; i < n; i++) {
        nums[i] = rand() % 100;
        cout << nums[i] << ' ';
    }
    cout << endl;

    shell_sort(nums);
    cout << "Sorted nums: " << endl;
    for (auto& num : nums) {
        cout << num << ' ';
    }
    return 0;
}