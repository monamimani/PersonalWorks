#include "TestUtilities/TestStruct.benchmark.h"

namespace Test
{

void BMStructOutOfLine::fct(int& x)
{
  x = 0;
  for (int i = 0; i < value; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::fctConst(int& x) const
{
  x = 0;
  for (int i = 0; i < value; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::fctConstOverloaded(int& x)
{
  x = 0;
  for (int i = 0; i < value; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::fctConstOverloaded(int& x) const
{
  x = 0;
  for (int i = 0; i < value + 1; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::fctParamOverloaded(int& x) const
{
  x = 0;
  for (int i = 0; i < value + 1; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::fctParamOverloaded(double& x) const
{
  x = 0;
  for (int i = 0; i < value + 1; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::operator()(int& x)
{
  x = 0;
  for (int i = 0; i < value; i++)
  {
    x++;
  }
}

void BMStructOutOfLine::operator()(int& x) const
{
  x = 0;
  for (int i = 0; i < value + 1; i++)
  {
    x++;
  }
}

void freeFunctionOutOfLine(int& x)
{
  x = 0;
  for (int i = 0; i < BMStructOutOfLine::staticValue; i++)
  {
    x++;
  }
}
} // namespace Test