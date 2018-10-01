#include "unity_src/unity.h"

void test_ThereIsNoDifferenceBetweenTwoIdenticalLines();
void test_AddedSingleCharIsEmphazedWithBrackets();

int main() {
	UnityBegin("cdiff tests");
	RUN_TEST(test_ThereIsNoDifferenceBetweenTwoIdenticalLines);
	RUN_TEST(test_AddedSingleCharIsEmphazedWithBrackets);
	return UnityEnd();
}
