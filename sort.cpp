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
        bool NoChanage = true;
        for (int j = 0; j < n-i-1; j++) {
            if (nums[j] > nums[j+1]) {
                swap(nums[j], nums[j+1]);
                NoChanage = false;
            }
        }
        if (NoChanage) break;
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
    // 根据gap来插排
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

void merge_sort(vector<int>& nums) { // nlogn,nlogn,nlogn,n,Y
    int n = nums.size();
    // 提前创建一个全局tmp数组，节省空间
    vector<int> tmp(n);
    // 分成[lft, mid]与[mid+1,rgt]两段，分别排序后再合并
    function<void(int, int, int)> merge = [&](int lft, int mid, int rgt) {
        int k = lft, i = lft, j = mid+1;
        while (i <= mid && j <= rgt) {
            if (nums[i] < nums[j]) {
                tmp[k++] = nums[i++];
            } else {
                tmp[k++] = nums[j++];
            }
        }
        while (i <= mid)
            tmp[k++] = nums[i++];
        while (j <= rgt)
            tmp[k++] = nums[j++];
        for (int i = lft; i <= rgt; i++)
            nums[i] = tmp[i];
    };
    // 分合
    function<void(int, int)> MergeSort = [&](int lft, int rgt) {
        if (lft < rgt) { // lft 有可能 == rgt
            int mid = ((rgt - lft) >> 1) + lft;
            MergeSort(lft, mid);
            MergeSort(mid+1, rgt);
            merge(lft, mid, rgt);
        }
    };

    MergeSort(0, n-1);
}

void quick_sort(vector<int>& nums) { // nlogn,n2,nlogn,nlogn,N
    int n = nums.size();
    function<int(int, int)> partition = [&](int lft, int rgt) {
        /* randomPartition （随机一个数为基准数）*/
        int i = rand() % (rgt - lft + 1) + lft;
        // 不随机的话i就固定是rgt
        swap(nums[i], nums[rgt]);
        // 先把基准数放在最后rgt上
        int x = nums[rgt], pivotIdx = lft; // x为基准数，pivotIdx是基准数最后的位置
        for (int j = pivotIdx; j < rgt; j++) { // 小于基准数的放左边，大于的放右边
            if (nums[j] < x) {
                swap(nums[pivotIdx++], nums[j]); // 小于基准数的放到前面来
            }
        }
        swap(nums[pivotIdx], nums[rgt]); // 将基准数放到该放的地方(左边数都小,右边数都大)
        return pivotIdx;
    };

    function<void(int, int)> QuickSort = [&](int lft, int rgt) {
        if (lft < rgt) {
            int pivotIdx = partition(lft, rgt);
            QuickSort(lft, pivotIdx - 1);
            QuickSort(pivotIdx + 1, rgt);
        }
    };

    QuickSort(0, n-1);
}

// 数的范围确定且较小时使用，可认为是特殊的桶排
void count_sort(vector<int>& nums) { // n+k,n+k,n+k,n+k,Y
    // 处理空数组
    if (nums.empty()) return;
    // 确定范围(如果已知range就省略，比实际范围更大的range也行)
    int mi = nums[0], mx = nums[0];
    for (auto num : nums) {
        if (num < mi) mi = num;
        if (num > mx) mx = num;
    }
    int range = mx - mi + 1;
    vector<int> cnts(range);
    // 计数
    for (auto num : nums) {
        cnts[num - mi]++;
    }
    // 按照计数排序
    int pos = 0;
    for (int i = 0; i < range; i++) {
        while (cnts[i] > 0) {
            nums[pos++] = i+mi;
            cnts[i]--;
        }
    }
}

// 数据分布均匀时使用
void bucket_sort(vector<int>& nums) { // n+k,n2,n,n+k,Y
    // 处理空数组
    if (nums.empty()) return;
    // 确定桶的数量
    int mi = nums[0], mx = nums[0];
    for (auto num : nums) {
        if (num < mi) mi = num;
        if (num > mx) mx = num;
    }
    int numBuckets = mx - mi + 1;
    vector<vector<int>> buckets(numBuckets);
    // 把数装进桶里
    for (int num : nums) {
        buckets[num - mi].emplace_back(num);
    }
    // 分别在桶内排序
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
    }
    // 将桶中的数装回数组
    int pos = 0;
    for (auto& bucket : buckets) {
        for (int num : bucket) {
            nums[pos++] = num;
        }
    }
}

// 限定条件：数的最大位数相同时，且复杂度较高，少用
void radix_sort(vector<int>& nums) { // n*k,n*k,n*k,n+k,Y
    function<int(int, int)> getDigit = [](int num, int digit) {
        for (int i = 0; i < digit; i++) {
            num /= 10;
        }
        return num % 10;
    };
    // 处理空数组
    if (nums.empty()) return;
    // 确定最大位数（这里用十进制）
    int mx = *max_element(nums.begin(), nums.end());
    int numDigits = 0;
    while (mx > 0) {
        mx /= 10;
        numDigits++;
    }
    // 创建桶
    vector<vector<int>> buckets(10);
    // 从最低位到最高位排序
    for (int digit = 0; digit < numDigits; digit++) {
        // 把数分配进桶里
        for (int num : nums) {
            int digitVal = getDigit(num, digit);
            buckets[digitVal].emplace_back(num);
        }
        // 把桶中的数装回数组
        int pos = 0;
        for (auto& bucket : buckets) {
            for (int num : bucket) {
                nums[pos++] = num;
            }
            bucket.clear();
        }
    }
}


int main() {
    int n = 10;
    vector<int> nums(n);
    cout << "Ori nums: " << endl;
    for (int i = 0; i < n; i++) {
        nums[i] = rand() % 100;
        cout << nums[i] << ' ';
    }
    cout << endl;

    radix_sort(nums);
    cout << "Sorted nums: " << endl;
    for (auto& num : nums) {
        cout << num << ' ';
    }
    return 0;
}