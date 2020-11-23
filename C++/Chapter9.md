# [Chapter 8] 템플릿
## 용어

* * *
## 클래스 템플릿
클래스 템플릿 : 클래스를 찍어내는 모양자   
* 선언
    ```c++
    template<typename T>
    class 클래스이름 {

    }
    ```
* 함수 템플릿처럼 비슷하게 사용
### 멤버 선언 및 정의
```c++
template<typename T>
클래스이름<T>::멤버함수이름() {

}
```
### 템플릿 매개 변수
* 선언
    ```c++
    template<typename T, typename T2>
    template<typename T, int nSize = 3>
    ```
* 사용
    ```c++
    CMyArray<int, 4> arr;
    ```
* * *
## 템플릿 특수화
### 함수 템플릿 특수화
```c++
template<typename T>
T Add(T a, T b) { return a + b; }

template< >
char* Add(char* pszLeft, char* pszRight) {
    ...
}
```
매개변수가 모두 `char*`이면 두 번째 함수로 대체
### 클래스 템플릿 특수화
```c++
template<typename T>
class CMyData {
    ...
}

template< >
class CMyData<char*> {
    ...
}
```
* * *
## 클래스 템플릿과 상속
```c++
template<typename T>
class CMyDataEx : public CMyData<T> {
    ...
}
```
## 스마트 포인터
|스마트 포인터|설명|
|---|---|
|`auto_ptr`|인스턴스 자동으로 삭제, 웬만해선 쓰지마라|
|`shared_ptr`|포인팅 횟수 계산해서 0이되면 삭제|
|`unique_ptr`|하나의 소유자만 허용|
|`weak_ptr`|특수한 경우에만 사용|
* * *
## 연습 문제
**Q1**   
템플렛 특수화   
**Q2**   
얕은 복사로 배열로 선언할 경우 삭제가 안됨   
