//
// Created by root on 19-3-23.
//

#ifndef STATION_JUDGE_NUM_H
#define STATION_JUDGE_NUM_H
#ifdef  __cplusplus
extern "C" {
#endif

  //判断一个数是不是奇数
  int Judge_num_isOdd(int num);
  //判断一个数是否为素数 素数还有一个特点，就是它总是等于 6x-1 或者 6x+1，其中 x 是大于等于1的自然数
  int Judge_num_isPrime(int num);


#ifdef  __cplusplus
}
#endif
#endif //STATION_JUDGE_NUM_H
