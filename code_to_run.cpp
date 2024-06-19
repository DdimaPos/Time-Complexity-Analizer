#include<iostream>
int func2(){
    for(int i=0; i<N; i++){
        for(int j=0; i<N; j++){
        
        }
    }
}
int func1(){
    for(int j=1; j < N; j++){
        std::cout<<"LOOP IN FUNCTION 1";
        func2();
    }
    return 1;
}
int main(){
    int N;
    for (int i = 0; i < N; i++){
        std::cout<<"LOOP 1";
    }
    for (int i = 0; i < N; i+=2){
        std::cout<<"LOOP 2";
        for (int i = 1; i < N; i*=2){
            std::cout<<"LOOP 2.1";
        }
    }
    func1();
    return 0;
}