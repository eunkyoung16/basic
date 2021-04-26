### 싱글톤 패턴 (Singleton Pattern)

* 특정 클래스의 __객체를 1개만 생성__되도록 제한하고 이를 공유하는 방법

#### 단점

* 싱글톤 인스턴스가 너무 많은 일을 하거나 많은 데이터를 공유시킬 경우 다른 클래스의 인스턴스들 간에 결합도가 높아져 "개방-폐쇄 원칙"을 위배함
* 따라서 수정이 어려워지고 테스트하기 어려워짐
* 멀티쓰레드 환경에서 동기화 처리를 안하면 인스턴스가 두개가 생성된다던지 하는 경우가 발생할 수 있음



---

### 1. 기본 싱글톤 (Basic Singleton)

```c++
class Singleton {
private:
    Singleton(){};
    Singleton(const Singleton& other);
 
    static Singleton instance;
    
public:
    static Singleton* GetInstance() {
        return instance;
    }
};
 
Singleton* Singleton::instance = nullptr;
 
//1< 사용법
Singleton::GetInstance();
```

* GetInstance : Singleton(자기자신)을 받는 함수를 하나 만들어서 미리 만들어 두었던 자기 자신 객체를 반환하는 클래스

<br>

#### 단점

1. static 클래스 멤버 변수는 프로그램 시작 시 main 함수 호출 이전에 초기화가 되면서 프로그램이 어떻게 되든 메모리를 잡게 되므로 비효율적인 작업이 됨
2. 정적 개체이기 때문에, 다른 전역 객체의 생성자에게 참조하고 싶은 경우 문제 발생. c++ 표준에서는 전역 객체들의 생성 순서가 정확히 정의되어 있지 않기 때문
   즉, 어떤 전역 객체의 생성자가 위 싱글톤을 참조하려고 할 때, 싱글톤 객체가 생성되기 이전에 발생할 수 있기 때문에 객체의 생성 시점을 변경해야함

<br>

### 2. 다이나믹 싱글톤 (Dynamic Singleton)

최초 GetInstance()를 호출하게 되는 시점에 생성됨. __게으른 초기화__(Lazy Initialization)라고도 함.

```c++
class DynamicSingleton {
private:
    DynamicSingleton() {};
    DynamicSingleton(const DynamicSingleton& other);
    ~DynamicSingleton() {};
 
    static DynamicSingleton* instance;
 
public:
    static DynamicSingleton* GetInstance() {
        if(instance == NULL) 
            instance = new DynamicSingleton(); // 메모리 할당 -> 도중에 메모리를 해제해야 한다면?
        return instance;
    }
};
DynamicSingleton* DynamicSingleton::instance = nullptr;
 
//!< 사용법
 DynamicSingleton::GetInstance()
```

그렇다면 __동적 메모리는 어떻게 해제시킬 것인가?__ : 도중에 메모리를 해지하여야 하는 경우

1) "atexit"로 Destory 함수를 콜백으로 만드는 방법  : "stdlib.h" 포함 시키기

```c++
class DynamicSingleton {
private:
    DynamicSingleton() {};
    DynamicSingleton(const DynamicSingleton& other);
    ~DynamicSingleton() {};
 
    static DynamicSingleton* instance;
 
    static void Destroy() {
        delete instance;
    }
 
public:
    static DynamicSingleton* GetInstance() {
        if (instance == NULL) {
            instance = new DynamicSingleton();
            atexit(Destroy);
        }
        return instance;
    }
};

DynamicSingleton* DynamicSingleton::instance = nullptr;
```

<br>

2. 전역 객체의 소멸자를 이용하여 메모리를 해제하는 방법 : __friend__를 이용하여 메모리를 해제시켜주는 방법 (잘 안 씀)

```c++
class DynamicSingleton {
private:
    DynamicSingleton() {};
    DynamicSingleton(const DynamicSingleton& other);
    ~DynamicSingleton() {};
 
    static DynamicSingleton* instance;
 
    friend DestroyerClass;
 
public:
    static DynamicSingleton* GetInstance() {
        if (instance == NULL) {
            instance = new DynamicSingleton();
        }
        return instance;
    }
};
DynamicSingleton* DynamicSingleton::instance = nullptr;
 
static class DestroyerClass{
public:
    ~DestroyerClass() {
        delete DynamicSingleton::GetInstance();
    }
};
```

그러나 두 방법 다 생산적이지 않음

<br>

### 3. 스테틱 지역 싱글톤 (Static local Singleton)

static 변수를 지역으로 사용하는 방법

```c++
//1. 첫번째 방식 - 포인터를 사용하지 않는 방식
class LocalStaticSingleton {
private:
    LocalStaticSingleton();
    LocalStaticSingleton(const LocalStaticSingleton& other);
    ~LocalStaticSingleton();
 
public:
    static LocalStaticSingleton& GetInstance() {
        static LocalStaticSingleton ins;
        return ins;
    }
};
 
//2. 두번째 방식 - 포인터를 사용하는 방식
class LocalStaticSingleton {
private:
    LocalStaticSingleton();
    LocalStaticSingleton(const LocalStaticSingleton& other);
    ~LocalStaticSingleton();
 
public:
    static LocalStaticSingleton* GetInstance() {
        static LocalStaticSingleton ins;  // 특징 1 _ 지역변수 생성
        return &ins;
    }
};
```

#### 특징

1. 함수를 처음 호출하는 시점에 초기화와 동시에 생성 진행 (객체를 한번도 사용하지 않을 경우 생성 x) => 함수 내에 static 으로 선언되어 있는 변수는 한 번 생성되어지면 그 다음부터는 static LocalStaticSingleton ins 구간을 지나도 ins 객체가 새로이 생성되지 않고 최초 생성되어진 ins 객체가 반환이 되어지게 됩니다
2. static 객체이기 때문에 프로그램 종료시까지 객체가 남아있음
3. 프로그램 종료시 자동으로 소멸자 호출
4. 소멸자에서 자원 해제를 하도록 하게되면 자원관리 신경 쓸 필요 없음

<br>

<br>



#### 출처

1. 프로그래밍 잡단공간 : [링크](https://vallista.tistory.com/entry/1-Singleton-Pattern-in-C)
2. 웬디의 기묘한 이야기 : [링크](https://wendys.tistory.com/12)
3. 장단점_기본기를 쌓는 정아마추어 코딩 블로그 : [링크](https://jeong-pro.tistory.com/86)