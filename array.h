#pragma once

// for copy memory
#include <algorithm>

// template class for dinamic array
template <typename T>
class Array
{
public:
  // create array with size "size", with type of value is T
  // only explicit ctor
  explicit Array(size_t size = 0, const T& value = T()) {
    this->m_data = new T[size];
    this->m_size = size;
    for(size_t i = 0; i < this->m_size; ++i){
      this->m_data[i] = value;
    }
  }
  // ctor with copy
  Array(Array const &other) {
    if (this != &other){
      this->m_data = new T[other.m_size];
      std::copy(other.m_data, other.m_data + other.m_size, this->m_data);
      this->m_size = other.m_size;
    }
  }
  // ctor with move
  Array(Array &&other){
    if (this != &other){
      // change pointer for other memory
      this->m_data = other.m_data;
      this->m_size = other.m_size;
      // clear internal fields of source class
      other.m_data = nullptr;
      other.m_size = 0;
    }
  }
  // dtor
  ~Array() {
    // free memory and clear size
    if (this->m_data != nullptr)
      delete [] this->m_data;
    this->m_data = nullptr;
    this->m_size = 0;
  }
  // asgmnt copy
  Array& operator=(Array const &other){
    if (this != &other){
      if (this->m_data != nullptr)
        delete [] this->m_data;
      this->m_data = new T[other.size()];
      this->m_size = other.m_size;
      std::copy(other.m_data, other.m_data + other.m_size, this->m_data);
    }
    return *this;
  }

  // asgmnt move
  Array& operator=(Array &&other){
    if (this != &other){
      if (this->m_data != nullptr)
        delete [] this->m_data;
      this->m_data = other.m_data;
      this->m_size = other.m_size;
      other.m_data = nullptr;
      other.m_size = 0;
    }
    return *this;
  }

  // index operator with const
  const T& operator[](size_t const &i) const{
    return (this->m_data[i]);
  }

  // index operator without const
  T& operator[](size_t const &i) {
    return (this->m_data[i]);
  }

  // size
  size_t size() const {
    return this->m_size;
  }

private:
  // pointer for data
  T *m_data;
  // size of array
  size_t m_size;
};

#ifdef GOOGLE_TEST_ON

#include <gtest/gtest.h>

// test fixture class (for google test only)
// is a special class for wrap source class
class ArrayTest: public ::testing::Test {
public:
  // size of memory for ctors
  size_t const size = 10;
  // value for elems for ctors
  int const value = -1;
  ArrayTest() {
    ;
  }
protected:
  // always perform this before each test
  void SetUp() override {
    // create objects for test
    defaultArray = new Array<int>();
    paramArray = new Array<int>(size, value);
  }

  // always perform this after each tests
  void TearDown() override {
    if (defaultArray != nullptr)
      delete defaultArray;
    if (paramArray != nullptr)
      delete paramArray;
  }

  // common variables for each tests
  // default array
  Array<int>* defaultArray;
  // array with params
  Array<int>* paramArray;
};

// test for default ctor
TEST_F(ArrayTest, DefaultConstructor) {
  // check size - zero size must be
  EXPECT_EQ(this->defaultArray->size(), 0);
  // check exist of object
  EXPECT_NE(&this->defaultArray, nullptr);
}

// ctor with params
TEST_F(ArrayTest, ParamsConstructor) {
  EXPECT_EQ(this->paramArray->size(), this->size);
  for(size_t i = 0; i < paramArray->size(); ++i){
    // IMPORTANT MOMENT: will do dereferencing a pointer before
    EXPECT_EQ((*paramArray)[i], this->value);
  }
}

// copy ctor
TEST_F(ArrayTest, CopyConstructor) {
  // new test object will be create from paramArray
  Array<int> test(*paramArray);
  EXPECT_EQ(test.size(), paramArray->size());
  for(size_t i = 0; i < test.size(); ++i){
    EXPECT_EQ(test[i], (*paramArray)[i]);
  }
  // check different address in memory
  EXPECT_NE(&test, paramArray);
  // change source
  for(size_t i = 0; i < paramArray->size(); ++i){
    (*paramArray)[i] += 1;
  }
  // check destination after change source
  for(size_t i = 0; i < paramArray->size(); ++i){
    EXPECT_NE(test[i], (*paramArray)[i]);
  }
}

// move ctor
TEST_F(ArrayTest, MoveConstructor) {
  // new test object will be create from paramArray
  Array<int> copySource(*paramArray);
  Array<int> destination(std::move(*paramArray));
  // check data
  for(size_t i = 0; i < destination.size(); ++i){
    EXPECT_EQ(destination[i], copySource[i]);
  }
  // check size with copy of origin
  EXPECT_EQ(destination.size(), copySource.size());
  // check zeros in original after moving
  EXPECT_EQ(paramArray->size(), 0);
  // pointer after delete is not check
  // not perform operator[] after free memory
}

// dtor
TEST_F(ArrayTest, Destructor) {
  // without test, because object is not exist
  delete this->paramArray;
  this->paramArray = nullptr;
  // if not drop, then OK result
  SUCCEED();
}

// asgmnt copy operator
TEST_F(ArrayTest, AsgmntCopy) {
  // new test object will be create from paramArray
  Array<int> test = *paramArray;
  EXPECT_EQ(test.size(), paramArray->size());
  for(size_t i = 0; i < test.size(); ++i){
    EXPECT_EQ(test[i], (*paramArray)[i]);
  }
  // check different address in memory
  EXPECT_NE(&test, paramArray);
  // change source
  for(size_t i = 0; i < paramArray->size(); ++i){
    (*paramArray)[i] += 1;
  }
  // check destination after change source
  for(size_t i = 0; i < paramArray->size(); ++i){
    EXPECT_NE(test[i], (*paramArray)[i]);
  }
}

TEST_F(ArrayTest, AsgmntMove){
  // new test object will be create from paramArray
  Array<int> copySource(*paramArray);
  Array<int> destination = std::move(*paramArray);
  // check data
  for(size_t i = 0; i < destination.size(); ++i){
    EXPECT_EQ(destination[i], copySource[i]);
  }
  // check size with copy of origin
  EXPECT_EQ(destination.size(), copySource.size());
  // check zeros in original after moving
  EXPECT_EQ(paramArray->size(), 0);
}

#endif
