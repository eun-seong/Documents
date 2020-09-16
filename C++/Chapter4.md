# [Chapter 4] 복사 생성자와 임시 객체
## 용어
복사 생성자 :    
깊은 복사 :    
얕은 복사 :    
임시 객체 :    
이동 시맨틱 :    
* * *
## 복사 생성자
객체의 복사본을 생성할 때 호출되는 생성자   
디폴트 복사 생성자 : 복사 생성자를 생략하면 디폴트 생성자처럼 컴파일러가 '알아서' 만들어 줌   
클래스 내부에서 멤버 변수에 포인터 변수를 동적 할당 및 해제할 경우 조심해야 함   
#### 복사 생성자가 호출되는 경우
1. 명시적으로 객체의 복사본을 생성하는 방식으로 선언   
2. 함수 형태로 호출 : 매개변수로 사용
3. 함수 형태로 호출 : 반환 타입으로 사용
```c++
// 매개변수가 상수형 참조
클래스이름(const 클래스이름 &rhs);
```
#### 복사 생성자 차단 방법
1. 명시적으로 복사 생성자 삭제
    ```c++
    ...
    Class(const Class& rhs) = delete;
    ...
    ```
2. 호출 함수 매개변수를 참조형으로 (👍추천 방식)     
    매개변수는 **꼭**`const`형으로 선언해야 한다.
    ```c++
    void function(const Class& param){
        // 매개변수가 참조형이므로 객체가 생성되지 않는다.
        param.setData(20);  // 원본 객체 수정
    }
    ```
### 함수 호출과 복사 생성자의 문제점
1. 클래스를 매개변수로 사용할 경우   
    ```c++
    void function(Class param){
        // param 객체의 복사 생성자가 호출되어 객체가 생성된다.
        param.setData(2);   // 당연히 원본은 바뀌지 않는다.
        // param 객체가 소멸한다.
    }
    ```
    => 참조형 매개변수로 해결 가능
2. 클래스를 반환 형식으로 사용되는 경우
    ```c++
    
    ```
### 얕은 복사
대상이 되는 값 하나에 접근 포인터가 둘이 되는 것   
클래스에 포인터 멤버변수가 있고 메모리 동적 할당 및 해제할 경우 오류가 날 수 있다.   
예제) a와 b의 멤버 포인터 변수가 하나의 메모리를 가리키게 됨
```c++
class mClass{
    mClass(int param){
        mData = new int;
        *mData = param;
    }
    
    ~mClass(int param){
        delete mData;
    }

    // 포인터 멤버 데이터
    int *mData = nullprt;
};

int main(){
    mClass a(10);   // 디폴트 생성자
    mClass b(a);    // 디폴트 복사 생성자
}
// 컴파일러가 디폴트 복사 생성자 만듦
```
=> 메모리 해제 시 b가 해제된 후 a가 이미 b가 해제한 메모리를 해제하려고 시도하기 때문에 에러 발생   
### 깊은 복사
복사에 의해 실제로 두 개의 값이 생성되는 것   
위의 예시 문제 해결 가능   
```c++
class mClass{
    mClass(int param){
        mData = new int;
        *mData = param;
    }

    mClass(const& mClass &rhs){
        mData = new int;        // 새 객체 생성

        *mData = *rhs.mData;    // 값만 복사
    }
    
    ~mClass(int param){
        delete mData;
    }

    // 포인터 멤버 데이터
    int *mData = nullprt;
};

int main(){
    mClass a(10);   // 디폴트 생성자
    mClass b(a);    // 복사 생성자 
}
```
=> 에러 안남
### 대입 연산자
클래스를 단순 대입하게 되면 모든 멤버의 값을 그대로 복사한다.
```c++
mClass a(10);
mClass b(20);

a = b;
// 깊은 복사가 수행되지 않아 에러 발생   
```
#### 단순 대입 연산자 함수 정의
```c++
mClass& operator=(const mClass &rhs){
    *mData = *rhs.mData;

    return *this;
}
```
* * *
## 묵시적 변환
### 변환 생성자
매개변수가 한 개인 생성자   
은근슬쩍 호출되는 경우가 있음 -> 불필요한 임시 객체 생성함   
```c++
class mClass{
    mClass(int nParam): mData(nParam){}
    ...
};

void function(const mClass& param){
    // 형 변환으로 인해 변환 생성자 호출 -> 불필요한 객체 생성
}

int main(){
    function(5);    // 매개변수 int -> mClass로 형변환
}
```
-> `function(mClass(5))` 가 되어버린 셈
#### `explicit` 예약어
묵시적 변환 생성자가 사용자 모르게 호출될 가능성을 차단
```c++
...
explicit mClass(int nParam): mData(nParam) { }
...
int main(){
    function(5);    // 컴파일 에러
}
```
### 허용되는 변환
변환 생성자는 클래스를 A 형식으로 변환될 수 있지만, A 형식을 클래스 형식으로 변환하지는 못함   
-> 형변환 연산자 생성
```c++
class mClass{
    explicit mClass(int nParam): mData(nParam) { }

    // 형변환 연산자
    operator int(void) {return mData;}  
};

int main(){
    mClass a(10);
    
    cout<<a<<'\n';  // int -> mClass 형변환이 되었기 때문에 가능
}
```
|형변환 연산자||
|---|---|
|`const_cast`|형 변환|
|`static_cast`|상속 관련|
|`dynamic_cast`|상속 관련|
|`reinterpret_cast`|C 스타일의 형변환((int)a) 가급적 쓰지 않는게 좋음|
* * *
## 이름 없는 임시 객체
함수의 리턴 타입이 클래스인 경우에 아무도 모르게 객체가 생성되고 소멸된다   
#### 중요
1. 임시 객체의 존재를 인식하는 것
    ```c++
    mClass function(int nParam){
        mClass a(nParam);
        
        return a;
    }

    int main(){
        mClass b(5);

        // 함수가 리턴되면서 임시 객체가 생성됐다가 대입 연산 후 소멸된다
        b = function(10);
    }
    ```    
2. 생성 및 소멸 시기를 정확하게 인식하는 것
    ```c++
    int main(){
        // 임시 객체는 main()함수가 리턴된 후 소멸한다
        mClass &rData = function(10);

        return 0;
    }
    ```
### r-value 참조
연산자 다중 정의에서 사용
```c++
class mClass{
    public:
    mClass(int param){
        this->mData = param;
    }
    
    int mData;
};

// 매개 변수가 r-value 참조 클래스
void testFunc(mClass &&nParam){
    cout<<"testFunc(): "<<nParam.mData<<'\n';
}

int main(){
    testFunc(mClass(10));

    return 0;
}
```
### 이동 시멘틱
1. 이동 생성자   
    어차피 사라진 이름 없는 임시 객체이므로 깊은 복사가 아닌 얕은 복사를 수행
    ```c++
    class mClass{
        ...
        // 이동 생성자
        // 매개변수가 r-value 참조
        mClass(mClass &&rhs): mData(rhs.mData){ }
        ...
    };

    mClass testFunc(int nParam){
        mClass a;
        a.setData(nParam);

        return a;
    }

    int main(){
        mClass b;

        b = testFunc(20);   // 이동 생성자 호출
        mClass c(b);        // 복사 생성자 호출

        return 0;
    }
    ```
2. 이동 대입 연산자   
    [대입 연산자](#대입-연산자)
* * *
## 연습 문제
**Q1**   
`const 참조형`   
**Q2**   
매개 변수에 포인터 멤버 변수가 있는 클래스   
**Q3**   
묵시적 형변환이 int -> CTestData 로 발생하고 있다.   
이는 `explicit 변환 생성자`를 선언함에 따라 해결할 수 있다.   
**Q4**   
리턴타입이 클래스인 함수에서 객체의 생성과 소멸이 이루어진다. 객체에 메모리가 할당되고 바로 소멸되기 때문에 비효율적이다.   
이 때문에 객체에 메모리를 할당하지 않고 얕은 복사를 수행하는 이동 시멘틱이 등장했다.   
