/*
 * @Author       : elu
 * @Date         : 2022-01-11 15:46:23
 * @LastEditTime : 2022-01-12 10:37:42
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _BIGNUM_H
#define _BIGNUM_H

#include "list.h"

#define BASE    ( 1000 )      // 基数

typedef List BigNum;

BigNum BigNumCreate(PtrElem PElemArray, int N); // 创建多项式链表
BigNum BigNumAdd(BigNum A, BigNum B);   // 多项式 A + B
BigNum BigNumSub(BigNum A, BigNum B);   // 多项式 A - B
BigNum BigNumMult(BigNum A, BigNum B);  // 多项式 A / B
BigNum BigNumPow(BigNum A, int N);  // 多项式 A^N
BigNum BigNumFormat(int N);     // 将N转换为BigNum 类型
void BigNumCarry(BigNum L, int Exp, int N);   // 对多项式A的指数为Exp的结点进位N
void BigNumCheckCarry(BigNum L);   // 对多项式A的指数为Exp的结点校验是否要进位

void ShowBigNum(BigNum L);
void DeleteBigNum(BigNum L);


BigNum BigNumFormatString(char *s); // 将数字串s转换为BigNum 类型
BigNum BigNumFormat(int N);         // 将N转换为BigNum 类型

#endif