#ifndef Helicopter_H_
#define Helicopter_H_

#include "IEntity.h"
#include "IStrategy.h"

class Helicopter : public IEntity {
 public:
  /**
   * @brief Helicopter are created with a name
   * @param obj JSON object containing the drone's information
   */
  Helicopter(JsonObject& obj);

  /**
   * @brief Destructor for Helicopter.
   **/
  ~Helicopter();

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
