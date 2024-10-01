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

static void quickSort(vector<int>& arr, int l, int r) {
	if (l >= r) { return; }
	int x = arr[l + rand() % (r - l + 1)];
	partition(arr, l, r, x);
	int left = first;
	int right = last;
	quickSort(arr, l, left - 1);
	quickSort(arr, right + 1, r);
}

static vector<int> sortArray(vector<int>& nums) {
	if (nums.size() > 1) {
		quickSort(nums, 0, nums.size() - 1);
	}
	return nums;
}

/*
int main() {
	vector<int> testArray{9, 13, 7, 6, 2, 8, 10, 1, 4, 5, 11, 3, 14, 12};
	for (int x : testArray) {
		cout << x << " ";
	}
	cout << endl;

	sortArray(testArray);
	for (int x : testArray) {
		cout << x << " ";
	}
	cout << endl;	

	return 0;
}
*/