#ifndef __TOKEN_BUCKET_HPP__
#define __TOKEN_BUCKET_HPP__

#include <chrono>
#include <mutex>
#include <iostream>

class tokenBucket{

  public:

    /* Constructors */

    tokenBucket():m_tokenFillRatePerSecond(0),m_bucketSize(0),m_availableTokens(0),m_lastRefillTime(0),m_timeIntervalPerRequestInMicroSeconds(0){}

    tokenBucket(const tokenBucket& src):m_tokenFillRatePerSecond(src.m_tokenFillRatePerSecond),m_bucketSize(src.m_bucketSize),m_availableTokens(src.m_availableTokens),m_lastRefillTime(src.m_lastRefillTime),m_timeIntervalPerRequestInMicroSeconds(src.m_timeIntervalPerRequestInMicroSeconds)  {}

    tokenBucket& operator=(const tokenBucket& src) {
      m_tokenFillRatePerSecond = src.m_tokenFillRatePerSecond;
      m_bucketSize = src.m_bucketSize;
      m_availableTokens = src.m_availableTokens;
      m_lastRefillTime = src.m_lastRefillTime;
      m_timeIntervalPerRequestInMicroSeconds = src.m_timeIntervalPerRequestInMicroSeconds;
      return *this;
    }

    tokenBucket(uint64_t fa_tokenFillRatePerSec): m_tokenFillRatePerSecond(fa_tokenFillRatePerSec),m_bucketSize(fa_tokenFillRatePerSec),m_availableTokens(fa_tokenFillRatePerSec),m_lastRefillTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()),m_timeIntervalPerRequestInMicroSeconds(double(NUMBER_OF_MICROSECONDS_IN_A_SECOND)/fa_tokenFillRatePerSec){
      fprintf(stdout,"\nToken Bucket initialised with the following values\nm_bucketSize(%lu)\nm_tokenFillRatePerSecond(%lu)\nm_availableTokens(%lu)\nm_lastRefillTime(%lu)\nm_timeIntervalPerRequestInMicroSeconds(%f)\n",m_bucketSize,m_tokenFillRatePerSecond,m_availableTokens,m_lastRefillTime,m_timeIntervalPerRequestInMicroSeconds);
    }


    uint64_t getTokens(uint64_t fa_numberOfRequestedTokens = 1){
      std::lock_guard<std::mutex> lg(m_mu);
      uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
      refill_tokens(now);

      uint64_t tokensToBeReturned = 0;
      if(m_availableTokens < fa_numberOfRequestedTokens){
        tokensToBeReturned = m_availableTokens;
        m_availableTokens = 0;
      }
      else{
        m_availableTokens -= fa_numberOfRequestedTokens;
        tokensToBeReturned = fa_numberOfRequestedTokens;
      }
      return tokensToBeReturned;
    }

  private:
    static constexpr int NUMBER_OF_MICROSECONDS_IN_A_SECOND = 1000000;
    std::mutex m_mu;
    uint64_t m_bucketSize; //Currently I'm taking Bucket Size = token fill rate per second.
    uint64_t m_tokenFillRatePerSecond;
    uint64_t m_availableTokens; //current number of tokens available in the bucket
    uint64_t m_lastRefillTime;
    double m_timeIntervalPerRequestInMicroSeconds; // = (pow(10,6)/m_tokenFillRatePerSecond)

    /* to refill the tokens as per time diff */
    void refill_tokens(const uint64_t& fa_currentRequestTime) {
      auto timeElapsed = fa_currentRequestTime - m_lastRefillTime;

      if (timeElapsed >= m_timeIntervalPerRequestInMicroSeconds) {

        uint64_t l_newTokens = timeElapsed/m_timeIntervalPerRequestInMicroSeconds;

        if(m_availableTokens + l_newTokens < m_bucketSize) {
          m_availableTokens += l_newTokens;
        }
        else {
          m_availableTokens = m_bucketSize;
        }
        m_lastRefillTime = fa_currentRequestTime;
      }
    }
};

#endif
