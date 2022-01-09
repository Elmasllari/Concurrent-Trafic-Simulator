#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "TrafficLight.h"
#include <mutex>




template <typename T>
T MessageQueue<T>::receive()
{
 std::unique_lock<std::mutex> lock(_mutex);
  _condition.wait(lock, [this]{return !_queue.empty();});

 T message = std::move(_queue.back());
  _queue.pop_back();

  
	return message;
}




TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
   while (true) {

    TrafficLightPhase message = M.receive();
    if (message == TrafficLightPhase::green)
      return;
  }
  
}


template <typename T>
void MessageQueue<T>::send(T &&message)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
 	 std::lock_guard<std::mutex> lock(_mutex);
  
    _queue.push_back(std::move(message));
  
    _condition.notify_one();
  
}



TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate() {

 
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
  
 	int max = 6;
    int min = 4;
  
    auto time = std::chrono::system_clock::now();

  while(true){
  
    int randNum = rand()%(max-min + 1) + min;
    
    auto timeNow = std::chrono::system_clock::now();
    auto t = timeNow - time;
    
    if(t >= std::chrono::seconds(randNum))
        {
            if (_currentPhase == green)  _currentPhase = red;
                else _currentPhase = green;
               
        
       time = std::chrono::system_clock::now();
      
       M.send(std::move(_currentPhase));       
      
      std::string light = "red";
       if(_currentPhase == green) light = "green";
      
      std::cout<< "Light is :" << light <<std::endl;
  	    }
    
         std::this_thread::sleep_for(std::chrono::milliseconds(1));           

    
  }
  
  
  
}