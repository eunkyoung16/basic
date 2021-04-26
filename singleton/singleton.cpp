/***
 * 작성일자 : 2021-03-24
 * 내용 : 싱글톤 구현
***/

#include<iostream>
#define endl "\n"
using namespace std;

class Singleton{
// 외부에서 생성자를 호출할 수 없도록 private으로 선언
private:
    Singleton() { } 
    Singleton(const Singleton& other);
    ~Singleton() { } 

    // 자전거 몇 명 빌렸는지 보여주는 변수
    int total=0;
    
public:
    // 싱글톤 인스턴스를 반환할 멤버함수_자기자신에 대한 인스턴스를 하나 만들어 외부에 제공해줄 메소드임
    static Singleton* GetInstance() {
        static Singleton ins;
        return &ins;
     }
     
    int showTotal(){
        return total;
    }

    void addTotal(){
        total += 1;
    }
};

int main(void) {
    // singleton 인스턴스 변수 포인터 변수 형태로 선언
    Singleton *ins1 = Singleton::GetInstance();
    Singleton *ins2 = Singleton::GetInstance();
    Singleton *ins3 = Singleton::GetInstance();
    
    // 인스턴스를 통해 total의 값을 1씩 증가시킴
    cout << "지금까지 자전거를 빌린 사람 : " << ins1->showTotal() << endl;
    
    ins1 -> addTotal();
    ins2 -> addTotal();
    cout << "지금까지 자전거를 빌린 사람 : " << ins2->showTotal() << endl;

    ins3 -> addTotal();
    cout << "지금까지 자전거를 빌린 사람 : " << ins3->showTotal() << endl;

    // 결과값을 보면 서로 다른 인스턴스를 통해 멤버변수 total 값을 증가시켰으나 그 값이 누적됨을 확인할 수 있음
    
    return 0;
} 