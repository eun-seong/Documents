# [Chapter 6] 상속 기본
## 용어
상속 : 객체 단위 코드를 재사용하는 방법    
재정의 : 기존의 선언 및 정의된 코드를 유지하면서 새롭게 바꾸는 방법   
메소드 재정의 : 클래스에 있는 메소드를 자유롭게 재정의    
* * *
## 상속
객체 단위 코드를 재사용하는 방법   
상속 받는 클래스와 상속하는 클래스 사이의 **관계**를 고려해 프로그램 작성
### 기본 형식   
```c++
class 파생클래스이름 : 접근제어지시자 부모클래스이름
```
### `public` 접근 제어 지시자를 통한 상속
* 파생 클래스 인스턴스가 생성되면 [기본 클래스의 생성자 호출](#생성자와-소멸자)   
* 파생 클래스는 `private` 접근 제어 지시자로 선언된 멤버를 제외한 기본 클래스의 멤버에 접근 가능    
    [식별자 검새 순서에 근거](./Chapter2.md/#식별자-검색-순서)
* 사용자 코드에서 파생 클래스의 인스턴스를 통해 기본 클래스 메소드 호출 가능   
* * *
## 메소드 재정의(Override)
기존 클래스의 메소드와 새로 정의된 메소드가 공존하며 새 메소드가 기존 메소드를 대체   
(X) 기존 코드 제거   
(O) 기존 메소드와 새 메소드 한데 묶어 작동   
### 기본 문법
```c++
class CMyData{
    public:
    void SetData(int nParam){ mData = nParam; }
    ...
};

class CMyDataEx : public CMyData{
    public:
    // 함수 재정의
    void SetData(int nParam){
        if(nParam < 0) CMyData::SetData(0);
        if(nParam > 10) CMyData::SetData(10);
        // **주의** 재귀호출 당연하쥬?
        // SetData(0);
    }
};
```
#### 부모 클래스 멤버 메소드 명시적 호출
```c++
CMyDataEx b;
b.SetData(15);              // CMyDataEx::SetData() 함수 호출
b.CMyData::SetData(15);    // CMyData::SetData() 함수 호출
```
### 참조 형식과 실형식
포인터도 가능
```c++
int main(){
    CMyDataEx a;
    CMyData& rData = a;
    rData.SetDat(15);   // CMyData::SetData() 함수 호출
    cout<< rData.GetData() << '\n';
}
```
* * *
## 생성자와 소멸자
### 실행 순서
```c++
Class A { };
Class B : public A { };
Class C : public B { };
```
```c++
C c;
```
#### 생성자
호출 순서 : C -> B -> A   
실행 순서 : A -> B -> C   
호출과 실행 순서가 **역순**   
자식 클래스에서 생성자를 실행하기 전 부모 클래스의 생성자 호출    
#### 소멸자
호출 순서 : C -> B -> A   
실행 순서 : C -> B -> A   
호출과 실행 순서가 **동일**   
자식 클래스의 소멸자에서 리턴하기 전 부모 클래스의 소멸자 호출    
#### 생성자와 소멸자는 객체 자신의 초기화 및 해제
* 파생 클래스는 부모 클래스의 멤버 변수에 직접 쓰기 연산하지 않아야 함
* 파생 클래스 생성자에서 부모 클래스 멤버 변수를 초기화하지 않아야 함
### 생성자 선택
이니셜라이저를 통해 상위 클래스 생성자를 호출하듯이 기술하는 것   
부모 클래스와 자식 클래스에서 생성자가 다중 정의되어 있을 경우 생성자 선택 가능   
```c++
class CMyData{
    public:
    CMyData() { ... }
    CMyData(int nParam) { ... }
    CMyData(double dParam) { ... }
}

class CMyDataEx: public CMyData{
    public:
    CMyDataEx() { }
    CMyDataEx(int nParam) : CMyData(nParam) { ... }
    DMyDataEx(int dParam) : DMyData(dParam) { ... }
}
```
#### 생성자 상속
부모 클래스의 생성자를 그대로 자식 클래스로 가져올 때 사용   
```c++
class CMyDataEx : public CMyData{
    public:
    using CMyData::CMyData;
}
```
* * *
## 연습 문제
**Q1**   
기본 형식      
**Q2**   
A   