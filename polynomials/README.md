# Polynomials

This task is mainly about [Operator Overloading](https://en.wikipedia.org/wiki/Operator_overloading).

To make the program efficient [Fast Fourier Transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform) was implemented.

## Task Description

The task is to develop a C++ class CPolynomial representing polynomials of arbitrary degree.

The class will store individual coefficients (floating point numbers - double precision), moreover, public interface will offer overloaded operators and methods to manipulate polynomials:

**default constructor** the constructor will prepare an object representing polynomial 0,
**copy constructor** the constructor will be implemented if your internal representation requires it,
**destructor** will be present if your internal representation requires it,
**overloaded operator=** the operator will copy the contents from one instance to another (if the automatically generated op= cannot be used with your implementation).
**operator <<** will output a textual form of the polynomial into an output stream. The output formatting must follow these rules:

* the polynomial will be displayed in a decreasing order of powers, i.e. from the highest power of x,terms with zero coefficient are not displayed in the output,
* terms with coefficient +1 and -1 are displayed without the coefficient (just a power of x),
* there are not unnecessary - in the output (i.e. x^1 - 9 or - x^2 + 4 is OK, whereas x^1 + (-9) is not),
* zero polynomial will be displayed as 0.

The variable displayed in the output shall be x by default. There is manipulator, the manipulator may be used to change the default x to any other variable (bonus test only).

**operator +** adds two polynomials,
**operator -**subtracts two polynomials,
**operator \***multiplies given polynomial either with a double, or with another polynomial,
**operators == and !=** compare two polynomials,
**operator []** is used to access (read / write) individual coefficients, the term is determined by the index (0 = absolute, 1 = x, 2 = x^2, ... ). The read form must be available even on const instances,
**operator ()** evaluates the value of the polynomial for the given value x (x is a double),
**Degree()** the method returns the degree of the polynomial (e.g. x^3+4 has degree of 3, 5 has degree 0, specifically 0 has degree 0).
**manipulator polynomial_variable ( name )** This manipulator changes the name of the variable use to display the polynomial. The default is to display the polynomial with variable x. The manipulator may be used, to change the variable name to any string (the parameter). The implementation of the manipulator is an extension - the manipulator is tested only in the bonus tests. If you do not intent to pass the bonus tests, keep the sample implementation of the manipulator. The delivered implementation of polynomial_variable does not do anything useful, it is just present, thus programs that use the manipulator actually compile.

Submit a source file with your implementation of CPolynomial class. The submitted file shall not contain any #include directives nor main function. If your main function or #include remains in the file, please place them into a conditional compile block.

This task does not provide the required class interface. Instead, you are expected to develop the interface yourself. Use the description above, the attached examples, and your knowledge of overloaded operators.
