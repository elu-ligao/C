/*
 * @Author       : elu
 * @Date         : 2022-01-10 16:59:40
 * @LastEditTime : 2022-01-11 17:12:54
 * @LastEditors  : Please set LastEditors
 * @Description  : 
 */
#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

#include "list.h"

List PolynomialCreate(PtrElem PElemArray, int N); // 创建多项式链表
List PolynomialAdd(List A, List B);   // 多项式 A + B
List PolynomialSub(List A, List B);   // 多项式 A - B
List PolynomialMult(List A, List B);  // 多项式 A / B
List PolynomialPow(List A, int N);  // 多项式 A^N

void ShowPolynomial(List L);
void DeletePolynomial(List L);

#endif