//
// Created by root on 1/15/19.
//

#ifndef STATION_SORT_FUNC_H
#define STATION_SORT_FUNC_H

/*
* 快速排序
*
* 参数说明：
*     a -- 待排序的数组
*     left -- 数组的左边界(例如，从起始位置开始排序，则l=0)
*     right -- 数组的右边界(例如，排序截至到数组末尾，则r=a.length-1)
*/
void quick_sort_int(int a[],int left,int right);

void quick_sort_str(char *,int, int);

int  Bubble_sort_string(char *src);

int  Select_sort_string(char *str);

int  Insert_sort_string(char *str);

int BucketSort(char *string,int size);
#endif //STATION_SORT_FUNC_H
