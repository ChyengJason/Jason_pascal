#include"token.h"

namespace jasonpascal
{
  TokenLocation::TokenLocation():fileName_(""),line_(1),column_(0)
  {}

   TokenLocation::TokenLocation(const std::string& fileName, int line, int column)
  :fileName_(fileName),line_(line),column_(column)
  {}

  std::string TokenLocation::toString()const
  {
    return fileName_+"::"+std::to_string(line_)+","+std::to_string(column_);
  }
  //end TokenLocation

  Token::Token() : type_(TokenType::UNKNOWN),value_(TokenValue::UNRESERVED),
  location_(std::string(""),0,0),name_(""),symbolPrecedence_(-1)
  {}

  Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
      std::string name, int symbolPrecedence)
      : type_(type),value_(value),location_(location),name_(name),
      symbolPrecedence_(symbolPrecedence)
  {}

  Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
      const std::string& strValue, std::string name)
      : type_(type),value_(value),location_(location),strValue_(strValue),
      name_(name),symbolPrecedence_(-1)
  {}

  Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
      long intValue, std::string name)
      : type_(type),value_(value),location_(location),intValue_(intValue),
      name_(name),symbolPrecedence_(-1)
  {}

  Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
      double realValue, std::string name)
      : type_(type),value_(value),location_(location),realValue_(realValue),
      name_(name),symbolPrecedence_(-1)
  {}

  std::string Token::tokenTypeDescription()const
  {
    std::string buffer;
    switch(type_)
    {
      case jasonpascal::TokenType::INTEGER:
           buffer = "integer:       ";
           break;
      case jasonpascal::TokenType::REAL:
           buffer = "real:          ";
           break;
      case jasonpascal::TokenType::CHAR:
           buffer = "char:          ";
           break;
      case jasonpascal::TokenType::STRING_LITERAL:
           buffer = "string literal:";
           break;
      case jasonpascal::TokenType::IDENTIFIER:
           buffer = "identifier:    ";
           break;
      case jasonpascal::TokenType::KEYWORDS:
           buffer = "keywords:      ";
           break;
      case jasonpascal::TokenType::OPERATORS:
           buffer = "operators:     ";
           break;
      case jasonpascal::TokenType::DELIMITER:
           buffer = "delimiter:     ";
           break;
      case jasonpascal::TokenType::END_OF_FILE:
           buffer = "eof:           ";
           break;
      default:
           break;
    }
    return buffer;
  }

  void Token::dump(std::ostream& out)const
  {
    out << location_.toString() << "\t" << tokenTypeDescription()
         << "\t" << name_ << "\t" << getSymbolPrecedence() << std::endl;
  }
  //end token

}

