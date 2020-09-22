# [Chapter 5] 연산자 다중 정의
## 용어
연산자 함수 :    
연산자 다중 정의 :    
* * *
## 연산자 함수
연산자를 이용하듯 호출할 수 있는 메소드   
클래스에서 다중 정의를 하게 되면 기본 자료형처럼 연산을 할 수 있음   
### 형변환 연산자
```c++
operator 변환할자료형()
```
### 덧셈 연산자
```c++
클래스이름 operator+(const 클래스이름& param)
```
클래스 형식을 리턴하므로 이동 생성자 호출   
`void`를 반환하게 되면 연속으로 연산 불가능   
다중 정의 가능 -> `클래스이름 operator+(다른자료형 param)`   
### 대입 연산자
#### 단순 대입 연산자
```c++
클래스이름& operator=(const 클래스이름& param){
    // 자신에 자신을 대입할 경우 대비
    if(this == &param) return;
    
    // 깊은 복사 수행
    delete mData;
    mData = new int(*param.mData);

    return *this;
}
```
`void`를 반환하게 되면 연속으로 연산 불가능   
#### **이동 대입 연산자**
```c++
클래스이름& operator=(const 클래스이름&& param){
    // 얕은 복사 수행
    mData = param.mData;
    param.mData = nullptr;

    return *this;
}
```
이동 대입 연산자를 선언하지 않으면 단순 대입 연산자를 호출   
-> 임시 객체가 생성되므로 좋지 않음   
#### 복합 대입 연산자
```c++
클래스이름& operator+=(const 클래스이름& param)
```
### 배열 연산자
#### **l-value, r-value 모두 사용 가능**
```c++
int& operator[] (int index);
```
#### **상수형 참조로만 호출, r-value로만 사용 가능**
```c++
int operator[] (int index) const;
```
### 관계 연산자
상등, 부등, 비교 연산자
```c++
int 클래스이름::operator==(const 클래스이름& param);
int 클래스이름::operator!=(const 클래스이름& param);
```
배열 비교 단위에서 많이 사용   
### 단항 증감 연산자
#### 전위식
```c++
자료형 operator++(){
    return ++(데이터);
}
```
증가한 후 증가된 객체를 리턴 -> l-value
#### 후위식
```c++
자료형 operator++(자료형){
    새 객체 생성 및 자료형 복사;
    데이터++;

    return 새 객체
}
```
증가되기 전 임시 객체를 리턴 -> r-value
## 연습 문제
**Q1**   
자신에 자신을 대입할 경우 처리 - 쓰레기 값이 들어갈 수 있음   
이동 대입 연산자 생성 - 임시 객체 생성을 막고 이동 시멘틱이 일어날 수 있게 해줌    
**Q2**   
`void`가 아닌 자기 자신의 참조   
**Q3**   
```c++
자료형 operator++(자료형)
```