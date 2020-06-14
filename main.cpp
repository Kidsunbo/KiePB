#include <iostream>
#include <vector>
#include <thread>
#include "KiePB/ProgressBar.h"

using namespace std::chrono_literals;



int main() {
    std::vector<int> v{1,2,3,4,5,6,7,8,9};
    Kie::ProgressBar pb(v);
//    Kie::ProgressBar pb(Kie::StyleType::Style4);
    pb.setTotal(100);
    pb.setStep(1);

    while(pb.display()) {
        std::this_thread::sleep_for(0.2s);
    }
    return 0;
}
