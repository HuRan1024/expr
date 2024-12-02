//
// Created by reprise on 24-12-2.
//
#include "../include/common.h"
#include "../include/expr.h"

token tokens[MAX];

enum {
    TK_NUM, TK_NEGATIVE
    };

int num_token;

bool make_token(const char str[]) {   // 将输入字符串转换为 tokens， 已经为你实现好了对加减乘除和数字的匹配。
                                    // 在这里其实可以使用正则表达式来匹配，但考虑到 win 上没有 regex.h文件，故使用一个循环来进行判断。
    num_token = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            i++;
            continue;
        }
        if (str[i] == '+') {
            tokens[num_token].type = '+';
            tokens[num_token++].value = 0;
        } else if (str[i] == '-') {
            if(num_token != 0 && tokens[num_token - 1].type == TK_NUM || tokens[num_token - 1].type == ')') {
                tokens[num_token].type = '-';
                tokens[num_token++].value = 0;
            } else {
                tokens[num_token].type = TK_NEGATIVE;
                tokens[num_token++].value = 0;
            }
        } else if (str[i] == '*') {
            tokens[num_token].type = '*';
            tokens[num_token++].value = 0;
        } else if (str[i] == '/') {
            tokens[num_token].type = '/';
            tokens[num_token++].value = 0;
        } else if (str[i] == '(') {
            tokens[num_token].type = '(';
            tokens[num_token++].value = 0;
        } else if (str[i] == ')') {
            tokens[num_token].type = ')';
            tokens[num_token++].value = 0;
        } else if (isdigit(str[i])) {
            tokens[num_token].type = TK_NUM;
            tokens[num_token].value = 0;
            while (isdigit(str[i])) {  // 将连续的数字字符转换为一个整数
                tokens[num_token].value = tokens[num_token].value * 10 + str[i] - '0';
                i++;
            }
            num_token++;
            i--;
        } else {
            printf("Invalid character\n");
            return false;
        }
        i++;
    }
    return true;
}

bool check_parentheses(int start, int end, bool *success) { // 该方法用于检测 tokens 数组中某一部分(start <=, <= end)是否被一对匹配的括号框住
    if(end > num_token) end = num_token - 1;
    int num = 0;
    bool res = true;
    for (int i = start; i <= end; i++) {
        if (tokens[i].type == '(') {
            num++;
        } else if (tokens[i].type == ')') {
            if (num == 0) {
                *success = false;
                return false;
            }
            num--;
            if (num == 0 && i != end) {
                res = false;
            }
        }
    }
    if (num != 0) {
        *success = false;
        return false;
    }
    if (tokens[start].type != '(' || tokens[end].type != ')') {
        res = false;
    }
    return res;
}

int eval(int p, int q, bool *success) {
    if (p > q) {
        /* 错误的情况（表达式有误）*/
        *success = false;
        return 0;
    }
    if (p == q) {
        /* 只有一个 token, 此时这个 token 应该是一个数字， 只需要将该 token 的 value 返回即可*/
        if (tokens[p].type == TK_NUM) {
            return tokens[p].value;
        }
        /* 其他情况均为错误的情况 */
        *success = false;
        return 0;
    }
    if (check_parentheses(p, q, success) == true) {
        /* 判断这个表达式是否被一对括号包围
         * 如果是，则去掉这对括号，重新计算表达式的值
         */
        return eval(p + 1, q - 1, success);
    }
    int op = -1, op_type = -1;
    int flag = 0;
    for (int i = p; i <= q; i++) {
        // 寻找表达式的主要运算符
        if (tokens[i].type == '(') {
            flag++;
        } else if (tokens[i].type == ')') {
            flag--;
        }
        if (flag == 0) {
            if (tokens[i].type == '+' || tokens[i].type == '-') {
                op_type = tokens[i].type;
                op = i;
            } else if (tokens[i].type == '*' || tokens[i].type == '/') {
                if (op_type != '+' && op_type != '-') {
                    op_type = tokens[i].type;
                    op = i;
                }
            }
        }
    }
    if (flag != 0) { // 括号不匹配
        *success = false;
        return 0;
    }
    if (op == -1) {
        if (tokens[p].type == TK_NEGATIVE) {
            return -1 * eval(p + 1, q, success);
        }
        success = false;
        return 0;
    }

    int val1 = eval(p, op - 1, success);
    int val2 = eval(op + 1, q, success);

    if (*success == false) {
        return 0;
    }
    switch (op_type) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case '/': {
            if (val2 == 0)
                assert(0);
            return val1 / val2;
        }
        default: {
            *success = false;
            return 0;
        };
    }
}

int expr(char *e, bool *success) {
    if (!make_token(e)) {
        *success = false;
        return 0;
    }
    /* TODO: 计算表达式的值 */
    return eval(0, num_token - 1, success);
}