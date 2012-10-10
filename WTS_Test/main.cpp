#include <iostream>
//#Print Hello World! Function, to demonstrate function creation
//#the line below is a one-word comment
//#one-word
//#another line
int print_hello_func() {
std::cout << "Hello World!\n" ;return 0;
}
int math_func() {
int a;
a = 2;
a = ( a + ( a * 2 ) );
while ( ( a < 10 ))
{
a = ( a + 1 );
if ( ( a == 4 ))
{
std::cout << "A is equal to 4!\n" ;}
if ( ( a != 10 ))
{
std::cout << "A is not equal to 10!\n" ;}
std::cout << "A is equal to " ;std::cout << a ;std::cout << "\n" ;}
return 0;
}
//#Main Function, entrence point of program
int main() {
print_hello_func();
math_func();
return 0;
}
