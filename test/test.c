//
// Created by reprise on 24-12-2.
//
#include "../include/expr.h"
#include "../include/common.h"

void TestExpr(){
    char str[2 * MAX];
    char expression[MAX];
    bool success = true;
    int ans;
    // 打开文件expr.txt
    FILE *fp = fopen("../test/exprTest.txt", "r");
    if(fp == NULL){
        printf("File not found\n");
        return;
    }
    // 读取文件中的表达式，是否有效指示位，以及正确答案
    while(fgets(str, 2 * MAX, fp) != NULL){
        // 使用strtok分割字符串
        char *token = strtok(str, ";");
        if (token != NULL) {
            strncpy(expression, token, MAX);
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            success = atoi(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            ans = atoi(token);
        }
        // 计算表达式的值
        bool success_test = true;
        int res = expr(expression, &success_test);
        // 判断是否与正确答案相同
        if(res == ans && success_test == success){
            printf("Correct\n");
        }else{
            printf("Wrong\n");
        }
    }
}

void TestCheckParentheses(){
    bool success = true;
    bool valid = true;
    char str[2 * MAX];
    char expression[MAX];
    // 打开文件parentheses.txt
    FILE *fp = fopen("../test/checkParenthesesTest.txt", "r");
    if(fp == NULL){
        printf("File not found\n");
        return;
    }
    // 读取文件中的表达式，是否有效指示位，以及正确答案
    while(fgets(str, 2 * MAX, fp) != NULL){
        // 使用strtok分割字符串
        char *token = strtok(str, ";");
        if (token != NULL) {
            strncpy(expression, token, MAX);
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            valid = atoi(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            success = atoi(token);
        }
        // 判断括号是否匹配
        bool success_test = true;
        bool valid_test = true;
        if (!make_token(expression)) {
            printf("Invalid expression\n");
        }
        bool res = check_parentheses(0, strlen(expression), &valid_test);
        // 判断是否与正确答案相同
        if(res == success && valid_test == valid){
            printf("Correct\n");
        }else{
            printf("Wrong\n");
        }
    }
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        TestExpr();
        TestCheckParentheses();
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "e") == 0) {
                TestExpr();
            } else if (strcmp(argv[i], "p") == 0) {
                TestCheckParentheses();
            }
        }
    }
    return 0;
}