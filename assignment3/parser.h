// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <cctype>

class Node {
public:
  virtual std::string to_string() const = 0;
  virtual Node* copy() const = 0;
  int depth;

  Node() {
    depth = 0;
  }

  virtual ~Node() = default;
};

class VariableNode : public Node {
public:
  std::string name;
  Node* type; // New member for type

  VariableNode(const std::string& name, Node* type = nullptr);
  virtual ~VariableNode();
  virtual std::string to_string() const override;
  virtual Node* copy() const override {
    return new VariableNode(name, type ? type->copy() : nullptr);
  }
};

class LambdaNode : public Node {
public:
  std::string param;
  Node* type;
  Node* body;

  LambdaNode(const std::string& param, Node* type, Node* body); // Updated constructor

  std::string to_string() const override;
  Node* copy() const override {
    return new LambdaNode(param, type ? type->copy() : nullptr, body->copy());
  }

  ~LambdaNode();
};


class ApplicationNode : public Node {
public:
  Node* left;
  Node* right;
  ApplicationNode(Node* left, Node* right);

  std::string to_string() const override;
  Node* copy() const override {
    return new ApplicationNode(left->copy(), right->copy());
  }

  ~ApplicationNode();
};

class TypeNode : public Node {
public:
  std::string body;

  TypeNode(const std::string& body);
  std::string to_string() const override;
  Node* copy() const override {
    return new TypeNode(*this);
  }
};

class JudgementNode : public Node {
  Node* left;
  Node* right;

  std::string to_string() const override;
  Node* copy() const override {
    return new JudgementNode(left->copy(), right->copy());
  }

  ~JudgementNode();

public:
  JudgementNode(Node* left, Node* right);
};

enum class TokenType {
  Lambda, Arrow, LParen, RParen, Dot, End, LVar, UVar, Caret, Colon
};

struct Token {
  TokenType type;
  std::string value;
};

class Parser {
public:
    Node* parse(const std::string& input_str);
    void tokenize(const std::string& inputString);

private:
    std::string input;
    size_t pos = 0;
    std::vector<Token> tokens;

    Node* parse_expression();
    Node* parse_atom();
    Node* parse_lambda();

  Node *parse_judgement();

  Node *parse_type();

  Node *parse_single_type();
};

void print_tree(Node* node);
void assign_depth(Node* node, int depth = 0);
std::string generate_dot(Node* node);
#endif //PARSER_H