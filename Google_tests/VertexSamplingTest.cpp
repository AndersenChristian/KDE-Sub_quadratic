//
// Created by cj on 30-4-24.
//

#include <gtest/gtest.h>
#include "KDE.hpp"
#include "sample.hpp"

class testKDE : public KDE {
  float query(const Eigen::VectorXf &q) override {
    float sum = 0;
    for (float f: q) {
      sum += f;
    }
    return sum;
  }

  const Eigen::MatrixXf &getData() override {
    return data;
  }
private:
  Eigen::MatrixXf data;
};

TEST(SampleTest, DegreeWeight) {
  const double sigma = 3.3;
  Eigen::MatrixXf data(10, 10);
  data << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
      11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
      31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
      41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
      61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
      71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
      81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
      91, 92, 93, 94, 95, 96, 97, 98, 99, 100;
  kernel::kernelLambda<float> kernel = kernel::kernel_function<float>(kernel::type::Gaussian, sigma);
  testKDE kde;

  auto out = Sample::DegreeWeight(data, kde, 0);
  std::vector<float> expected_out{460.000000, 470.000000, 480.000000, 490.000000, 500.000000, 510.000000, 520.000000,
                                  530.000000, 540.000000, 550.000000};
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(out[i], expected_out[i]);
  }
}

TEST(SampleTest, Bucket) {
  Sample::pair p1(5, 1);
  Sample::pair p2(10, 2);
  float bucket_size = 7.5;
  auto bucket = Sample::SetupBucket(p1, p2, bucket_size);

  //check if bucket is set up correct
  EXPECT_EQ(bucket.index[0], 1);
  EXPECT_EQ(bucket.value[0], 5);
  EXPECT_EQ(bucket.index[1], 2);
  EXPECT_EQ(bucket.value[1], 2.5);

  //check if p2 has been reduced correctly
  EXPECT_EQ(p2.first, 7.5);
}

TEST(SampleTest, CreateSampleBuckets) {
  std::vector<float> data{0.2, 0.3, 0.4, 1, 1.1, 1.2};
  float bucket_size = 0;

  for (auto val: data) {
    bucket_size += val;
  }
  //bucket_size /= (float) data.size();

  auto buckets = Sample::CreateSampleBuckets(data, bucket_size);

  EXPECT_EQ(buckets.size(), 6);

  //checks if the 3 first are made exactly as expected
  EXPECT_EQ(buckets[0].index[0], 0);
  EXPECT_EQ(buckets[0].index[1], 5);
  EXPECT_FLOAT_EQ(buckets[0].value[0], 0.2);
  EXPECT_FLOAT_EQ(buckets[0].value[1], 0.5);
  EXPECT_EQ(buckets[1].index[0], 1);
  EXPECT_EQ(buckets[1].index[1], 4);
  EXPECT_FLOAT_EQ(buckets[1].value[0], 0.3);
  EXPECT_FLOAT_EQ(buckets[1].value[1], 0.4);
  EXPECT_EQ(buckets[2].index[0], 2);
  EXPECT_EQ(buckets[2].index[1], 3);
  EXPECT_FLOAT_EQ(buckets[2].value[0], 0.4);
  EXPECT_FLOAT_EQ(buckets[2].value[1], 0.3);

  //4-6 can be in different order based on how heap is implemented.
  //checks that only a single value is placed in each bucket and filled the entire bucket
  EXPECT_FLOAT_EQ(buckets[3].value[0], 0.7);
  EXPECT_FLOAT_EQ(buckets[4].value[0], 0.7);
  EXPECT_FLOAT_EQ(buckets[5].value[0], 0.7);
  EXPECT_FLOAT_EQ(buckets[3].value[1], 0);
  EXPECT_FLOAT_EQ(buckets[4].value[1], 0);
  EXPECT_FLOAT_EQ(buckets[5].value[1], 0);

  //ensures that all exist in there only once
  int count[6]{0};
  for (int i = 3; i < 6; ++i) {
    count[buckets[i].index[0]]++;
    count[buckets[i].index[1]]++;
  }
  EXPECT_EQ(count[0], 3);
  EXPECT_EQ(count[1], 0);
  EXPECT_EQ(count[2], 0);
  EXPECT_EQ(count[3], 1);
  EXPECT_EQ(count[4], 1);
  EXPECT_EQ(count[5], 1);

}

TEST(SampleTest, vertexSampling) {
  std::vector<float> data{0.2, 0.3, 0.4, 1, 1.1, 1.2};
  float bucket_sum = 4.2;
  float bucket_size = bucket_sum / (float) data.size();
  auto buckets = Sample::CreateSampleBuckets(data, bucket_sum);
  const unsigned long nr_samples = 100000;
  auto samples = Sample::VertexSampling(buckets, nr_samples, bucket_size);

  unsigned long count[6]{0};
  for (auto val: samples)
    count[val]++;

  //testing against being inside the 99 percentile
  EXPECT_NEAR(count[0], 4762, 167);
  EXPECT_NEAR(count[1], 7143, 199);
  EXPECT_NEAR(count[2], 9524, 224);
  EXPECT_NEAR(count[3], 23809, 291);
  EXPECT_NEAR(count[4], 26190, 304);
  EXPECT_NEAR(count[5], 28571, 316);
}

TEST(MatrixShuffle, Small){
  Eigen::MatrixXf data(3,3);
  data << 1, 2, 3, 4, 5, 6, 7, 8, 9;
  Sample::ShuffleMatrixRows(data);
  std::cout << data;
}