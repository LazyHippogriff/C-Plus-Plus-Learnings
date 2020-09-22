#ifndef __TOKEN_BUCKET_HPP__
#define __TOKEN_BUCKET_HPP__

#include <chrono>
#include <mutex>
#include <iostream>

class tokenBucket{
  private:
    static constexpr int NUMBER_OF_MICROSECONDS_IN_A_SECOND = 1000000;
    std::mutex m_mu;
    uint64_t m_bucketSize; //Currently I'm taking Bucket Size = token fill rate per second.
    uint64_t m_tokenFillRatePerSecond;
    uint64_t m_availableTokens; //current number of tokens available in the bucket
    uint64_t m_lastRefillTime;
    uint64_t m_timeIntervalPerRequestInMicroSeconds; // = (pow(10,6)/m_tokenFillRatePerSecond)

    /* to refill the tokens as per time diff */
    void refill_tokens(const uint64_t& fa_currentRequestTime) {
      /*calculate tokens added in particular time difference */
      if (fa_currentRequestTime >= m_lastRefillTime + m_timeIntervalPerRequestInMicroSeconds) {

        uint64_t l_newTokens = (fa_currentRequestTime - m_lastRefillTime)/m_timeIntervalPerRequestInMicroSeconds; //new tokens available in time diff of microsecond level

        if(m_availableTokens + l_newTokens < m_bucketSize) {
          m_availableTokens += l_newTokens;
        }
        else {
          m_availableTokens = m_bucketSize;
        }
        m_lastRefillTime = fa_currentRequestTime; //not update last fill if tokes in bucket are empty or less then 1 to handle 1 request
      }
    }

  public:

    /* Constructor */
    tokenBucket(uint64_t fa_tokenFillRatePerSec): m_tokenFillRatePerSecond(fa_tokenFillRatePerSec),m_bucketSize(fa_tokenFillRatePerSec),m_availableTokens(fa_tokenFillRatePerSec),m_lastRefillTime(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()),m_timeIntervalPerRequestInMicroSeconds(NUMBER_OF_MICROSECONDS_IN_A_SECOND/fa_tokenFillRatePerSec){
      fprintf(stdout,"\nToken Bucket initialised with the following values\nm_bucketSize(%lu)\nm_tokenFillRatePerSecond(%lu)\nm_availableTokens(%lu)\nm_lastRefillTime(%lu)\nm_timeIntervalPerRequestInMicroSeconds(%lu)\n",m_bucketSize,m_tokenFillRatePerSecond,m_availableTokens,m_lastRefillTime,m_timeIntervalPerRequestInMicroSeconds);
    }

    /* always called to check if sufficient number of tokens are available to server the request*/
    bool areTokensAvailable(int fa_numberOfRequestedTokens = 1){
      std::lock_guard<std::mutex> lg(m_mu);
      uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      refill_tokens(now);

      if(m_availableTokens < fa_numberOfRequestedTokens){
        return false;
      }
      else{
        m_availableTokens -= fa_numberOfRequestedTokens;
        return true;
      }
    }
};

#endif
