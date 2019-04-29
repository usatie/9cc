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

Token *new_token_id(char *input) {
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

Vector *tokenize(char *p) {
	Vector *tokens = new_vector();

	while (*p) {
		if (isspace(*p)) {
			p++;
			continue;
		}

		// == or !=
    if (!strncmp(p, "==", 2)) {
      Token *token = new_token(TK_EQ, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }
    if (!strncmp(p, "!=", 2)) {
      Token *token = new_token(TK_NE, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }
    if (!strncmp(p, "<=", 2)) {
      Token *token = new_token(TK_LE, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }
    if (!strncmp(p, ">=", 2)) {
      Token *token = new_token(TK_GE, p);
      vec_push(tokens, token);
			p++;
			p++;
			continue;
    }

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>') {
      Token *token = new_token(*p, p);
      vec_push(tokens, token);
			p++;
			continue;
		}

    if ('a' <= *p && *p <= 'z') {
      Token *token = new_token_id(p);
			vec_push(tokens, token);
      p++;
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
