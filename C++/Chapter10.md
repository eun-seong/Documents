# [Chapter 10] 예외 처리
## 용어
`try` : 예외가 발생할 수 있는 곳    
`throw` : 예외 발생   
`catch` : 예외 처리    
스택 풀기 : 스택 메모리에 불필요한 데이터가 쌓이지 않게 예외 발생하기 전으로 돌아감   
* * *
## `try`, `catch`, `throw`
예외를 `if-else`문으로 처리하는 것보다 **구조적으로** 간결
```c++
try{
    // 에러가 발생할 수 있는 코드
    // 흐름이 한 눈에 보임
    if(error 발생)
        throw errorcode;
    ...
} catch(int e){
    예외 처리
}
```
### `catch` 다중화
예외 상황을 상황에 따라 구별하여 사용할수 있음
```c++
try{
    ...
} catch(int eNum){

} catch(char eCh){
    ...
    // 이중 가능
    try {
        ...
    } catch(...) {
        ...
    }
}
```
### 예외 클래스
사용자화 예외 클래스 만들 수 있음   
```c++
class CMyException {
public:
    CMyException(int errorCode, const char* pszMsg){
        m_nErrorCode = errorCode;
        strcpy_s(m_szMsg, sizeof(pszMsg), pszMsg);
    }
    ...
};

int main(){
    ...
    try{
        if(input < 0) {
            CMyException exp(10, "에러 발생 메세지");
            throw exp;
        }
    } catch(CMyException& e) {
        // 예외 처리
        ...
    }
}
```
## Stack unwinding
함수 호출로 인해 쌓아 올려진 스택에서 예외가 발생하면 `catch`를 만날 때까지 함수와 그 함수의 지역변수를 정리하며 스택에서 `pop`한다.   
이렇게 되면 스택이 자동으로 풀리며 `catch`를 만나기 전 까지 스택에 있던 함수들이 호출되기 전 상태로 **정상적으로** 되돌아온다. 
## 메모리 예외 처리
`bad_alloc` 를 사용하여 메모리 예외 처리가 가능
```c++
try{
    ...
} catch(bad_alloc& exp) {
    cout << exp.what() << '\n';
}
```
* * *
## 연습 문제
**Q1**   
호출 스택에서 예외가 발생할 경우 `catch`를 만날 때까지 거슬로 올라가 함수가 호출되기 전 상태로 되돌아간다.         
**Q2**   
메모리 예외 처리를 할 수 있다. `what()` 함수를 사용하면 원인을 알 수 있다.   