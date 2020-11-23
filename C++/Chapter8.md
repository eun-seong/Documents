# [Chapter 8] 수평적 관계와 집합 관계
## 용어

* * *
## friend
* 선언   
    `friend` 예약어 사용
    ```c++
    friend class 클래스이름;
    friend 함수원형선언;
    ``` 
* 접근 제어 지시자의 영향 X
* **응집성**을 위해서 존재
### `friend` 함수
```c++
class CMyData{
    ...
    // 전역 함수를 friend로 선언
    friend void PrintData(const CMyData& a);
}

void PrintData(const CMyData& a){
    cout << "printData(): " << rData.m_nData << '\n';
}
```
### `friend` 클래스
* 많은 경우 중에 자료와 관리 방법 에서 사용
    => ex) 노드와 리스트
* like 연예인 - 매니저
    완전히 다른 객체이지만 관련이 깊은 관계
* * *
## 집합 관계
### Composition
* 사전적 의미 : `구성 요소`
* 인스턴스가 사라지면 **구성 요소들 모두 *함께 소멸***
* 모여서 뗄 수 없는 한 덩어리
* 예시
    ```c++
    자동차
    |--- 엔진
    |--- 핸들
    |--- 변속기
    ```
#### 실습
```c++
class CNode {
    friend class CMyList;
    ...
}

class CMyList {
    ...
    void Print() {
        // 리스트의 모든 노드 값 출력
    }
}

class CMyUI {
    ...
    private:
    CMyList m_list;     // CMyList 생성
}

int main() {
    CMyUI ui;   // CMyUI만 생성
    ui.Run();
}
```
`CMyUI`를 생성하면 `CMyList`가 생성됨   
`CMyUI`가 소멸되면 `CMyList`도 함께 소멸됨
### Aggregation
* 사전적 의미 : `집합체`
* 모두 모여 하나의 시스템을 이루지만 각각 개별적으로 분리되어 **독립적**으로 활용
* 각자 독립적인 것들이 모여 만들어진 분리 가능 집합체
* 인스턴스가 소멸되어도 구성요소들은 소멸되지 않음
* 예시
    ```c++
    컴퓨터 시스템
    |- 모니터
    |- 본체
    |- 키보드
    ```
#### 실습
```c++
...

class CMyUI {
    public:
    CMyUI(CMyList& rList) : m_list(rList) { }
    ...
    private:
    CMyList& m_list;
}

int main() {
    // List와 UI 객체 따로 선언
    CMyList list;
    CMyUI ui(list);
    ui.Run();
}
```
`CMyList`와 `CMyUI`가 독립적인 객체로 선언   
> Aggregation 관계가 되는 것이 Composition 관계가 되는 것보다 좋을 때가 많다.
    프로그램의 제어 시스템, 사용자 인터페이스, 원본 데이터는 분리하는 것이 원칙이기 때문
* * *
## 연습 문제
**Q1**   
화장실 마음대로 쓸 수 있음
**Q2**   
Composition
