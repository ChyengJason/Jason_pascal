//-std=c++11
#include<algorithm>
#include<cctype>
#include "scanner.h"
#include "error.h"

namespace jasonpascal
{
  Scanner::Scanner(const std::string& srcFileName)
      :fileName_(srcFileName),line_(1),column_(0),
       currentChar_(0),errorFlag_(false),state_(State::NONE)
  {
    input_.open(fileName_);
    if(input_.fail())
    {
      errorToken("when trying to open file"+fileName_+",ocurred error");
      errorFlag_=true;
    }
  }

  void Scanner::getNextChar()
  {
    currentChar_=input_.get();
    if(currentChar_=='\n')
    {
      line_++;
      column_=0;
    }
    else
    {
      column_++;
    }
  }

  char Scanner::peekChar()
  {
    char c=input_.peek();
    return c;
  }

  void Scanner::addToBuffer(char c)
  {
    buffer_.push_back(c);
  }

  void Scanner::reduceBuffer()
  {
    buffer_.pop_back();
  }

  void Scanner::makeToken(TokenType tt,TokenValue tv,
                          const TokenLocation& loc,std::string name,
                          int symbolPrecedence)
  {
    token_=Token(tt,tv,loc,name,symbolPrecedence);
    buffer_.clear();
    state_=State::NONE;
  }

  void Scanner::makeToken(TokenType tt,TokenValue tv,
                          const TokenLocation& loc,long intValue,
                          std::string name)
  {
    token_=Token(tt,tv,loc,intValue,name);
    buffer_.clear();
    state_=State::NONE;
  }

  void Scanner::makeToken(TokenType tt, TokenValue tv,
      const TokenLocation& loc, double realValue, std::string name)
  {
    token_ = Token(tt, tv, loc, realValue, name);
    buffer_.clear();
    state_ = State::NONE;
  }

  void Scanner::preprocess()
  {
    do
    {
      while(std::isspace(currentChar_))
      {
        getNextChar();
      }
      handleLineComment();
      handleBlockComment();
    }while(std::isspace(currentChar_));
  }

  void Scanner::handleLineComment()
  {
    loc_=getTokenLocation();
    if(currentChar_=='(' && peekChar()=='*')
    {
      getNextChar();
      getNextChar();
      while(!(currentChar_=='*' && peekChar()==')'))
      {
        getNextChar();
        if(input_.eof())
        {
          errorToken(getTokenLocation().toString()+"end of file happened in comment, *) is expected! but find "+ currentChar_);
          errorFlag_=true;
          break;
        }
      }

      if(!input_.eof())
      {
        getNextChar();
        getNextChar();
      }
    }
  }

  void Scanner::handleBlockComment()
  {
    loc_ = getTokenLocation();
    if(currentChar_=='{')
    {
      do
      {
        getNextChar();
        if(input_.eof())
        {
          errorToken(getTokenLocation().toString() + "end of file happended in comment, } is expected!, but find " + currentChar_);
          errorFlag_ = true;
          break;
        }
      }while(currentChar_!='}');

      if(!input_.eof())
      {
        getNextChar();
      }
    }
  }

  Token Scanner::getNextToken()
  {
      bool matched=false;

      do
      {
        if(state_!=State::NONE)
        {
          matched=true;
        }
        switch(state_)
        {
          case State::NONE:
            getNextChar();
            break;

          case State::END_OF_FILE:
            handleEOFState();
            break;

          case State::IDENTIFIER:
            handleIdentifierState();
            break;

          case State::STRING:
            handleStringState();
            break;

          case State::OPERATION:
            handleOperationState();
            break;

          default:
            errorToken("Match token state error");
            errorFlag_ = true;
            break;
        }

        if(state_==State::NONE)
        {
          preprocess();

          if(input_.eof())
          {
            state_ = State::END_OF_FILE;
          }
          else
          {
            if(std::isalpha(currentChar_))
            {
              state_ = State::IDENTIFIER;
            }
            else if(std::isdigit(currentChar_)||
                (currentChar_=='$'))
            {
              state_ = State::NUMBER;
            }
            else if(currentChar_=='\'')
            {
              state_ = State::STRING;
            }
            else
            {
              state_ = State::OPERATION;
            }
          }
        }
      }while(!matched);
      return token_;
  }

 
void Scanner::handleEOFState()
    {
        loc_ = getTokenLocation();
        makeToken(TokenType::END_OF_FILE, TokenValue::UNRESERVED,
                  loc_, std::string("END_OF_FILE"), -1);
        // close the file
        input_.close();
    }

  void Scanner::handleStringState()
  {
    loc_ = getTokenLocation();

    getNextChar();

    while(true)
    {
      if(currentChar_ == '\'')
      {
        if(peekChar() == '\'')
        {
          getNextChar();
        }
        else
        {
          break;
        }
      }

      addToBuffer(currentChar_);
      getNextChar();
    }
    getNextChar();
    if(buffer_.length()==1)
    {
      makeToken(TokenType::CHAR, TokenValue::UNRESERVED, loc_,
          static_cast<long>(buffer_.at(0)), buffer_);
    }
    else
    {
      makeToken(TokenType::STRING_LITERAL, TokenValue::UNRESERVED,
          loc_, buffer_,-4);
    }
  }

  void Scanner::handleIdentifierState()
    {
        loc_ = getTokenLocation();
        // add first char
        addToBuffer(currentChar_);
        getNextChar();

        while (std::isalnum(currentChar_) || currentChar_ == '_')
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
        // end while. currentChar_ is not alpha, number and _.

        // keyword or not
        // because Pascal is not case sensitive
        // we should transform it to lower case
        std::transform(buffer_.begin(), buffer_.end(), buffer_.begin(), ::tolower);
        // use dictionary to judge it is keyword or not
        auto tokenMeta = dictionary_.lookup(buffer_);
        makeToken(std::get<0>(tokenMeta), std::get<1>(tokenMeta), loc_, buffer_, std::get<2>(tokenMeta));
    }

    void Scanner::handleOperationState()
    {
        loc_ = getTokenLocation();
        bool matched = false;
        // add current symbol char
        addToBuffer(currentChar_);
        // add next one symbol char
        addToBuffer(peekChar());

        if (dictionary_.haveToken(buffer_))
        {
            matched = true;
            getNextChar();
        }
        else
        {
            reduceBuffer();
        }

        auto tokenMeta = dictionary_.lookup(buffer_);
        // token type, token value, name, symbol precedence
        makeToken(std::get<0>(tokenMeta), std::get<1>(tokenMeta), loc_, buffer_, std::get<2>(tokenMeta));
        // update currentChar_
        getNextChar();
    }

    void Scanner::handleDigit()
    {
        // add first number of integer
        addToBuffer(currentChar_);
        getNextChar();

        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
        // end while. currentChar_ is not digit.

        // notice maybe currentChar_ is .(dot) or E/e,
        // so the NumberState can be changed into
        // NumberState::Fraction or NumberState::Exponent.
    }

    void Scanner::handleXDigit()
    {
        // notice: we have eat $ and update currentChar_
        // in the handleNumber function. so we need not
        // eat currentChar_ like handleDigit function.
        // only have $ or not
        bool readFlag = false;

        while (std::isxdigit(currentChar_))
        {
            readFlag = true;
            addToBuffer(currentChar_);
            getNextChar();
        }

        if (!readFlag)
        {
            errorToken(getTokenLocation().toString() + "Hexadecimal number format error.");
            errorFlag_ = true;
        }
    }

    void Scanner::handleFraction()
    {
        // currentChar_ is . (dot)

        // if we have number 4..12. just simple error condition.
        // our compiler has one big difference compared with
        // commercial compiler, that is about error conditions.
        if (peekChar() == '.')
        {
            errorToken(getTokenLocation().toString() + "Fraction number can not have dot after dot");
            errorFlag_ = true;
        }

        // eat .
        addToBuffer(currentChar_);
        getNextChar();

        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }

    void Scanner::handleExponent()
    {
        // eat E/e
        addToBuffer(currentChar_);
        getNextChar();

        // if number has +/-
        if (currentChar_ == '+' || currentChar_ == '-')
        {
            addToBuffer(currentChar_);
            getNextChar();
        }

        while (std::isdigit(currentChar_))
        {
            addToBuffer(currentChar_);
            getNextChar();
        }
    }


}
