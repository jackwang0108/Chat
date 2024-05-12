#pragma once

/**
 * @brief 删除fgets得到的字符串中的换行符
 * @param str fgets得到的字符串
 * @param n 字符串的长度
 * @return 删除后的字符串
 *
 * @warning 该函数会将字符串中所有的换行符替换为结尾字符, 因此要求输入的字符串必须只有一个换行符且处于末尾
 */
char *format(char str[], int n);