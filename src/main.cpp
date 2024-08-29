#include <iostream>
#include <chrono>
#include <thread>
#include "fsrInterface.h"

// for logging to a file
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std::chrono;
using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int main() {
    cout << "Beginning to use FSRs" << endl;

    fsrInterface* m_fsr = new fsrInterface();

    m_fsr->initialize();

    cout <<"initialzed" << endl;
    int count = 0;
    int totalCount = 500;

    std::cout << "Beginning Readings" << std::endl;
    while(count <= totalCount){
        m_fsr->updateSensorState();
        //cout << "Ch1: " << m_fsr->getChannelReading(0) << endl;
        count++;
    }

    std::cout << "Bye Anakin!" << std::endl;


    return 0;
}
