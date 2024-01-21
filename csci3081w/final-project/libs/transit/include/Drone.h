#ifndef DRONE_H_
#define DRONE_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

#include "Package.h"
#include "IPublisher.h"
#include "ISubscriber.h"

class Package;

// Represents a drone in a physical system.
// Drones move using euler integration based on a specified
// velocity and direction.
/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity, public IPublisher {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Drone(JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~Drone();


  /**
   * @brief Gets the next delivery in the scheduler
   */
  void getNextDelivery();


  /**
   * @brief Gets drones that are available to pick up a package
   *
   * @return boolean 
   */
  bool getAvailable();


  /**
   * @brief Sets the drones speed, will be used to determine rates for the deliver
   */
  void setSpeed(double newSpeed);

  /**
   * @brief Gets the package a current drone has
   *
   * @return pointer to the package objects
   */
  Package* getPackage();

  /**
   * @brief Sets the simulation model being used
   *
   * @param model A pointer to SimulationModel object containing the 
   * parameters for the model
   */
  void setModel(SimulationModel* model);

  /**
   * @brief Gets the revenue the drone has made
   *
   * @return A float that contains the dollar amount of revenue
   * the drone has
   */
  float getRevenue();

  /**
   * @brief Sets the revenue of the drone
   *
   * @param revenue A float that contains the dollar amount
   * the drone has made
   */
  void setRevenue(float revenue);


  /**
   * @brief Getter for the drones toFinalDestination
   * strategy pattern
   * 
   * @return Returns a pointer to an IStrategy
   */
  IStrategy* getToFinalDestination();

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   */
  void update(double dt);

  /**
   * @brief Removing the copy constructor operator
   * so that drones cannot be copied.
   */
  Drone(const Drone& drone) = delete;

  /**
   * @brief Removing the assignment operator
   * so that drones cannot be copied.
   */
  Drone& operator=(const Drone& drone) = delete;

  /////// IPublisher Functions ///////
    /**
   * @brief subscribes a subscriber object to notifications
   *
   * @param sub A pointer to an ISubscriber object that will
   * subscribe to notifications
   */
  void subscribe(ISubscriber* sub) override;

    /**
   * @brief unsubscribes a subscriber object from notifications
   *
   * @param sub A pointer to an ISubscriber object that will
   * unsubscribed from notifications
   */
  void unsubscribe(ISubscriber* sub) override;

    /**
   * @brief Notifies the subscriber of an event or change related
   * to the drone or package
   *
   * @param entity A pointer to the IEntity object that causes the notifications
   * @param what A string that describes what the event is
   */
  void notify(IEntity *entity, const std::string& what) override;

  float tripRevenue;

 private:
  bool available = false;
  bool pickedUp = false;
  Package* package = nullptr;
  IStrategy* toPackage = nullptr;
  IStrategy* toFinalDestination = nullptr;
  float revenue;
  std::vector<ISubscriber *> subscribers;
};

#endif
