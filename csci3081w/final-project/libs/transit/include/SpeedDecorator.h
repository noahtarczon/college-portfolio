#ifndef SPEEDDECORATOR_H_
#define SPEEDDECORATOR_H_

#include "IDroneDecorator.h"
#include "Drone.h"

/**
 * @class SpeedDecorator
 * @brief A decorator class that extends the IDroneDecorator
 * creates speed logic for a done. Based on the speed we adjust the cost/rate for 
 * the drone and track the revenue a drone accumulates
 * 
 */
class SpeedDecorator : public IDroneDecorator {
 private:
    Drone* drone = nullptr;
    float revenue = 0;
    double time = -1;
    float rate = 0;     // set in setter and getter
    double elapsedTime;
    bool flag = false;
    bool isGold = false;

    /**
    * @brief Calculates the revenue generated how much time has elapsed
    * @param dt Time elapsed since the last calculation/update.
    */
    void calcRevenue();

 public:
    /**
    * @brief Constructor for the SpeedDecorator.
    * @param entity A pointer to Drone that will be deocrated
    * @param obj JsonObject containing simulation data 
    */
    SpeedDecorator(Drone* entity, JsonObject obj);

    /**
    * @brief Checks if the decorated drone has arrived in time
    * @return boolean True if the entity has arrived in time, false otherwise.
    */
    bool hasArrivedInTime();

    /**
    * @brief Update the current entities state
    * 
    * @param dt Delta time
    */
    void update(double dt);

    /**
     * @brief Setting the time limit the drone has to arrive in
     * 
     */
    void setTimeLimit();

    /**
     * @brief Links the speed decorator to the simulation model
     * 
     * @param model A pointer to the SimulationModel that is being linked
     *
     */
    void linkModel(SimulationModel* model);
};

#endif
