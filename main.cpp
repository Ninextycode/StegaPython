#include "TempBasedSteganography.h"
#include <iostream>


using namespace std;
using namespace stcr;


int main() {
    HighLevelStega hstega;
    hstega.encodeAndHideFileInJpgStructure("vodkaseledka14448", "secret", "container1.jpg");
}
