//Compile time solution that counts the days from March 1, 1984 to a given day
//Copyright (C) 2003  Sashan Govender
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.


//I checked it against the program here http://www.timeanddate.com/date/duration.html
//I've compiled it on gcc 3.2.2, vc7.1 beta and comeau online http://www.comeaucomputing.com/tryitout/

#include <iostream>

using namespace std;

#define ENDGUARD(Value) (Value)
#define GUARD(Condition, Value) (Condition) ? (Value) : 

class DaysPerMonth
{
public:
  static const int Jan = 31;
  static const int Feb = 28;
  static const int Mar = 31;
  static const int Apr = 30;
  static const int May = 31;
  static const int Jun = 30;
  static const int Jul = 31;
  static const int Aug = 31;
  static const int Sep = 30;
  static const int Oct = 31;
  static const int Nov = 30;
  static const int Dec = 31;
};

template<int Month>
class SelectMonth;

template<int Month>
class SelectMonth
{
public:
  enum
  {
    value = GUARD(Month == 1, DaysPerMonth::Jan)
        GUARD(Month == 2, DaysPerMonth::Feb)
        GUARD(Month == 3, DaysPerMonth::Mar)
        GUARD(Month == 4, DaysPerMonth::Apr)
        GUARD(Month == 5, DaysPerMonth::May)
        GUARD(Month == 6, DaysPerMonth::Jun)
        GUARD(Month == 7, DaysPerMonth::Jul)
        GUARD(Month == 8, DaysPerMonth::Aug)
        GUARD(Month == 9, DaysPerMonth::Sep)
        GUARD(Month == 10, DaysPerMonth::Oct)
        GUARD(Month == 11, DaysPerMonth::Nov)        
        GUARD(Month == 12, DaysPerMonth::Dec)
        ENDGUARD(0)
        
  };
};

template<int Year>
class IsLeapYear;

template<int Year>
class IsLeapYear
{
public:
  enum { value = (((Year%4 == 0) && (Year%100 != 0)) || (Year%400 == 0)) ? 1 : 0 };
};

//count the leap years from 1985 to the given year - we don't worry about the 
//given year being 1984 because we only start counting from 1 March 1984
template<int Year>
class LeapYearsFrom1984;

template<>
class LeapYearsFrom1984<1985>
{
public:
  enum { value = 0 };
};

template<int Year>
class LeapYearsFrom1984
{
public:
  enum { value = IsLeapYear<Year>::value + LeapYearsFrom1984<Year-1>::value };
};

template<int Month, int Day>
class IsFeb29;

template<>
class IsFeb29<2, 29>
{
public:
  enum { value = 1 };
};

template<int Month, int Day>
class IsFeb29
{
public:
  enum { value = 0 };
};

//Is the given date before feb 29? If it is and the year is a leap year return 1 else return 0
template<int Year, int Month, int Day>
class IsBeforeFeb29;

template<int Year, int Month, int Day>
class IsBeforeFeb29
{
public:
  enum { value = (IsLeapYear<Year>::value == 1 && (Month == 1  || (Month == 2 && Day < 29))) ? 1 : 0};
};

//Count the days from the beginning of a year to a given month. Doesn't consider leap years.
template<int Month>
class DaysFromYearStart;

template<>
class DaysFromYearStart<0>
{
public:
  enum { value = 0 };
};

template<int Month>
class DaysFromYearStart
{
public:
  enum { value = SelectMonth<Month>::value + DaysFromYearStart<Month-1>::value };
};

//count the number of days from the given year to 1985. Does not cater for leap years.
template<int Year>
class DaysInWholeYears;

template<int Year>
class DaysInWholeYears
{
public:
  enum { value = 365 * (Year - 1985) };
};

//calculate the days from (not including) 1 march 1984 to the given month
template<int Month>
class DaysFrom1March1984;

template<>
class DaysFrom1March1984<3>
{
public:
  enum { value = SelectMonth<3>::value - 1 };
};

template<>
class DaysFrom1March1984<2>
{
public:
  enum { value = -1 };
};

template<int Month>
class DaysFrom1March1984
{
public:
  enum { value = SelectMonth<Month>::value + DaysFrom1March1984<Month-1>::value };
};

//Count the days from a 1 March 1984 to a given date.
template<int Year, int Month, int Day>
class CountDays;

template<int Month, int Day>
class CountDays<1984, Month, Day> 
{
public:
  enum 
  { 
    value = DaysFrom1March1984<Month-1>::value + Day
  };
};

template<int Year, int Month, int Day>
class CountDays
{
public:
  enum 
  { value = Day + DaysFromYearStart<Month-1>::value + 
        DaysInWholeYears<Year>::value + DaysFrom1March1984<12>::value +
        LeapYearsFrom1984<Year>::value - IsFeb29<Month, Day>::value - IsBeforeFeb29<Year, Month, Day>::value
  };
};

int main()
{
  cout << CountDays<1984, 3, 1>::value << endl;  //0
  cout << CountDays<1984, 3, 15>::value << endl;  //14
  cout << CountDays<1984, 3, 31>::value << endl;  //30
  cout << CountDays<1984, 4, 3>::value << endl;  //33
  cout << CountDays<1984, 5, 30>::value << endl;  //90
  cout << CountDays<1984, 12, 31>::value << endl;  //305
  cout << CountDays<1985, 1, 1>::value << endl;  //306
  cout << CountDays<1985, 2, 28>::value << endl;  //364
  cout << CountDays<1985, 3, 1>::value << endl;  //365  
  cout << CountDays<1985, 3, 31>::value << endl;  //395
  cout << CountDays<1985, 12, 31>::value << endl;  //670
  cout << CountDays<1986, 1, 1>::value << endl;  //671
  cout << CountDays<1986, 12, 31>::value << endl;  //1035
  cout << CountDays<1988, 2, 29>::value << endl;  //1460
  cout << CountDays<1996, 1, 29>::value << endl;  //4351 
  cout << CountDays<1996, 2, 29>::value << endl;  //4382
  cout << CountDays<1996, 8, 12>::value << endl;  //4547 
  cout << CountDays<2001, 12, 12>::value << endl;  //6495
  cout << CountDays<2004, 6, 30>::value << endl;  //7426    
  cout << CountDays<2004, 12, 12>::value << endl;  //7591 
  cout << CountDays<2060, 1, 1>::value << endl;  //27699  
  cout << CountDays<2060, 2, 28>::value << endl;  //27757 
  cout << CountDays<2060, 2, 29>::value << endl;  //27758  
  cout << CountDays<2088, 1, 2>::value << endl;  //37927
  cout << CountDays<2088, 2, 29>::value << endl;  //37985
  cout << CountDays<2099, 1, 1>::value << endl;  //41944  
  cout << CountDays<2099, 2, 28>::value << endl;  //42002   
  cout << CountDays<2099, 3, 1>::value << endl;  //42003    
  cout << CountDays<2099, 12, 31>::value << endl;  //42308 

  cout << "Press any key to end" << endl;
  cin.get();

  return 0;
}

