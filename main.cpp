// Diego Enrique Jiménez Urgell A01652617
// 2/12/2020
#include <iostream>
#include <vector>
#include <tuple>
#include <map>

using namespace std;

#include "IP.h"
#include "DateTime.h"
#include "ResizingVectorGraph.h"
#include "LogRegister.h"

int main() {

    freopen("binnacleFinalProject.txt", "r", stdin);
    freopen("fans.txt", "w", stdout);

    cout << "Diego Enrique Jiménez Urgell A01652617" << endl;
    cout << "23/11/2020\n" << endl;

    LogRegister reg = LogRegister();
    string line = "";

    while (getline(cin, line)){
        reg.addConnection(line); // Adding all the entries to the LogRegister object
    }

    reg.printFans();

    freopen("attacks.txt", "w", stdout);
    cout << "Diego Enrique Jiménez Urgell A01652617" << endl;
    cout << "23/11/2020\n" << endl;

    reg.printAttacks();

    freopen("groupedAttacks.txt", "w", stdout);
    cout << "Diego Enrique Jiménez Urgell A01652617" << endl;
    cout << "23/11/2020\n" << endl;

    reg.printGroupedAttacks();

    return 0;
}
