# Task management
## Task operation
### Creation
```c++
OStaskCreate(
    void(*task)(void* pd),      // task의 시작 번지
    void* pdata                 // task로 넘겨줄 전달인자
    OS_STK* ptos,               // task의 stack 사용 시작 번지
    INT8U prio                  // task의 우선 순위
);
```
<img src='imgs/2020-10-28-14-06-04.png' width=600/>

* `OStaskCreateExt()`
    - 전달인자 9개
    - 8번째 인자 | stack checking or stack clearing

### Deletion
<img src='imgs/2020-10-28-14-11-31.png' width=600>

### Suspension
<img src='imgs/2020-10-28-14-12-54.png' width=600>

### Resumption
<img src='imgs/2020-10-28-14-12-15.png' width=600>
