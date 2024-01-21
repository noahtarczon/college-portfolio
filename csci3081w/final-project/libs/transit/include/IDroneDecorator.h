#ifndef IDRONEDECORATOR_H_
#define IDRONEDECORATOR_H_

#include "IEntity.h"
#include "Drone.h"
#include "SimulationModel.h"

/**
 * @brief this class inhertis from the IEntity class and is represents
 * a decorator where the entity will be given existing functionality.
 * Currently, we will be adding a decorator for choosing different speeds
 * for the drone.
 */
class IDroneDecorator : public IEntity {
 protected:
    float timeToDeliver;
    double speed;

 public:
    /**
    * @brief Constructor for an IDroneDecorator
    * @param dt Time elapsed since the last update.
    * @param obj JsonObject containing information for the decorator
    */
    IDroneDecorator(Drone* entity, JsonObject obj);

    // void virtual move(IEntity* entity) = 0;

    /**
    * @brief Virtual function to update the decorated entity's state.
    * @param dt Time elapsed since the last update.
    */
    void virtual update(double dt) = 0;

    /**
    * @brief Links the decorated drone to the simulation model
    * @param model A pointer to the SimulationModel being linked
    */
    void virtual linkModel(SimulationModel* model) = 0;
};

#endif
