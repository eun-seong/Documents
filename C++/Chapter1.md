# [Chapter 1] C와 C++의 다른 점

## 용어
`std` : 하나의 namespace, 개념상 '소속'
`cout` : std 네임스페이스에 속한 콘솔 출력을 담당하는 객체(인스턴스)
`::` : 범위 지정 연산자
`<<` : 연산자 "함수"
`endl` : 개행과 flush 역할 동시에 수행
`instance` : c++에서 객체의 형식을 갖는 변수
* * *
### Hello World
```c++
#include <iostream>

int main(){
    std::cout << "Hello, World" << std::endl;

    return 0;
}
```
C에서 "Hello, World"를 출력하려면 `printf("%s\n", "Hello, World")`를 사용한다.   
C++에선 `%s`처럼 내가 직접 설정해야 하는 일(형식 문자, 이스케이프 시퀀스 등)이 없이 `<<` 연산자를 이용하여 출력할 수 있다. `<<`의 의미를 `std::cout`에게 문자열을 **전달**한다고 생각하면 편하다.   
* * *
### 자료형
`long long` : 64비트 정수   
`char16_t` : 16비트 문자, 유니코드 사용할 때    
`char32_t` : 32비트 문자, 유니코드 사용할 때   
`auto` : 컴파일러가 자동으로 형식을 규정하는 자료형   
`decltype(expr)` : `expr`과 동일한 자료형   
#### auto
초기값의 형식에 맞춰 선언하는 인스턴스의 형식이 **자동**으로 결정된다.    
`for loop`에서 유용하게 사용된다.   
#### decltype(expr)
`expr`과 동일한 자료형으로 자료형이 결정된다.
```c++
int x = 10;
decltype(x) y = 20; // y의 자료형은 int
```
* * *
### 메모리 동적 할당
#### new와 delete
C의 `malloc()`과 `free()` 함수와 비슷하다.    
C++의 `new`와 `delete`는 **연산자**이다.   
```c++
int *data = new int(10);
delete data;

// 배열일 경우
int *arr = new int[5];
delete[] arr;
```
배열일 경우 꼭 `delete[]`를 사용해야 한다.
`new` 연산자는 객체의 생성자를 호출하고, `delete` 연산자는 객체의 소멸자를 호출한다.   
* * *
### 참조자
참조자는 C의 포인터와 구조적으로 비슷하다. 그래서 상수에는 참조자를 선언할 수 없다(포인터 또한 불가능).   
선언과 동시에 반드시 초기화해야 한다.   
```c++
int nData = 10;
int& ref = nData;
ref = 20;   // nData의 값도 20으로 변경된다.
```
c++에선 쓸 수 있다면 포인터 대신 참조자를 사용하자   

**참조나 포인터를 굳이 왜 쓰나?**   
=> 덩치 큰 자료는 값이 아니라 참조(혹은 주소)를 전달하는 것이 효율적이기 때문   
#### r-value reference
* `l-value`
    + 연산자의 두 피연산자 중 **왼쪽**에 위치한 연산자
    + 이름이 있음
    + 명시적인 메모리 공간 확보해야 함
* `r-value`
    + 연산자의 두 피연산자 중 **오른쪽**에 위치한 연산자
    + 이름이 없음
    + 임시 결과값(연산에 활용된 직후 소멸하는 값)   

번외 : `++i`는 `l-value`이고 `i++`은 `r-value`이다. `++i`는 `i`값을 증가시켜 `i`를 반환하고, `i++`은 `i`값을 증가시키긴 하지만 증가되기 전 값을 임시보관하여 반환하기 때문이다.
* * *
### 범위 기반 for loop
배열의 사이즈를 모를 때 유용하다.
```c++
int list[5] = {1,2,3,4,5};

for(auto n : list){
    cout<< n << '\n';
}
```
* * *
### 연습 문제
**Q1**
```c++
#include <iostream>

int main(int argc, const char * argv[]) {
    std::string name;
    int age;
    std::cout<<"이름 : ";
    std::cin>>name;
    std::cout<<"나이 : ";
    std::cin>>age;
    
    std::cout<<"나의 이름은 "<<name<<"이고, "<<age<<"살입니다.\n";
}
```
**Q2**
`auto`는 컴파일러가 자료형을 자동으로 결정해주는 예약어이다.
```c++
#include <iostream>

int main(int argc, const char * argv[]) {
    auto integer = 5;
    auto std = "deveun";
    auto character = 'a';
    
    std::cout << typeid(integer).name() << std::endl;
    std::cout << typeid(std).name() << std::endl;
    std::cout << typeid(character).name() << std::endl;
}
```
**Q3**
```c++
#include <iostream>

int main(int argc, const char * argv[]) {
    char* str = new char[12];
    delete[] str;
}
```
**Q4**
```c++
#include <iostream>

void Swap(int& a, int& b){
    int tmp = b;
    b = a;
    a = tmp;
}

int main(int argc, const char * argv[]) {
    int a = 10;
    int b = 20;
    
    std::cout<<"a : "<<a<<"\nb : "<<b<<'\n';

    Swap(a, b);
    std::cout<<"a : "<<a<<"\nb : "<<b<<'\n';
}
```
**Q5**
...? 상수형 참조? `const int& ref = a`를 말하는 건가?   
`a`에 대한 참조만 할 수 있고 `ref`로는 값 변경이 불가능하며, `a`가 변경되면 `ref`도 변경된다.   

**Q6**
내림차순 정렬함
```c++
#include <iostream>

int main(int argc, const char * argv[]) {
    int aList[5] = {10, 20, 30, 40, 50};
    
    for(int i=0;i<5;i++){
        for(int j=1;j<5-i;j++){
            int& left = aList[j-1];
            int& right = aList[j];
            if(left<right){
                int tmp = left;
                left = right;
                right = tmp;
            }
        }
    }
    
    for(auto n : aList){
        std::cout<<n<<' ';
    }
}
```