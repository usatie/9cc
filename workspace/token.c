#include "cc.h"

Token *new_token(int ty, char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = ty;
  token->input = input;
  return token;
}

Token *new_token_num(int val, char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = TK_NUM;
  token->val = val;
  token->input = input;
  return token;
}

Token *new_token_eof(char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = TK_EOF;
  token->input = input;
  return token;
}
