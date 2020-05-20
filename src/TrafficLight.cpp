#include <iostream>
#include <random>
#include <time.h>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    //lock for execution
    std::lock_guard<std::mutex> lock(_mtx);
    //add to queue
    queue.push_back(std::move(msg));
    _cond.notify_one();

}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 

    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    

    //Set cycle duration
    std::random_device seed;
    std::mt19937 eng(seed());
    std::uniform_real_distribution<> distr(4000.0, 6000.0);
    double cycle = distr(eng);
    //Get starting time
    std::chrono::time_point<std::chrono::system_clock> lastUpdate = std::chrono::system_clock::now;
    //starts infinite loop
    while(true){
        //reduce CPU resouce consumption by sleeping for 1ms
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //calculate elapsed time from last update
        long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        //Check if elapsed time is longer than cycle time
        if(elapsedTime >= cycle){
            //flip traffic light state
            if(_currentPhase == TrafficLightPhase::green){
                _currentPhase == TrafficLightPhase::red;
            }
            else{
                _currentPhase == TrafficLightPhase::green;
            }

            //update last update time with current time
            lastUpdate = std::chrono::system_clock::now();

            //inform message queue
            _msgQueue.send(std::move(_currentPhase));
        }
        
    }



}

