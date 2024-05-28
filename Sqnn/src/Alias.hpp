//
// Created by cj on 28-5-24.
//

#ifndef KDE_SUB_QUADRATIC_ALIAS_HPP
#define KDE_SUB_QUADRATIC_ALIAS_HPP

#include <vector>
#include <random>
#include <chrono>


/**
 *
 * @tparam T should support decimal number
 * @tparam T2 Natural number that is large enough to support the size of the dataset
 */
template<typename T, typename T2>
class Alias {
  using set = std::pair<T2, T>;  //contains its index and weighting (in that order)
public:
  Alias(T *t, T2 t2) {
    //compute the mean
    mean_ = 0;
    for (T2 i = 0; i < t2; ++i) mean_ += *(t + i);
    mean_ /= t2;

    //resize the bucket list to the correct size
    buckets_.resize(t2);
    this->SetupBucket(t, t2);

    //setup distributions for both bucket selection and internal bucket selection
    bucket_distribution = std::uniform_int_distribution(static_cast<T2>(0), t2 - 1);
    internal_bucket_distribution = std::uniform_real_distribution(0.f, mean_);
  }

  T2 Sample(long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count()) {
    //setup RNG
    std::mt19937_64 random(seed);

    //select a bucket
    T2 bucket_index = bucket_distribution(random);
    Bucket *bucket = &buckets_[bucket_index];

    //select a member of the bucket
    T internal_sampling = internal_bucket_distribution(random);
    bool member = bucket->t - internal_sampling < 0;
    return bucket->t2[member];
  }

private:
  struct Bucket {
    T t;        //How much of the bucket the first element filled
    T2 t2[2];   //original index of the 2 elements used to fill the bucket. second value will be -1 if unused (aka. t=mean_
  };

  std::vector<Bucket> buckets_;
  T mean_;      //also the size of each bucket

  std::uniform_int_distribution<T2> bucket_distribution;          //for selecting bucket
  std::uniform_real_distribution<T> internal_bucket_distribution; //for selecting index inside a bucket

  void SetupBucket(T *t, T2 t2) {
    assert(t != NULL);  //handle error throw in case it is needed

    std::vector<set> stack(t2);  //double stack
    T2 p1 = 0, p2 = t2 - 1;                   //index to the first free point from both side

    //filling both stack,
    //one is filled with values equal to or less than the mean.
    //second is filled with values greater than the mean.
    for (T2 i = 0; i < t2; ++i) {
      T2 index = t[i] > mean_ ? p2-- : p1++;
      stack[index] = std::pair(i, t[i]);
    }

    //index of next empty bucket in buckets
    T2 bucket_index = 0;

    //Build buckets by selecting one element from the stack smaller than mean and filling with the
    //rest using an element from the larger than mean stack
    for (; p2 != t2 - 1;) {
      set *s1 = &stack[--p1], *s2 = &stack[++p2];   //pops the first element in each stack
      Bucket *bucket = &buckets_[bucket_index++];   //Next unused bucket
      this->BuildBucket(bucket, s1, s2);            //Build bucket
      s2->second -= mean_ - s1->second;             //reducing the remaining size of the large element
      T2 index = s2->second > mean_ ? p2-- : p1++;  //checks what stack the remaining is going back to
      stack[index] = *s2;
    }

    //remaining elements are equal size of the stack, so they are filled into their own containers
    for (; p1 != 0;) {
      set *s = &stack[--p1];           //next element in stack
      Bucket *bucket = &buckets_[bucket_index++];   //next empty bucket
      this->BuildBucket(bucket, s);
    }
  }

  void BuildBucket(Bucket *bucket, set *s1, set *s2 = nullptr) {
    assert(bucket != nullptr && s1 != nullptr); //error throw
    bucket->t2[0] = s1->first;                  //assigning first index
    bucket->t = s1->second;                     //filling the bucket
    bucket->t2[1] = s2 ? s2->first : -1;        //assigning second index, -1 if no index was passed.
  }
};

#endif //KDE_SUB_QUADRATIC_ALIAS_HPP
