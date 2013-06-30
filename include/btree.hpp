#include <iostream>

const size_t BUCKET_SIZE = 5;
const size_t VALUEARRAY_SIZE = BUCKET_SIZE + 1;

class AlternateArray {
public:
	AlternateArray();
	AlternateArray(const AlternateArray& aa);
	~AlternateArray();
	void add(size_t key, void* val);
	void set(size_t idx, size_t key, void* val);
	void setFromIdx(size_t idx, void* val);
	void setFromKey(size_t key, void* val);
	void* getFromIdx(size_t idx);
	size_t getKeyFromIdx(size_t idx);
	//void set(size_t key, void* data);
	size_t getSize();
	size_t getBsize();
	size_t getVsize();
	void sort();
	int getIdx(size_t key);
	void move(size_t idx, size_t len);
private:
	size_t bsize;
	size_t vsize;
	void setKey(size_t idx, size_t key);
	void setValue(size_t idx, void* val);
	size_t bucketArray[BUCKET_SIZE];
	void*  valueArray[VALUEARRAY_SIZE];
};

