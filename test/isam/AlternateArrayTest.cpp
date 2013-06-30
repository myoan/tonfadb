/*
 * AlternateArray create():
 *  - データ領域は存在するが、サイズは0 である
 *  - データは0 クリアされている
 * void destruct()
 *  - 0 クリアしてfree する
 * void add(key, val): -> push or insert;
 *  - sizeが許容範囲内であればpush, そうでなければinsertする
 * void getIdx(key):
 *  - 該当するidxを返す
 * void insert(key, val):
 *  - getIdxをして、move して setする
 * void deleteFromKey(key):
 *  - keyより小さい最大のbucketとその隣のdataを0 クリアして、move する
 * void deleteFromIdx(idx):
 *  - idxにあるbucketとその隣のdataを0 クリアして、move する
 * void move(idx, len):
 *  - idx以降のAlternatearrayをlen 分ずらす( 負も可)
 * void set(idx, key, val):
 *  - idx番目のbucketにkeyを入れて、その隣のdataにvalをいれる
 * void setFromKey(key, val):
 *  - keyより小さい最大のbucketの隣のdataにvalをいれる
 * void setFromIdx(idx, val):
 *  - idxにあるbucketの隣のdataにvalをいれる
 * val getFromKey(key):
 *  - keyより小さい最大のbucketの隣のvalを返す
 * val getFromIdx(val):
 *  - idxにあるbucketの隣のvalを返す
 * bucket split(Alternaterray* before, AlternateArray* after):
 *  - 中央のbucket の値を返す
 *  - before, afterは空のpointer
 *  - bucketより前の配列をbeforeにいれる
 *  - bucketより後ろの配列をafterにいれる
 * AlternateArray before(bucket):
 *  - bucketより前の配列をnew して左詰めで返す
 * AlternateArray after(bucket):
 *  - bucketより後ろの配列をnew して左詰めで返す
  */

#include <CppUTest/CommandLineTestRunner.h>
#include <stdio.h>
#include <string.h>
#include <btree.hpp>

TEST_GROUP(AlternateArrayTestGroup) {
	AlternateArray* array;
	void setup() {
		array = new AlternateArray();
	}
	void teardown() {
		delete(array);
	}
};

/*
 * AlternateArray create():
 *  - データ領域は存在するが、サイズは0 である
 *  - データは0 クリアされている
*/

TEST(AlternateArrayTestGroup, Create) {
	LONGS_EQUAL(0, array->getSize());
}

/*
 * void destruct()
 *  - 0 クリアしてfree する
 */

/*
 * void add(key, val): -> push or insert;
 *  - sizeが許容範囲内であればpush, そうでなければinsertする
 */

void* createDummy(int i) {
	char* ret = (char*)malloc(16);
	memset(ret, '\0', 16);
	sprintf(ret, "hello%d", i);
	return (void*)ret;
}

//TEST(AlternateArrayTestGroup, Add) {
//	void* dummy = createDummy(1);
//	array->add(1, dummy);
//}

/*
 * void set(idx, key, val):
 *  - idx番目のbucketにkeyを入れて、その隣のdataにvalをいれる
 */

TEST(AlternateArrayTestGroup, Set) {
	void* dummy = createDummy(1);
	try {
		array->set(0, 1, dummy);
		LONGS_EQUAL(1, array->getBsize());
	}
	catch (...){
		FAIL("BucketOverFlow Exception is throwed.");
	}
}

TEST(AlternateArrayTestGroup, SetOverflowed) {
	try {
		void* dummy = createDummy(1);
		array->set(0, 1, dummy);
		dummy = createDummy(2);
		array->set(1, 2, dummy);
		dummy = createDummy(3);
		array->set(2, 3, dummy);
		dummy = createDummy(4);
		array->set(3, 4, dummy);
		dummy = createDummy(5);
		array->set(4, 5, dummy);
		FAIL("BucketOverFlow Exception is not throwed.");
	}
	catch (...) {
		//std::cout << "catched" << std::endl;
	}
}

/*
 * void setFromKey(key, val):
 *  - keyより小さい最大のbucketの隣のdataにvalをいれる
 */

TEST(AlternateArrayTestGroup, SetFromKey) {
	void* dummy = createDummy(1);
	array->set(0, 10, dummy);
	dummy = createDummy(2);
	array->setFromKey(30, dummy);
	STRCMP_EQUAL("hello2", (char*)array->getFromIdx(1));
	dummy = createDummy(3);
	array->setFromKey(10, dummy);
	STRCMP_EQUAL("hello3", (char*)array->getFromIdx(0));
}

/*
 * void setFromIdx(idx, val):
 *  - idxにあるbucketの隣のdataにvalをいれる
 */

TEST(AlternateArrayTestGroup, SetFromIdx) {
	void* dummy = createDummy(1);
	array->setFromIdx(0, dummy);
	char* ret = (char*)array->getFromIdx(0);
	STRCMP_EQUAL("hello1", ret);
}

/*
 * void getIdx(key):
 *  - 該当するidxを返す
 */

TEST(AlternateArrayTestGroup, getIdx) {
	try {
		void* dummy = createDummy(1);
		array->set(0, 10, dummy);
		dummy = createDummy(2);
		array->set(1, 12, dummy);
		LONGS_EQUAL(0, array->getIdx(10));
		LONGS_EQUAL(1, array->getIdx(12));
	}
	catch (...) {
		FAIL("BucketOverFlow");
	}
}

TEST(AlternateArrayTestGroup, getIdxFromUnsetKey) {
	try {
		void* dummy = createDummy(1);
		array->set(0, 10, dummy);
		dummy = createDummy(2);
		array->set(1, 12, dummy);
		LONGS_EQUAL(2, array->getIdx(15));
		LONGS_EQUAL(-1, array->getIdx(3));
	}
	catch (...) {
		FAIL("BucketOverFlow");
	}
}

/*
 * void insert(key, val):
 *  - getIdxをして、move して setする
 */

/*
 * void deleteFromKey(key):
 *  - keyより小さい最大のbucketとその隣のdataを0 クリアして、move する
 */

/*
 * void deleteFromIdx(idx):
 *  - idxにあるbucketとその隣のdataを0 クリアして、move する
 */

/*
 * void move(idx, len):
 *  - idx以降のAlternatearrayをlen 分ずらす( 負も可)
 */

TEST(AlternateArrayTestGroup, PositiveMove1) {
	try {
		void* dummy = createDummy(1);
		array->set(0, 10, dummy);
		array->move(0, 1);
		STRCMP_EQUAL("", (char*)array->getFromIdx(0));
		LONGS_EQUAL(10, array->getFromIdx(1));
		STRCMP_EQUAL("hello1", (char*)array->getFromIdx(1));
	}
	catch (...) {
		FAIL("BucketOverFlow");
	}
}

/*
 * val getFromKey(key):
 *  - keyより小さい最大のbucketの隣のvalを返す
 */

//TEST(AlternateArrayTestGroup, GetFromKey) {
//	void* dummy = createDummy(1);
//	array->add(1, dummy);
//}

/*
 * val getFromIdx(idx):
 *  - idxにあるbucketの隣のvalを返す
 */

TEST(AlternateArrayTestGroup, GetFromIndex) {
	void* dummy = createDummy(1);
	try {
		array->set(0, 1, dummy);
	}
	catch (...) {
		FAIL("BucketOverFlow");
	}
	char* ret = (char*)array->getFromIdx(0);
	STRCMP_EQUAL("hello1", ret);
}

/*
 * bucket split(Alternaterray* before, AlternateArray* after):
 *  - 中央のbucket の値を返す
 *  - before, afterは空のpointer
 *  - bucketより前の配列をbeforeにいれる
 *  - bucketより後ろの配列をafterにいれる
 */

/*
 * AlternateArray before(bucket):
 *  - bucketより前の配列をnew して左詰めで返す
 */

/*
 * AlternateArray after(bucket):
 *  - bucketより後ろの配列をnew して左詰めで返す
 */


/************************************************************/
/* [private method] */

TEST(AlternateArrayTestGroup, Sort) {
}

/************************************************************/

int main(int argc, char const* argv[])
{
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
