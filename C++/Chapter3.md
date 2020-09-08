# [Chapter 3] 클래스
## 용어
인터페이스 함수 : 함수가 내부적으로 어떻게 상호작용하는지 사용자가 알지 않아도 되는 함수   
생성자 : 객체가 생성될 때 자동으로 호출되는 함수
소멸자 : 객체가 소멸될 때 자동으로 호출되는 함수
* * *
## 객체 지향 프로그래밍 개요
> **사용자의 편의성을 극대화 해야 한다**   
> **사용자의 실수를 제작자가 차단해야 한다**   

==> 제작자와 사용자 관점을 분리해서 생각   
사용자 관점에서? 함수가 구조체의 멤버로 들어간 것   
* * *
## 클래스 기본 문법
```c++
class 클래스이름{
    접근제어지시자:
        멤버변수선언;
        멤버함수선언및정의;
}
```
### 멤버 변수 초기화
1. 생성자   
인스턴스가 생성될 때 자동으로 호출된다.
    ```c++
    class myClass {
        myClass(){
            // 생성자 함수
            myData = 10;    // 멤버 변수 초기화
        }
        ...
    };
    ```
2. Initializer(생성자 초기화 목록)   
    ```c++
    class myClass {
        myClass()
            : myData(10), myData2(20)   // initializer
        {
            ...
        }
    }
    ```
3. 선언과 동시에   
    c++11부터 지원
    ```c++
    clas myClass{
        ...
        int myData = 10;
        int myData2 = 20;
        ...
    }
    ```
#### ❓생성자와 이니셜라이저의 차이점은   
생성자는 멤버 변수의 인스턴스가 먼저 **생성된 후**에 초기화를 진행한다.   
-> 멤버 변수가 참조형 변수이면 꼭! 이니셜라이저 사용해야 한다.   
#### ❗️참조형 멤버 변수 초기화할 때     
```c++
...
myClass(int param): myData(param) { };
...
int& myData; 
```
일 경우 `param`은 함수가 끝난 후 소멸되기 때문에 `myData`에 쓰레기 값이 들어가게 된다.
```c++
myClass(int& param): myData(param) { };
```
처럼 파라미터 또한 참조형 변수로 설정해야 한다.
### 멤버 함수(메소드) 정의 및 선언
```c++
class myClass{
    public:
        void myClassFunc(void){
            ...
        }
}
```
혹은
```c++
// 선언부 정의부 분리
class myClass{
    public:
        void myClassFunc(void)
}

void myClass::myClassFunc(){
    ...
}
```
### 접근 제어 지시자
❓사용 이유   
많은 여러가지 오류와 사용자의 실수를 방지하기 위함, 사용자가 값의 변화를 통제할 수 있음   
|종류|`public`|`protected`|`private`|
|---|---|---|---|
|외부 접근|허용|차단|차단|
|파생 클래스|허용|허용|차단|
|특징|-|-|기본 지시자|
* * *
## 생성자와 소멸자
### 생성자   
```c++
myClass()
```
클래스 객체가 생성 및 소멸될 때 **자동**으로 호출되는 함수
다중 정의 가능   
**전역 변수**로 선언한 `클래스의 생성자`가 `main()`보다 먼저 호출된다.   
* **디폴트 생성자**   
    디폴트 생성자와 소멸자를 기술하니 않아도 컴파일러가 알아서 만든다.   
    생성자와 소멸자가 없는 클래스는 없다.   
* **다중 정의**   
    매개 변수 개수, 타입 등이 다를 경우 다중 정의 가능   
    디폴트 생성자가 없이 다중 정의하게 될 경우 디폴트 생성자는 사라진다.   
    ```c++
    myClass(int param): myData(param) { };
    myClass(int x, int y): myData(x+y) { };
    ```
* **생성자 위임**   
    이니셜라이저에서 다른 생성자를 추가로 호출할 수 있다.   
    ```c++
    myClass(int x) { ... }
    myClass(int x, int y)
        : myClass(x)
        { ... }
    ```
* **명시적 디폴트 생성자**   
    `default` 예약어 사용   
    ```c++
    class myClass{
        myClass(void) = default;    // 같은 의미
        myClass(void) { };          // 같은 의미
        ...
    };
    ```   
    `delete` 예약어 사용   
    명시적으로 디폴트 생성자 삭제할 경우   
    ```c++
    myClass(void) = delete;
    ```
### 소멸자   
```c++
~myClass()
```
클래스 객체가 생성 및 소멸될 때 **자동**으로 호출되는 함수
다중 정의 불가능   
여러 개의 객체가 동시에 소멸될 때, 제일 최신 선언된 객체부터 소멸된다.      
### 동적 객체
* 생성   
    ```c++
    int main(){
        ...
        myClass myclass = new myClass;
        myClass* myclasses = new myClass[3];
    }
    ```
* 소멸
    ```c++
    int main(){
        ...
        delete myclass;
        delete[] myclasses;
    }
    ```
    배열로 동적 할당한 객체 `delete`로 소멸하게 되면 첫 번째 요소만 소멸되고 나머지는 그대로 메모리에 남음   
* * *
## 메소드(method)
클래스의 멤버 함수   
```c++
// 멤버 함수의 원형
static 반환자료형 클래스이름::함수이름(매개변수) const;
```
|종류|일반|`const`|`static`|`virtual`|
|---|---|---|---|---|
|this 포인터 접근|가능|가능|**불가능**|가능|
|일반 멤버 읽기|가능|가능|가능(제한적)|가능
|일반 멤버 쓰기|가능|**불가능**|가능(제한적)|가능
|`static` 멤버 읽기|가능|가능|가능|가능
|`static` 멤버 읽기|가능|**불가능**|가능|가능
|특징|가장 보편적인 메소드|멤버 쓰기 방지|C의 전역 함수와 유사|상속 관계에서 의미 있음
### this 포인터
`this` 포인터 : 작성 중인 클래스의 실제 인스턴스에 대한 주소를 가리키는 포인터   
메소드에서 멤버 변수를 사용할 때 `myData`라고 작성해도 컴파일러는 `this->myData`라고 컴파일한다.   
### 상수형 메소드
읽기만 허용되고 쓰기는 허용되지 않는 메소드   
`const` 예약어 사용
```c++
class myClass{
    ...
    int getData() const{
        return myData;
    }
};
```
멤버 변수의 값을 쓸 수 없고, 멤버 함수도 **상수형** 메소드만 호출할 수 있다.
* 예외 상황   
    - `mutable` 예약어   
    `mutable` 멤버 변수 값은 상수형 메소드에서도 쓰기 허용   
    ```c++
    class myClass{
        int getData() const {
            myData = 20;    // 가능
            return myData;
        }
        ...
        mutable int myData;
    };
    ```
    * `const_cast< >` 형변화 연산자   
    상수형 참조 쓰기 허용   
    ```c++
    int testFunc(const int& param){
        int& newParam = const_cast<int&>(param);
        newParam = 20;
    }
    
    int main(){
        int nData = 10;
        testFunc(nData);    // 20으로 값 변경
    }
    ```
### 메소드 다중 정의
매개 변수 값을 제어할 수 있다.   
```c++
class myClass{
    int getData() { return myData; }
    void setData(int param) { myData = param; }
    // 다중 정의
    void setData(double dParam) { dParam = 0; }
    ...
}
```
* 매개 변수 자료형 제한   
    `delete` 예약어 사용   
    ```c++
    void setData(double dParam) = delete;
    ```
* * *
## static 멤버
전역 함수 대신 사용할 수 있다.   
정적 함수는 인스턴스를 선언하지 않고 직접 호출할 수 있지만 `this` 사용 불가      
정적 변수는 선언과 정의 반드시 분리해야 한다.   
```c++
class myClass{
    ...
    static int getCount(){
        return myCount;
    }
    static int myCount;
};

// 선언, 정의 분리
int myClass::myCount = 0;

int main(){
    cout<<myClass::getCount()<<'\n';
}

```
* * *
## 연습 문제
**Q1**   
외부와 내부, 상속 클래스까지 멤버 변수나 멤버 함수에 접근할 수 없다.   
**Q2**   
생성자 함수, 반환자료형이 없다.   
**Q3**    
initializer(생성자 초기화 목록)   
**Q4**    
`m_nData`는 참조형 변수인데, 생성자 함수에서 `m_nData` 멤버 변수를 초기화하고 있어 에러가 난다.   
-> 참조형 변수는 이니셜라이저를 통해 초기화해야 한다.
```c++
class myClass{
public:
    myClass(int& param): m_nData(param) { };
    ...

private:
    int& m_nData;
};
```
**Q5**   
`this` 포인터   
**Q6**   
멤버 변수 쓰기    
상수형 메소드가 아닌 멤버 함수 호출   
**Q7**    
`this` 포인터   