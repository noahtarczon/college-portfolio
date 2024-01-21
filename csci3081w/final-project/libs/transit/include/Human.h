#ifndef HUMAN_H_
#define HUMAN_H_

#include "IEntity.h"
#include "IStrategy.h"

class Human : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Human(JsonObject& obj);

 /**
   * @brief Destructor for human
   */
  ~Human();

  /**
   * @brief Updates the Helicpoter's position
   * 
   * @param dt Delta time
   **/
  void update(double dt);

 private:
  IStrategy* movement = nullptr;
};

#endif
