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

bool make_token(const char str[]) {   // �������ַ���ת��Ϊ tokens�� �Ѿ�Ϊ��ʵ�ֺ��˶ԼӼ��˳������ֵ�ƥ�䡣
                                    // ��������ʵ����ʹ��������ʽ��ƥ�䣬�����ǵ� win ��û�� regex.h�ļ�����ʹ��һ��ѭ���������жϡ�
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
            while (isdigit(str[i])) {  // �������������ַ�ת��Ϊһ������
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

bool check_parentheses(int start, int end, bool *success) { // �÷������ڼ�� tokens ������ĳһ����(start <=, <= end)�Ƿ�һ��ƥ������ſ�ס
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
        /* �������������ʽ����*/
        *success = false;
        return 0;
    }
    if (p == q) {
        /* ֻ��һ�� token, ��ʱ��� token Ӧ����һ�����֣� ֻ��Ҫ���� token �� value ���ؼ���*/
        if (tokens[p].type == TK_NUM) {
            return tokens[p].value;
        }
        /* ���������Ϊ�������� */
        *success = false;
        return 0;
    }
    if (check_parentheses(p, q, success) == true) {
        /* �ж�������ʽ�Ƿ�һ�����Ű�Χ
         * ����ǣ���ȥ��������ţ����¼�����ʽ��ֵ
         */
        return eval(p + 1, q - 1, success);
    }
    int op = -1, op_type = -1;
    int flag = 0;
    for (int i = p; i <= q; i++) {
        // Ѱ�ұ��ʽ����Ҫ�����
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
    if (flag != 0) { // ���Ų�ƥ��
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
    /* TODO: ������ʽ��ֵ */
    return eval(0, num_token - 1, success);
}