#include <btree.hpp>
#include <stdio.h>

AlternateArray::AlternateArray() {
	bsize = 0;
	vsize = 0;
}

AlternateArray::AlternateArray(const AlternateArray& array) {
}

AlternateArray::~AlternateArray() {
}

void AlternateArray::add(size_t key, void* value) {
}

void AlternateArray::setKey(size_t idx, size_t key) {
	if (this->bsize == BUCKET_SIZE-1) {
		throw "Bucket OverFlow!!";
	}
	bucketArray[idx] = key;
	bsize++;
}

void AlternateArray::setValue(size_t idx, void* value) {
	if (this->vsize == VALUEARRAY_SIZE-1) {
		throw "Bucket OverFlow!!";
	}
	valueArray[idx] = value;
	vsize++;
}

void AlternateArray::set(size_t idx, size_t key, void* value) {
	try {
		this->setKey(idx, key);
		this->setValue(idx+1, value);
	}
	catch (...) {
		throw "Bucket OverFlow!!";
	}
}

void AlternateArray::setFromIdx(size_t idx, void* val) {
	try {
		this->setValue(idx+1, val);
	}
	catch (...) {
	}
}

void AlternateArray::setFromKey(size_t key, void* val) {
	try {
		int idx = this->getIdx(key);
		this->setValue(idx+1, val);
	}
	catch (...) {
	}
}

void* AlternateArray::getFromIdx(size_t idx) {
	return valueArray[idx+1];
}

size_t AlternateArray::getKeyFromIdx(size_t idx) {
	return bucketArray[idx];
}

void AlternateArray::move(size_t idx, size_t len) {
	
}

size_t AlternateArray::getBsize() {
	return this->bsize;
}

size_t AlternateArray::getVsize() {
	return this->vsize;
}

size_t AlternateArray::getSize() {
	return this->getBsize() + this->getVsize();
}

void AlternateArray::sort() {
	size_t i, j;
	for (i = 0; i < this->getSize(); i++) {
		for (j = i+1; j < this->getSize() - i; j++) {
			if (this->bucketArray[i] > this->bucketArray[j]) {
				size_t tmp = this->bucketArray[i];
				this->bucketArray[i] = this->bucketArray[j];
				this->bucketArray[j] = tmp;
			}
		}
	}
}

int AlternateArray::getIdx(size_t key) {
	size_t i;
	for (i = 0; i < this->getBsize(); i++) {
		if (key < bucketArray[i]) {
			return i-1;
		}
		else if (key == bucketArray[i]) {
			return i;
		}
	}
	return this->getBsize();
}
