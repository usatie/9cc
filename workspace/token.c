#include "9cc.h"

static Token *new_token(int ty, char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = ty;
  token->input = input;
  return token;
}

static Token *new_token_num(int val, char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = TK_NUM;
  token->val = val;
  token->input = input;
  return token;
}

static Token *new_token_id(char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = TK_IDENT;
  token->input = input;
  return token;
}

Token *new_token_eof(char *input) {
  Token *token = malloc(sizeof(Token));
  token->ty = TK_EOF;
  token->input = input;
  return token;
}

static bool is_alnum(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') || (c == '_');
}

static bool expect(char *c) { return true; }

Vector *tokenize(char *p) {
  Vector *tokens = new_vector();

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Return
    if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
      Token *token = new_token(TK_RETURN, p);
      vec_push(tokens, token);
      p += 6;
      continue;
    }

    // if
    if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
      Token *token = new_token(TK_IF, p);
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    // else
    if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
      Token *token = new_token(TK_ELSE, p);
      vec_push(tokens, token);
      p += 4;
      continue;
    }

    // while
    if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
      Token *token = new_token(TK_WHILE, p);
      vec_push(tokens, token);
      p += 5;
      continue;
    }

    // while
    if (strncmp(p, "for", 3) == 0 && !is_alnum(p[3])) {
      Token *token = new_token(TK_FOR, p);
      vec_push(tokens, token);
      p += 3;
      continue;
    }

    // == or !=
    if (!strncmp(p, "==", 2)) {
      Token *token = new_token(TK_EQ, p);
      vec_push(tokens, token);
      p += 2;
      continue;
    }
    if (!strncmp(p, "!=", 2)) {
      Token *token = new_token(TK_NE, p);
      vec_push(tokens, token);
      p += 2;
      continue;
    }
    if (!strncmp(p, "<=", 2)) {
      Token *token = new_token(TK_LE, p);
      vec_push(tokens, token);
      p += 2;
      continue;
    }
    if (!strncmp(p, ">=", 2)) {
      Token *token = new_token(TK_GE, p);
      vec_push(tokens, token);
      p += 2;
      continue;
    }

    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' ||
        *p == ')' || *p == '<' || *p == '>' || *p == ';' || *p == '=' ||
        *p == '{' || *p == '}' || *p == ',') {
      Token *token = new_token(*p, p);
      vec_push(tokens, token);
      p++;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      int i = 1;
      while (is_alnum(*(p + i))) {
        i++;
      }
      char *name = malloc(i);
      strncpy(name, p, i);
      Token *token = new_token_id(p);
      token->name = name;
      vec_push(tokens, token);
      p += i;
      continue;
    }

    if (isdigit(*p)) {
      Token *token = new_token_num(strtol(p, &p, 10), p);
      vec_push(tokens, token);
      continue;
    }

    error("Cannot tokenize: %s", p);
    exit(1);
  }

  Token *token = new_token_eof(p);
  vec_push(tokens, token);

  return tokens;
}
