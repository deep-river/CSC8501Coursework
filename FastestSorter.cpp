#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

int first, last;

static void partition(vector<int>& arr, int l, int r, int x) {
	int i = l;
	first = l;
	last = r;
	while (i <= last) {
		if (arr[i] == x) {
			i++;
		}
		else if (arr[i] < x) {
			swap(arr[first++], arr[i++]);
		}
		else {
			swap(arr[last--], arr[i]);
		}
	}
}

static void quick_sort(vector<int>& arr, int l, int r) {
	if (l >= r) { return; }
	int x = arr[l + rand() % (r - l + 1)];
	partition(arr, l, r, x);
	int left = first;
	int right = last;
	quick_sort(arr, l, left - 1);
	quick_sort(arr, right + 1, r);
}

static vector<int> sort_array(vector<int>& nums) {
	if (nums.size() > 1) {
		quick_sort(nums, 0, nums.size() - 1);
	}
	return nums;
}

/*
int main() {
	vector<int> test_array{9, 13, 7, 6, 2, 8, 10, 1, 4, 5, 11, 3, 14, 12};
	for (int x : test_array) {
		cout << x << " ";
	}
	cout << endl;

	sort_array(test_array);
	for (int x : test_array) {
		cout << x << " ";
	}
	cout << endl;	

	return 0;
}
*/