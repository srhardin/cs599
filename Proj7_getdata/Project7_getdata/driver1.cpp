#include <iostream>
#include <set>
#include "field.h"
#include "analyzer.h"

int main() try {
	MSfieldPart1 f2("ms2");
	//  012345678            012345678
	// +---------+          +---------+
	//0|121212121|         0|121212121|
	//1|         |         1|msmsmsmsm|
	// +---------+          +---------+
	std::cout << f2 << std::endl;

	//part 1
	Analyzer a2(f2);

	std::cout << ( ( a2.IsMine(0,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(2,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(4,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(6,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(8,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(1,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(3,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(5,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(7,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	//wrong
	std::cout << ( ( a2.IsSafe(0,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(2,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(4,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(6,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsSafe(8,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(1,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(3,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(5,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a2.IsMine(7,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";

	MSfieldPart1 f3("ms3");
	//            01234
	//+-----+    +-----+
	//|     |   0|smsms|
	//| 242 |   1|s242s|
	//|     |   2|smsms|
	//+-----+    +-----+
	std::cout << f3 << std::endl;

	//part 1
	Analyzer a3(f3);

	std::cout << ( ( a3.IsMine(1,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
    std::cout << ( ( a3.IsMine(3,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(1,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(3,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(0,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(0,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(0,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(2,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(2,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(4,0) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(4,1) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(4,2) ) ? "OK" : "Wrong" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(1,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(3,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(1,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsSafe(3,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(0,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(0,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(0,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(2,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(2,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(4,0) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(4,1) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";
	std::cout << ( ( a3.IsMine(4,2) ) ? "Wrong" : "OK" ) << std::endl;
	std::cout << "--------------------------------------\n";

	MSfieldPart1 f5("ms5");
    //   012345
	//  +------+
	// 0|m     |
	// 1|2 MM  |
	// 2|2M M  |
	// 3|m 4M  |
	// 4|223M  |
	// 5|1M2   |
	// 6|3 3   |
	// 7|mm MMM|
	// 8|mm4m4 |
	// 9|m4323 |
	//10|2m11mM|
	//  +------+
	std::cout << f5 << std::endl;

	//part 1
	Analyzer a5(f5);

    //cannot prove neither mine nor safe for 3,2
    std::cout << ( ( a5.IsMine(3,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << ( ( a5.IsSafe(3,2) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    //can prove mine for 3,3	
    std::cout << ( ( a5.IsMine(3,3) ) ? "OK" : "Wrong" ) << std::endl;  //wrong
    std::cout << ( ( a5.IsSafe(3,3) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
    //can prove mine for 3,4
    std::cout << ( ( a5.IsMine(3,4) ) ? "OK" : "Wrong" ) << std::endl;    //wrong
    std::cout << ( ( a5.IsSafe(3,4) ) ? "Wrong" : "OK" ) << std::endl;
    std::cout << "--------------------------------------\n";
} catch ( MSexception& e) {
	std::cerr << e.what() << std::endl;
}
