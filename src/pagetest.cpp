#include <iostream>
#include <cstdlib>
#include "recordmodel/DBSinglePageBase.h"
#include "recordmodel/DBInfoPage.h"
using namespace std;

int main(){
	DBInfoPage* page = new DBInfoPage();
	cout << page -> getbit(13204) << endl;
	page -> setbit((unsigned)13204, 1);
	cout << page -> getbit(13204) << endl;
	page -> debug();
	return 0;
}