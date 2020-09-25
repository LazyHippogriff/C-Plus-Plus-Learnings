This program is my attempt at solving the below problem:

We have a function which is going to be called by multiple threads. The problem is to find a way to limit the number of times this function is executed per second given that we don't have a control over:

a) The number of threads calling this function.

b) The number of times per second this function will be called by any thread.

We need to somehow limit its rate of execution.

This program is my attempt at solving this kind of problem. I've tried to use the Token Bucket Algorithm for this. I read about the Token Bucket Algorithm from the below links:

* https://en.wikipedia.org/wiki/Token_bucket
* http://intronetworks.cs.luc.edu/current2/html/tokenbucket.html

The usage is simple. It consists of a header file "tokenBucket.hpp". You need to include this in your program.
Suppose you want to limit the execution rate of the function given below:

void printHelloWorld();

You need to do 2 things:

1. You need to create a tokenBucket object constructed with the required rate you want your function to be executed per second. 
e.g. tokenBucket aBucket(500);

2. When calling your function, then instead of:
printHelloWorld();

you have to do:

if(aBucket.getTokens(1) == 1) { 
    printHelloWorld();    
}

tokenBucket::getTokens() will return the number of tokens depending on the number that you requested(input argument) and the number of tokens available at that point of time in the bucket. In the above example, we requested 1 token and then checked if we got 1 token from the function before executing printHelloWorld().



