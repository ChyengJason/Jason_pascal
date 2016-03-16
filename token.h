#ifndef TOKEN_H_
#define TOKEN_H_

#include<string>
#include<iostream>
#include<tuple>
#include<map>

namespace jasonpascal
{
  enum class TokenType //强枚举类
  {
    INTEGER,//整型
    REAL,//浮点型
    BOOLEAN,//布尔型
    CHAR,//字符型
    STRING_LITERAL,//literal 字符串型

    IDENTIFIER,//比如abc
    KEYWORDS,//比如if
    OPERATORS,//比如+
    DELIMITER,//比如,
    END_OF_FILE,//文件结束
    UNKNOWN
  };

  enum class TokenValue
  {
    AND,
    FOR,
    TO,
    DOWNTO,
    DO,
    IF,
    THEN,
    ELSE,
    WHILE,
    REPEAT,
    UNTIL,
    FUNCTION,
    PROCEDURE,
    BEGIN,
    END,
    PROGRAM,
    FORWARD,
    GOTO,
    OR,
    NOT,
    CASE,
    OTHERWISE,
    WITH,
    IN,
    //I/O
    WRITE,
    WRITELN,
    READ,
    READLN,
    //TYPE/CAR/CONST
    TYPE,
    VAR,
    PACKED,
    ARRAY,
    OF,
    RECORD,
    CONST,
    FILE,
    SET,
    STRING,

    //SYSMBOLS
    LEFT_PAREN,       //(
    RIGHT_PAREN,      //)
    LEFT_SQUARE,      //[
    RIGHT_SQUARE,     //]
    PLUS,             //+
    MINUS,            //-
    MULTIPLY,         //*
    DIVIDE,           ///
    COMMA,            //,
    SEMICOLON,        //;
    COLON,            //:
    ASSIGN,           //:=
    PERIOD,           //.
    DOT_DOT,          //..
    UPARROW,          //^
    DIV,              //DIV
    MOD,              //MOD
    XOR,              //XOR
    SHR,              //SHR
    SHL,              //SHL

    //comparation symbols
    LESS_OR_EQUAL,    //<=
    LESS_THAN,        //<
    GREATER_OR_EQUAL, //>=
    GREATER_THAN,     //>
    EQUAL,            //=
    NOT_EQUAL,        //<>

    UNRESERVED
  };

  class TokenLocation//
  {
    public:
      TokenLocation();
      TokenLocation(const std::string& fileName,int line,int colum);

      std::string toString() const;//返回值是const
    private:
      std::string fileName_;
      int line_;
      int column_;
  };

  class Token
  {
    public:
      Token();
      Token(TokenType type,TokenValue value,const TokenLocation& location,
          std::string name,int symbolPrecedence);
      Token(TokenType type,TokenValue value,const TokenLocation& location,
          const std::string& strValue,std::string name);
      Token(TokenType type,TokenValue value,const TokenLocation& location,
          long intValue,std::string name);
      Token(TokenType type,TokenValue value,const TokenLocation& location,
          double realValue,std::string name);

      //get information
      TokenType getTokenType() const;
      TokenValue getTokenValue()const;
      const TokenLocation& getTokenLocation() const;
      std::string getTokenName()const;

      //+ -* / so on
      int getSymbolPrecedence()const;

      //get constant values of token
      long getIntValue()const;
      double getRealValue() const;
      std::string getStringValue()const;

      //output debug information
      void dump(std::ostream& out=std::cout)const;

      std::string tokenTypeDescription()const;
      std::string toString()const;

    private:
      TokenType type_;
      TokenValue value_;
      TokenLocation location_;
      int symbolPrecedence_;
      std::string name_;

      //const values of token
      long intValue_;
      double realValue_;
      std::string strValue_;
  };

  inline TokenType Token::getTokenType()const
  {
    return type_;
  }

  inline TokenValue Token::getTokenValue()const
  {
    return value_;
  }

  inline const TokenLocation& Token::Token::getTokenLocation()const
  {
    return location_;
  }

  inline std::string Token::getTokenName()const
  {
    return name_;
  }

  inline long Token::getIntValue()const
  {
    return intValue_;
  }

  inline double Token::getRealValue()const
  {
    return realValue_;
  }

  inline std::string Token::getStringValue()const
  {
    return strValue_;
  }

  inline int Token::getSymbolPrecedence()const 
  {
    return symbolPrecedence_;
  }

}
#endif//







