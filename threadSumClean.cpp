// should be compiled with -lpthread

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

mutex mtx;


// sum of the vector values without using threads
int normalSum(vector<int> nums){
  int ans = 0;

  for(int i = 0 ; i < nums.size(); i++){
    ans += nums[i];
  }
  
  return ans;
}


// sum of the vector values using threads
void thSum(int start, int end, vector<int> nums, int& ans){

  for( int i = start; i < end; i++){
    lock_guard<mutex> lock(mtx);
    ans += nums[i];
  }
}


// splitting the vector into equal parts, instantiating new threads,
// and joining them before function ends
void thVecSplit(vector<int> nums, unsigned int thNum){

// the base range on which
// one thread operates
int chunk = 0;

// the start point and endpoint
// of the chunk
int currentStart = 0;
int previousStart = 0;
int currentEnd = 0;
int previousEnd = 0;

int remainder = 0;

  // calculate chunks on which threads can perform computation

  // when vector is divisible by the number of threads
  if(nums.size() % thNum == 0){
    chunk = (nums.size() / thNum);
    cout << chunk << " <- chunk size" << endl;

  // when vector is NOT divisible by the number of threads
  }else {
    remainder = (nums.size() % thNum);
    chunk = (nums.size() / thNum);
    cout << chunk << " <- prime chunk size" << endl;

  } // end of chunk calculation 

currentStart = 0;
previousStart = 0;
currentEnd = chunk;
previousEnd = chunk;

// vector of threads
// that will be instantiated
vector<thread> threadVec;

// sum of values in the vector
// worked on by threads
int sum = 0;

int increase = 0;

for(int i = thNum; i != 0; i--){

  // stack allocated, no need for deallocation
  threadVec.push_back(thread(thSum, currentStart, currentEnd, ref(nums), ref(sum))); 
  
  if(remainder > 0){
    increase = 1; 
  }else{
    increase = 0;
  }

  previousStart = currentStart;
  previousEnd = currentEnd; 

  currentStart = previousEnd;
  currentEnd = currentStart + chunk + increase;
  remainder--;
}

// joining the threads
for(thread& th : threadVec){
  th.join();
}

// outputting the calculated sum
cout << sum << " <- thSum output" << endl;

}


int main(int argc, char** argv){

  unsigned int threadNumMax = thread::hardware_concurrency();

  // output the number of threads available on this machine 
  cout << "The max number of threads available: " << threadNumMax << endl;

  // set base threadToUse to 1
  unsigned int threadsToUse = 1;

  cout << "Please enter the number of threads to use: " << endl;
  
  //basic input check
  while(true){
   
    cin >> threadsToUse;
    
    if(threadsToUse < 1){
      cout << "Please specify a greater than 0 number of threads:" << endl;
      continue;
    }

    if(threadsToUse > threadNumMax){
      cout << "Please specify a number of threads apropriate for your hardware" << endl;
      continue;
    }

    break;
  }

  cout << "Threads to use: " << threadsToUse << endl;
  

  // vector with values to be added together
  vector<int> vec;

  // pushing values onto the vector 
  for(int i = 0; i < 10000;i++){
    vec.push_back(i);
  }

  cout << "Size of vector: " << vec.size() << endl;


  int vecSum = normalSum(vec);
  
  cout << vecSum << " <- sum of vector\n";


  cout << "thVecSplit with " << threadsToUse << endl;

  // vector sum performed with multiple threads
  thVecSplit(vec,threadsToUse);


  return 0;
}
