# Working Days

## Task Description

The task is to implement two functions (not a whole program, just two functions) which handle working day computations. The required functions are:

int IsWorkDay ( int y, int m, int d )
    The function decides whether the date (given in the parameters) is a working day, or not. The parameters are year, month, and day to test. Return value is integer 1 if the date is a working day, or integer 0 otherwise (the date is Sunday, Saturday, a national holiday, or the date is invalid).
int CountWorkDays ( int y1, int m1, int d1, int y2, int m2, int d2, int * cnt )
    The function shall count working days in the given interval of days. The interval is given by the start and end dates. Start date is formed by year, month, and day y1, m1, and d1, end date is formed by y2, m2, and d2. The interval is considered closed, i.e. both boundary days are included in the computation. Return value is either 1 to indicate success or 0 to indicate an error. If the function succeeds, it places the resulting number of working days into the output parameters cnt. The function fails if the input parameters are invalid (see below).

The input parameters must define a valid date. To be valid, the input must satisfy the following restrictions:

    year must be greater than or equal to 2000 (i.e. all dates before 2000-01-01 are invalid),
    month is valid (1 to 12),
    day is valid (1 to the number of days in the month),
    in CountWorkDays: the end timestamp must not precede the start timestamp.

Submit a source file with the implementation of both required functions (IsWorkDay and CountWorkDays). Further, the source file must include your auxiliary functions which are called from the required functions.The function will be called from the testing environment, thus, it is important to adhere to the required interface. Use the attached sample code as a basis for your development, complete the required functions and add your required auxiliary functions. There is an example main with some test in the attached code. These values will be used in the basic test. Please note the header files as well as main is nested in a conditional compile block (#ifdef/#endif). Please keep these conditional compile block in place. They are present to simplify the development. When compiling on your computer, the headers and main will be present as usual. On the other hand, the header and main will "disappear" when compiled by Progtest. Thus, your testing main will not interfere with the testing environment's main.

Your function will be executed in a limited environment. There are limits on both time and memory. The exact limits are shown in the test log of the reference. The time limits are set such that a correct implementation of the naive solution passes all mandatory tests. Thus, the solution may be awarded nominal 100%. The algorithm must be improved to pass the bonus tests and gain the extra points. There are long intervals tested in the bonus tests (years exceed 4000 several orders of magnitude).
Advice:

    Monday to Friday are considered working days. Moreover, national holidays are not considered working days. We use the following list of national holidays (actually the national holidays in the Czech Republic):

        1.1, 1.5, 8.5, 5.7, 6.7, 28.9, 28.10, 17.11, 24.12, 25.12, and 26.12
       

    Please note Easter is not included in the list. The computation of Easter is a bit more complicated (Moon phase is required), thus Easter is not used in this homework.

    We assume standard Gregorian calendar when counting days. Thus, there is a fixed number of days (30/31) in a month, with the exception of February. February is either 28 days (non-leap year) or 29 days (a leap year). The leap year rules of Gregorian calendar are:
        years are not leap years in general,
        except multiples of 4 which are leap years,
        except multiples of 100 which are not leap years,
        except multiples of 400 which are leap years,
        except multiples of 4000 which are not leap years.

    Thus, years 2001, 2002, 2003, 2005, ... are not leap years (rule #1), years 2004, 2008, ..., 2096, 2104, ... are leap years (rule #2), years 2100, 2200, 2300, 2500, ... are not leap years (rule #3), years 2000, 2400, ..., 3600, 4400, ... are leap years (rule #4), and years 4000, 8000, ... are not leap years (rule #5).
    Download the attached sample code and use it as a base for your development.
    The main function in your program may be modified (e.g. a new test may be included). The conditional compile block must remain, however.
    It is difficult to handle 3-tuples like (year, month, day). It is a good idea to convert the 3-tuple into some other representation, e.g. into a single integer.
    There will be a lot of computation done twice in the program: for the start and end timestamps. Thus, it is a good idea to prepare auxiliary functions and call them twice.
    The years in the mandatory tests do not exceed 2200.
    There is macro assert used in the example main function. If the value passed to assert is nonzero (true), the macro does nothing. On the other hand, if the parameter is zero, the macro stops the execution and reports line, where the test did not match (and shall be fixed). Thus, the program ends silently when your implementation passes the tests correctly.

